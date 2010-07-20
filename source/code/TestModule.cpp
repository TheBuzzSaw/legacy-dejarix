/**
 *  This file is part of Dejarix.
 *
 *  Dejarix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Dejarix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dejarix.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TestModule.h"
#include "Config.h"
#include "DisplayEngine.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

TestModule::TestModule() : mCard(mCardProgram)
{
}

TestModule::~TestModule()
{
}

void TestModule::loadCardImage(const char* inFile, GLuint inTexture)
{
    string s("data/images/");
    s += inFile;
    Surface back = DisplayEngine::blankSurface(512, 512);

    Surface picture = DisplayEngine::loadImage(s.c_str());
    SDL_BlitSurface(picture, NULL, back, NULL);
    SDL_FreeSurface(picture);
    DisplayEngine::loadTexture(back, inTexture);
}

void TestModule::onLoad()
{
    glGenTextures(NUM_TEXTURES, mTextures);
    DisplayEngine::loadTexture("data/images/wood.jpg", mTextures[0]);

    ifstream fin("data/decks/ds2dark.htd");
    if (fin.fail())
        throw Module::Exception("failed to open dark deck");

    string oneLine;
    size_t targetTexture = 1;
    while (getline(fin, oneLine))
    {
        if (oneLine.substr(0, 4) != "card") continue;

        stringstream ss;
        ss << oneLine;
        string s;

        ss >> s >> s;
        s = s.substr(1, s.length() - 2);
        size_t t = s.find_last_of('/');
        string cardFile("cards/");
        cardFile += s.substr(1, t - 1);
        cardFile += "/large/";
        cardFile += s.substr(t + 3);
        cardFile += ".gif";

        cerr << cardFile << endl;

        loadCardImage(cardFile.c_str(), mTextures[targetTexture]);
        ++targetTexture;
    }

    fin.close();

    GLfloat tableVertices[12] = {
        100.0f, 100.0f, 0.0f,
        100.0f, -100.0f, 0.0f,
        -100.0f, -100.0f, 0.0f,
        -100.0f, 100.0f, 0.0f};

    GLfloat tableTextures[8] = {
        0.0f, 0.0f, 20.0f, 0.0f,
        20.0f, 20.0f, 0.0f, 20.0f};

    mTable.loadVAA(CardProgram::VERTEX, 3, 4, tableVertices);
    mTable.loadVAA(CardProgram::TEXTURE, 2, 4, tableTextures);

    mMouseMode = NONE;

    glGetIntegerv(GL_VIEWPORT, mViewport.array());
    mProjection.perspective(30.0f, DisplayEngine::getAspectRatio(), 1.0f,
        1000.0f);
    for (size_t i = 0; i < 16; ++i) mProjectionArray[i] = mProjection[i];
    mModelView.reset();
    mCamera.zoom(18.0f);
    mSpin = false;
    mKeyState = SDL_GetKeyState(NULL);
    mCurrentTexture = 1;
}

void TestModule::onUnload()
{
    glDeleteTextures(NUM_TEXTURES, mTextures);
}

void TestModule::onOpen()
{
    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
}

void TestModule::onClose()
{
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

void TestModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mModelView.push();
    mModelView.matrix().multiply(mCamera.matrix());

    mModelView.push();
    mModelView.matrix().translate(mCardTranslate[0], mCardTranslate[1], 0.25f);
    //mModelView.matrix().scale(1.0f, 1.0f, 10.0f);
    (mMVPM = mProjection).multiply(mModelView.matrix());
    mCardProgram.setMatrix(mMVPM);
    //mCardProgram.setColor(Vector3D<float>(0.5f, 0.0f, 0.0f, 0.5f));
    mCardProgram.setColor(mCardColor);
    mCard.display(mTextures[mCurrentTexture], mTextures[mCurrentTexture]);
    mModelView.pop();

    static const Vector3D<float> tableColor(0.0f);
    mCardProgram.useTexture(true);
    mCardProgram.setColor(tableColor);
    (mMVPM = mProjection).multiply(mModelView.matrix());
    mCardProgram.setMatrix(mMVPM);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]);
    mTable.displayLinear(GL_QUADS, 0, 4);

    mModelView.pop();
}

void TestModule::onFrame()
{
    if (mSpin) mCamera.spin(0.5f);
    mCamera.update();
    processPosition();

    float w = 6.3f / 2.0f;
    float h = 8.8f / 2.0f;
    if (mPointer[0] >= mCardTranslate[0] - w
        && mPointer[0] <= mCardTranslate[0] + w
        && mPointer[1] >= mCardTranslate[1] - h
        && mPointer[1] <= mCardTranslate[1] + h)
    {
        if (mMouseMode == NONE)
            mCardColor.set(0.2f, 0.2f, 0.2f);
        else if (mMouseMode == DRAGGING)
            mCardColor.set(0.2f, 0.2f, 0.4f);
    }
    else
    {
        mCardColor.set(0.0f);
    }

    if (mMouseMode == DRAGGING)
    {
        Vector3D<GLfloat> difference = mPointer - mDragAnchor;
        mCardTranslate[0] = mCardDragSource[0] + difference[0];
        mCardTranslate[1] = mCardDragSource[1] + difference[1];
    }
}

void TestModule::onMouseWheel(bool inUp, bool inDown)
{
    if (mKeyState[SDLK_LSHIFT] || mKeyState[SDLK_RSHIFT])
    {
        mCamera.rise(inUp ? 5.0f : -5.0f);
    }
    else
    {
        mCamera.zoom(inUp ? -2.0f : 2.0f);
    }
}

void TestModule::onLButtonDown(int inX, int inY)
{
    mMouseMode = DRAGGING;
    mDragAnchor = mPointer;
    mCardDragSource = mCardTranslate;
}

void TestModule::onLButtonUp(int inX, int inY)
{
    mMouseMode = NONE;
}

void TestModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    mMouseCoordinates[0] = inX;
    mMouseCoordinates[1] = inY;
    processPosition();
}

void TestModule::processPosition()
{
    int fixedY = SDL_GetVideoSurface()->h - mMouseCoordinates[1];
    GLfloat depthZ;
    glReadPixels(mMouseCoordinates[0], fixedY, 1, 1, GL_DEPTH_COMPONENT,
        GL_FLOAT, &depthZ);

    if (!unproject((float)mMouseCoordinates[0], (float)fixedY, depthZ,
        mCamera.matrix(), mProjection, mViewport, mPointer))
    {
        cerr << "failure on gluUnProject" << endl;
        return;
    }
}

void TestModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_ESCAPE:
        {
            mRunning = false;
            break;
        }

        case SDLK_SPACE:
        {
            mSpin = !mSpin;
            break;
        }

        case SDLK_PLUS:
        case SDLK_KP_PLUS:
        {
            ++mCurrentTexture;
            if (mCurrentTexture >= NUM_TEXTURES) mCurrentTexture = 1;
            break;
        }

        case SDLK_MINUS:
        case SDLK_KP_MINUS:
        {
            --mCurrentTexture;
            if (mCurrentTexture < 1) mCurrentTexture = NUM_TEXTURES - 1;
            break;
        }

        default:
        {
        }
    }
}

bool TestModule::unproject(float inX, float inY, float inZ,
    const Matrix3D& inMVM, const Matrix3D& inPM,
    const Vector3D<int>& inViewport, Vector3D<float>& inResult)
{
    Vector3D<float> in;

    in[0] = (inX - inViewport[0]) * 2.0f / inViewport[2] - 1.0f;
    in[1] = (inY - inViewport[1]) * 2.0f / inViewport[3] - 1.0f;
    in[2] = 2 * inZ - 1.0f;
    in[3] = 1.0f;

    Matrix3D a(inPM);
    a.multiply(inMVM);
    Matrix3D m;
    a.inverse(m);
    transformPoint(m, in, inResult);
    if (inResult[3] == 0.0f) return false;
    inResult[0] /= inResult[3];
    inResult[1] /= inResult[3];
    inResult[2] /= inResult[3];
    inResult[3] = 1.0f;
    return true;
}

void TestModule::transformPoint(const Matrix3D& inMatrix,
    const Vector3D<float>& inVector, Vector3D<float>& inResult)
{
    inResult[0] = inMatrix(0, 0) * inVector[0] + inMatrix(0, 1) * inVector[1]
        + inMatrix(0, 2) * inVector[2] + inMatrix(0, 3) * inVector[3];
    inResult[1] = inMatrix(1, 0) * inVector[0] + inMatrix(1, 1) * inVector[1]
        + inMatrix(1, 2) * inVector[2] + inMatrix(1, 3) * inVector[3];
    inResult[2] = inMatrix(2, 0) * inVector[0] + inMatrix(2, 1) * inVector[1]
        + inMatrix(2, 2) * inVector[2] + inMatrix(2, 3) * inVector[3];
    inResult[3] = inMatrix(3, 0) * inVector[0] + inMatrix(3, 1) * inVector[1]
        + inMatrix(3, 2) * inVector[2] + inMatrix(3, 3) * inVector[3];
}
