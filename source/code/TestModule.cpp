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
#include <iomanip>
using namespace std;

TestModule::TestModule()
{
}

TestModule::~TestModule()
{
}

void TestModule::onLoad()
{
    glGenTextures(2, mTextures);
    DisplayEngine::loadTexture("data/images/wood.jpg", mTextures[0]);
    loadCardImage("localuprising.gif", mTextures[1]);
    loadCardImage("liberation.gif", mTextures[2]);


    mCardProgram.attachShader("card.vs");
    mCardProgram.attachShader("card.fs");
    mCardProgram.addVariable("CardVertex");
    mCardProgram.addVariable("CardTextureCoordinate");
    mCardProgram.bindAndLink();
    mUniformUseTexture = mCardProgram.getUniformLocation("UseTexture");
    mUniformCardColor = mCardProgram.getUniformLocation("CardColor");
    CardModel::setUniforms(mCardProgram.getUniformLocation("CardTexture"),
        mUniformUseTexture, mUniformCardColor);
    mCard.build(mCardProgram);

    GLfloat tableVertices[12] = {
        100.0f, 100.0f, 0.0f,
        100.0f, -100.0f, 0.0f,
        -100.0f, -100.0f, 0.0f,
        -100.0f, 100.0f, 0.0f};

    GLfloat tableTextures[8] = {
        0.0f, 0.0f, 20.0f, 0.0f,
        20.0f, 20.0f, 0.0f, 20.0f};

    mTable.loadVAA(mCardProgram.getBinding("CardVertex"), 3, 4, tableVertices);
    mTable.loadVAA(mCardProgram.getBinding("CardTextureCoordinate"), 2, 4,
        tableTextures);

    mMouseMode = NONE;

    glGetIntegerv(GL_VIEWPORT, mViewport.array());
    mProjection.perspective(30.0f, DisplayEngine::getAspectRatio(), 1.0f,
        1000.0f);
    mModelView.reset();
    mCamera.zoom(18.0f);
}

void TestModule::onUnload()
{
    glDeleteTextures(3, mTextures);
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
    mMV = mModelView.matrix();

    mModelView.push();
    mModelView.matrix().translate(mCardTranslate[0], mCardTranslate[1], 0.25f);
    (mMVPM = mProjection).multiply(mModelView.matrix());
    mCardProgram.setMatrix(mMVPM);
    mCard.display(mTextures[1], mTextures[2]);
    mModelView.pop();

    static const Vector3D<float> tableColor(0.0f);
    glUniform1i(mUniformUseTexture, 1);
    glUniform4fv(mUniformCardColor, 1, tableColor.array());
    (mMVPM = mProjection).multiply(mModelView.matrix());
    mCardProgram.setMatrix(mMVPM);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]);
    mTable.displayLinear(GL_QUADS, 0, 4);

    mModelView.pop();
}

void TestModule::onFrame()
{
    mCamera.spin(0.5f);
    mCamera.update();
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

void TestModule::onMouseWheel(bool inUp, bool inDown)
{
    Uint8* keyState = SDL_GetKeyState(NULL);
    if (keyState[SDLK_LSHIFT] || keyState[SDLK_RSHIFT])
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
    int fixedY = SDL_GetVideoSurface()->h - inY;
    GLfloat depthZ;
    glReadPixels(inX, fixedY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthZ);

    GLdouble* a = mPointer.array();

    GLdouble mv[16];
    GLdouble p[16];
    for (size_t i = 0; i < 16; ++i)
    {
        mv[i] = mMV[i];
        p[i] = mProjection[i];
    }

    if (!gluUnProject((GLdouble)inX, (GLdouble)fixedY, depthZ, mv, p,
        mViewport.array(), a, a + 1, a + 2))
    {
        cerr << "failure on gluUnProject" << endl;
        return;
    }

    if (mMouseMode == DRAGGING)
    {
        Vector3D<GLdouble> difference = mPointer - mDragAnchor;
        mCardTranslate[0] = mCardDragSource[0] + difference[0];
        mCardTranslate[1] = mCardDragSource[1] + difference[1];
    }
}
