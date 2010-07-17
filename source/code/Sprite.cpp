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

#include "Sprite.h"
#include "DisplayEngine.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

ShaderProgram* Sprite::mShaderProgram = NULL;

Sprite::Sprite(const char* inKey) : mKey(inKey)
{
    string path("data/sprites/");
    path += mKey;
    path += '/';
    string spriteFile(path);
    spriteFile += "static.02d";

    ifstream fin(spriteFile.c_str());
    if (fin.fail())
    {
        cerr << "failed to load sprite: " << spriteFile << endl;
        return;
    }

    char c;
    Frame currentFrame;

    size_t numSheets;
    size_t numFrames;
    getline(fin, mName) >> numSheets >> numFrames >> c;
    mSheets.resize(numSheets);
    mFrames.resize(numFrames);
    while (c != 'x')
    {
        switch (c)
        {
            case 'b':
            {
                fin >> currentFrame.base;
                break;
            }

            case 's':
            {
                fin >> currentFrame.size;
                break;
            }

            case 'f':
            {
                size_t index;
                fin >> index;
                Frame& f = mFrames[index];
                fin >> f.sheet >> f.location;
                f.base = currentFrame.base;
                f.size = currentFrame.size;
                break;
            }

            default:
            {
                cerr << "unrecognized guide: " << c << endl;
                c = 'x';
            }
        }

        if (c != 'x') fin >> c;
    }

    size_t numStates;
    fin >> numStates >> c;
    mStates.resize(numStates);
    size_t currentState = 0;
    size_t currentSize = 0;
    size_t currentDuration = 0;
    size_t currentAnimation = 0;

    while (c != 'x')
    {
        switch (c)
        {
            case 's':
            {
                fin >> currentState >> currentSize;
                mStates[currentState].animation = new SubState[currentSize];
                currentAnimation = 0;
                break;
            }

            case 'd':
            {
                fin >> currentDuration;
                break;
            }

            case 'f':
            {
                SubState ss;
                ss.duration = currentDuration;
                fin >> ss.frame;
                mStates[currentState].animation[currentAnimation] = ss;
                ++currentAnimation;
                break;
            }

            default:
            {
                cerr << "unrecognized guide: " << c << endl;
                c = 'x';
            }
        }

        if (c != 'x') fin >> c;
    }

    fin.close();

    for (size_t i = 0; i < mSheets.size(); ++i)
    {
        stringstream ss;
        ss << path << "sheet-" << i << ".png";
        glGenTextures(1, &mSheets[i].texture);
        Surface s = DisplayEngine::loadImage(ss.str().c_str());

        if (s == NULL)
        {
            cerr << "failed to load sprite sheet: " << ss.str() << endl;
            exit(1);
        }

        mSheets[i].size[0] = s->w;
        mSheets[i].size[1] = s->h;

        DisplayEngine::loadTexture(s, mSheets[i].texture);
    }

    GLfloat* vertices = new GLfloat[mFrames.size() * 16];
    GLfloat* coordinates = new GLfloat[mFrames.size() * 16];

    for (size_t i = 0; i < mFrames.size(); ++i)
    {
        const Frame& f = mFrames[i];
        const Sheet& s = mSheets[f.sheet];

        Point halves;
        halves[0] = float(f.size[0]) / 2.0f;
        halves[1] = float(f.size[1]) / 2.0f;

        float width = float(f.size[0]) / float(s.size[0]);
        float height = float(f.size[1]) / float(s.size[1]);

        Point textureUL;
        textureUL[0] = float(f.location[0]) / float(s.size[0]);
        textureUL[1] = float(f.location[1]) / float(s.size[1]);

        Point textureLR;
        textureLR[0] = textureUL[0] + width;
        textureLR[1] = textureUL[1] + height;

        for (size_t j = 0; j < 2; ++j)
        {
            size_t k = (i * 16) + (j * 8);
            float offset = width * float(j);

            /// upper right
            vertices[k + 0] = halves[0];
            vertices[k + 1] = halves[1];
            coordinates[k + 0] = textureLR[0] + offset;
            coordinates[k + 1] = textureUL[1];

            /// lower right
            vertices[k + 2] = halves[0];
            vertices[k + 3] = -halves[1];
            coordinates[k + 2] = textureLR[0] + offset;
            coordinates[k + 3] = textureLR[1];

            /// lower left
            vertices[k + 4] = -halves[0];
            vertices[k + 5] = -halves[1];
            coordinates[k + 4] = textureUL[0] + offset;
            coordinates[k + 5] = textureLR[1];

            /// upper left
            vertices[k + 6] = -halves[0];
            vertices[k + 7] = halves[1];
            coordinates[k + 6] = textureUL[0] + offset;
            coordinates[k + 7] = textureUL[1];
        }
    }

    try
    {
        if (!mShaderProgram) throw Shader::Exception("no program pointer");

        mSVBO.loadVAA(mShaderProgram->getBinding("CornerVertex"), 2,
            mFrames.size() * 8, vertices);
        mSVBO.loadVAA(mShaderProgram->getBinding("TexCoord"), 2,
            mFrames.size() * 8, coordinates);
    }
    catch (Shader::Exception& se)
    {
        cerr << "sprite shader exception -- " << se.reason << endl;
        exit(1);
    }
    catch (...)
    {
        cerr << "unknown sprite exception" << endl;
        exit(1);
    }

    delete [] coordinates;
    delete [] vertices;
}

Sprite::~Sprite()
{
    for (size_t i = 0; i < mSheets.size(); ++i)
    {
        glDeleteTextures(1, &mSheets[i].texture);
    }

    for (size_t i = 0; i < mStates.size(); ++i)
    {
        delete [] mStates[i].animation;
    }
}

void Sprite::draw(size_t inIndex, bool inFacingRight)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mSheets[mFrames[inIndex].sheet].texture);
    GLint target = inIndex * 8;
    if (inFacingRight) target += 4;
    mSVBO.displayLinear(GL_QUADS, target, 4);
}
