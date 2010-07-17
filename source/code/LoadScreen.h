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

#ifndef LOADSCREEN_H
#define LOADSCREEN_H

#include "DisplayEngine.h"
#include "ShaderProgram.h"
#include "Matrix3D.h"

#include <cmath>
#include <string>

#include <iostream>
using namespace std;

class LoadScreen
{
    public:
        LoadScreen();
        virtual ~LoadScreen();

        void update(int inPercent);
        void update(float inPercent);

        void setBackgroundImage(const char* inBackgroundImage);
        void setLoadImage(const char* inLoadImage);
        void setLoadLocation(int inX, int inY);

        void setup();

    private:
        float mScreenRange;
        int mLoadLocationX;
        int mLoadLocationY;
        int mLoadScreenLocationX;
        int mLoadScreenLocationY;
        int mLoadWidth;
        int mLoadHeight;

        Matrix3D mProjection;
        Matrix3D mMVPM;
        GLuint mBackTexture;
        GLuint mLoadTexture;

        ShaderVBO mLoadingBarVBO;
        ShaderProgram mProgram;
        ShaderVBO mBackVBO;
};

#endif // LOADSCREEN_H
