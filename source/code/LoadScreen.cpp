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

#include "LoadScreen.h"

LoadScreen::LoadScreen()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DisplayEngine::render();
}

LoadScreen::~LoadScreen()
{
    glDisable(GL_BLEND);
}

void LoadScreen::update(int inPercent)
{
    float percent = inPercent % 101;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mProgram.use();
    glBindTexture(GL_TEXTURE_2D, mBackTexture);
    mBackVBO.displayLinear(GL_QUADS, 0, 4);

    glBindTexture(GL_TEXTURE_2D, mLoadTexture);

    glEnable(GL_SCISSOR_TEST);
    glScissor(mLoadScreenLocationX, mLoadScreenLocationY,
              int(mLoadWidth * (percent / 100.0f)), mLoadHeight);
    mLoadingBarVBO.displayLinear(GL_QUADS, 0, 4);

    glDisable(GL_SCISSOR_TEST);
    DisplayEngine::render();
}

void LoadScreen::update(float inPercent)
{
    update(inPercent * 100);
}

void LoadScreen::setup()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    mProgram.attachShader("sprite.vs");
    mProgram.attachShader("sprite.fs");
    mProgram.addVariable("CornerVertex");
    mProgram.addVariable("TexCoord");
    mProgram.bindAndLink();

    GLfloat* vertices = new GLfloat[8];
    GLfloat* texCoord = new GLfloat[8];

    float ratio = DisplayEngine::getAspectRatio();
    mScreenRange = DisplayEngine::getDisplayHeight() / 2;
    mProjection.orthographic(mScreenRange, ratio);

    mLoadScreenLocationX = mLoadLocationX + int(mScreenRange * ratio) - mLoadWidth / 2;
    mLoadScreenLocationY = mLoadLocationY + int(mScreenRange) - mLoadHeight / 2;

    vertices[0] = mScreenRange * ratio;
    vertices[1] = mScreenRange;
    vertices[2] = mScreenRange * ratio;
    vertices[3] = -mScreenRange;
    vertices[4] = -mScreenRange * ratio;
    vertices[5] = -mScreenRange;
    vertices[6] = -mScreenRange * ratio;
    vertices[7] = mScreenRange;


    texCoord[0] = 1.0f;
    texCoord[1] = 0.0f;
    texCoord[2] = 1.0f;
    texCoord[3] = 1.0f;
    texCoord[4] = 0.0f;
    texCoord[5] = 1.0f;
    texCoord[6] = 0.0f;
    texCoord[7] = 0.0f;

    mBackVBO.loadVAA(mProgram.getBinding("CornerVertex"), 2, 4,
        vertices);
    mBackVBO.loadVAA(mProgram.getBinding("TexCoord"), 2, 4,
        texCoord);

    GLint texLoc = mProgram.getUniformLocation("Texture");
    glUniform1i(texLoc, 0);

    GLint z = mProgram.getUniformLocation("z");
    glUniform1f(z, 0.0f);

    mProgram.setMatrix(mProjection);


    vertices[0] = mLoadLocationX + (mLoadWidth / 2);
    vertices[1] = mLoadLocationY + (mLoadHeight / 2);
    vertices[2] = mLoadLocationX + (mLoadWidth / 2);
    vertices[3] = mLoadLocationY - (mLoadHeight / 2);
    vertices[4] = mLoadLocationX - (mLoadWidth / 2);
    vertices[5] = mLoadLocationY - (mLoadHeight / 2);
    vertices[6] = mLoadLocationX - (mLoadWidth / 2);
    vertices[7] = mLoadLocationY + (mLoadHeight / 2);


    texCoord[0] = 1.0f;
    texCoord[1] = 0.0f;
    texCoord[2] = 1.0f;
    texCoord[3] = 1.0f;
    texCoord[4] = 0.0f;
    texCoord[5] = 1.0f;
    texCoord[6] = 0.0f;
    texCoord[7] = 0.0f;

    mLoadingBarVBO.loadVAA(mProgram.getBinding("CornerVertex"), 2, 4,
        vertices);
    mLoadingBarVBO.loadVAA(mProgram.getBinding("TexCoord"), 2, 4,
        texCoord);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLint fade = mProgram.getUniformLocation("fade");
    glUniform1f(fade, 1.0);

}

void LoadScreen::setBackgroundImage(const char* inBackgroundImage)
{
    glGenTextures(1, &mBackTexture);
    std::string path = "data/images/";
    path += inBackgroundImage;
    DisplayEngine::loadTexture(path.c_str(), mBackTexture);
}

void LoadScreen::setLoadImage(const char* inLoadImage)
{
    glGenTextures(1, &mLoadTexture);
    std::string path = "data/images/";
    path += inLoadImage;
    Surface load = DisplayEngine::loadImage(path.c_str());
    mLoadWidth = load->w;
    mLoadHeight = load->h;
    //DisplayEngine::loadTexture(path.c_str(), mLoadTexture);
    DisplayEngine::loadTexture(load, mLoadTexture);
}

/**********************************************
* Sets the location for the loading bar.
* Takes two int values because range is always set
* to match the screen pixels, but the values should
* be based on opengl's coordinate system (e.g. with
* 0,0 at the center.
***********************************************/
void LoadScreen::setLoadLocation(int inX, int inY)
{
    mLoadLocationX = inX;
    mLoadLocationY = inY;
}
