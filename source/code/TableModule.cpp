#include "TableModule.h"
#include "DisplayEngine.h"

#include <iostream>
#include <sstream>
using namespace std;

TableModule::TableModule() : mCardModel(mCardProgram)
{
}

TableModule::~TableModule()
{
}

void TableModule::loadCardImage(const char* inFile, GLuint inTexture)
{
    string s("data/images/");
    s += inFile;
    Surface back = DisplayEngine::blankSurface(512, 512);

    Surface picture = DisplayEngine::loadImage(s.c_str());
    SDL_BlitSurface(picture, NULL, back, NULL);
    SDL_FreeSurface(picture);
    DisplayEngine::loadTexture(back, inTexture);
}

GLuint TableModule::generateTexture()
{
    glGenTextures(1, mTextures + mLoadedTextures);
    ++mLoadedTextures;
    return mTextures[mLoadedTextures - 1];
}

void TableModule::onLoad()
{
    mLoadedTextures = 0;

    mWindowCenter[0] = SDL_GetVideoSurface()->w / 2;
    mWindowCenter[1] = SDL_GetVideoSurface()->h / 2;

    glGenTextures(1, &mTableTexture);
    DisplayEngine::loadTexture("data/images/wood.jpg", mTableTexture);

    mCardSelect = NULL;
    GLuint back = generateTexture();
    loadCardImage("cards/ds.jpg", back);

    ifstream fin("data/decks/ds2dark.htd");
    if (fin.fail())
        throw Module::Exception("failed to open dark deck");

    string oneLine;
    Vector3D<float> offset;
    offset[2] = 0.025f;
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

        //cerr << cardFile << endl;

        GLuint front = generateTexture();
        loadCardImage(cardFile.c_str(), front);
        CardInstance* ci = new CardInstance(front, back);
        ci->setPosition(offset);
        offset[0] += 6.5f;
        mCards.push_back(ci);
    }

    fin.close();

    GLfloat tableVertices[12] = {
        500.0f, 500.0f, 0.0f,
        500.0f, -500.0f, 0.0f,
        -500.0f, -500.0f, 0.0f,
        -500.0f, 500.0f, 0.0f};

    GLfloat tableTextures[8] = {
        0.0f, 0.0f, 100.0f, 0.0f,
        100.0f, 100.0f, 0.0f, 100.0f};

    mTableVBO.loadVAA(CardProgram::VERTEX, 3, 4, tableVertices);
    mTableVBO.loadVAA(CardProgram::TEXTURE, 2, 4, tableTextures);

    mMouseMode = NONE;

    glGetIntegerv(GL_VIEWPORT, mViewport.array());
    mProjection.perspective(30.0f, DisplayEngine::getAspectRatio(), 1.0f,
        1000.0f);
    mProjectionHUD.orthographic(5.0f, DisplayEngine::getAspectRatio());
    mModelView.reset();
    mCamera.zoom(18.0f);
    mKeyState = SDL_GetKeyState(NULL);
}

void TableModule::onUnload()
{
    glDeleteTextures(1, &mTableTexture);
    glDeleteTextures(mLoadedTextures, mTextures);
}

void TableModule::onOpen()
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

