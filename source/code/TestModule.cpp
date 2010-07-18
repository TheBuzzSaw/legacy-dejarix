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
    loadCardImage("localuprising.gif", mTextures[0]);
    loadCardImage("liberation.gif", mTextures[1]);

    mCardProgram.attachShader("card.vs");
    mCardProgram.attachShader("card.fs");
    mCardProgram.addVariable("CardVertex");
    mCardProgram.addVariable("CardTextureCoordinate");
    mCardProgram.bindAndLink();
    CardModel::setUniforms(mCardProgram.getUniformLocation("CardTexture"),
        mCardProgram.getUniformLocation("UseTexture"),
        mCardProgram.getUniformLocation("CardColor"));
    mCard.build(mCardProgram);

    mProjection.perspective(30.0f, DisplayEngine::getAspectRatio(), 1.0f, 1000.0f);
    //mProjection.orthographic(5.0f, DisplayEngine::getAspectRatio());
    mModelView.reset();
    mModelView.matrix().translate(0.0f, 0.0f, -20.0f);
    mRotation = 0.0f;
}

void TestModule::onUnload()
{
    glDeleteTextures(2, mTextures);
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
    mModelView.matrix().rotateY(mRotation);
    mModelView.matrix().scale(1.0f, 1.0f, 10.0f);
    //mModelView.matrix().scale(5.0f);
    (mMVPM = mProjection).multiply(mModelView.matrix());
    mCardProgram.setMatrix(mMVPM);
    mCard.display(mTextures[0], mTextures[1]);
    mModelView.pop();
}

void TestModule::onFrame()
{
    mRotation += 0.5f;
    if (mRotation > 180.0f) mRotation -= 360.0f;
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