void TableModule::onClose()
{
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

void TableModule::onFrame()
{
    mCamera.smartPan(mSmartPanning);
    mCamera.update();
    processPosition();

    if (mMouseMode == NONE)
    {
        mCardSelect = NULL;
        for (list<CardInstance*>::iterator i = mCards.begin();
            i != mCards.end(); ++i)
        {
            if ((*i)->isInside(mPointer))
            {
                mCardSelect = *i;
                break;
            }
        }
    }
    else if (mMouseMode == DRAGGING)
    {
        Vector3D<float> difference = mPointer - mDragAnchor;
        Vector3D<float> cardTranslate(mCardSelect->getPosition());
        cardTranslate[0] = mCardAnchor[0] + difference[0];
        cardTranslate[1] = mCardAnchor[1] + difference[1];
        mCardSelect->setPosition(cardTranslate);
    }
}

void TableModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mModelView.push();
    mModelView.matrix().multiply(mCamera.matrix());

    /// render table
    static const Vector3D<float> tableColor(0.0f);
    mCardProgram.setColor(tableColor);
    (mMVPM = mProjection).multiply(mModelView.matrix());
    mCardProgram.setMatrix(mMVPM);
    glBindTexture(GL_TEXTURE_2D, mTableTexture);
    mTableVBO.displayLinear(GL_QUADS, 0, 4);

    /// render all cards
    for (list<CardInstance*>::iterator i = mCards.begin(); i != mCards.end();
        ++i)
    {
        CardInstance* ci = *i;
        mModelView.push();
        mModelView.matrix().multiply(ci->matrix());
        (mMVPM = mProjection).multiply(mModelView.matrix());
        mCardProgram.setMatrix(mMVPM);
        if (mCardSelect == ci)
            mCardProgram.setColor(mMouseMode == DRAGGING
                ? Vector3D<float>(0.2f, 0.2f, 0.4f) : Vector3D<float>(0.2f));
        mCardModel.display(ci->front(), ci->back());
        if (mCardSelect == ci) mCardProgram.setColor(Vector3D<float>(0.0f));
        mModelView.pop();
    }
    glReadPixels(mMouseCoordinates[0], mFixedY, 1, 1, GL_DEPTH_COMPONENT,
        GL_FLOAT, &mDepthZ);

    /// draw HUD
    //glClear(GL_DEPTH_BUFFER_BIT);
    //(mMVPM = mProjectionHUD).multiply(mModelView.matrix());

    mModelView.pop();
}

void TableModule::processPosition()
{
    if (!unproject((float)mMouseCoordinates[0], (float)mFixedY, mDepthZ,
        mCamera.matrix(), mProjection, mViewport, mPointer))
    {
        cerr << "failure on gluUnProject" << endl;
        return;
    }
}

bool TableModule::unproject(float inX, float inY, float inZ,
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

void TableModule::transformPoint(const Matrix3D& inMatrix,
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

void TableModule::onMouseWheel(bool inUp, bool inDown)
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

void TableModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    mMouseCoordinates[0] = inX;
    mMouseCoordinates[1] = inY;
    mFixedY = SDL_GetVideoSurface()->h - inY;
    processPosition();
}

void TableModule::onLButtonDown(int inX, int inY)
{
    switch (mMouseMode)
    {
        case NONE:
        {
            if (!mCardSelect) break;
            mMouseMode = DRAGGING;
            mDragAnchor = mPointer;
            mCardAnchor = mCardSelect->getPosition();
            break;
        }

        case DRAGGING:
        {
            break;
        }

        default: {}
    }
}

void TableModule::onLButtonUp(int inX, int inY)
{
    switch (mMouseMode)
    {
        case NONE:
        {
            break;
        }

        case DRAGGING:
        {
            mMouseMode = NONE;
            break;
        }

        default: {}
    }
}

void TableModule::onRButtonDown(int inX, int inY)
{
}

void TableModule::onRButtonUp(int inX, int inY)
{
}

void TableModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_ESCAPE:
        {
            mRunning = false;
            break;
        }

        case SDLK_LEFT:
        {
            mSmartPanning[0] = -1.0f;
            break;
        }

        case SDLK_RIGHT:
        {
            mSmartPanning[0] = 1.0f;
            break;
        }

        case SDLK_UP:
        {
            mSmartPanning[1] = 1.0f;
            break;
        }

        case SDLK_DOWN:
        {
            mSmartPanning[1] = -1.0f;
            break;
        }

        default: {}
    }
}

void TableModule::onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_LEFT:
        case SDLK_RIGHT:
        {
            mSmartPanning[0] = 0.0f;
            break;
        }

        case SDLK_UP:
        case SDLK_DOWN:
        {
            mSmartPanning[1] = 0.0f;
            break;
        }

        default: {}
    }
}
