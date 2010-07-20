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

#ifndef _DISPLAYENGINE_H_
#define _DISPLAYENGINE_H_

#include "Vector2D.h"
#include "LogFile.h"

#include <SDL.h>


#include "OGL.h"
typedef SDL_Surface* Surface;

#include <iostream>

#define ENGINE_FPS 30
#define FRAME_LENGTH (1000 / ENGINE_FPS)
#define FIELD_OF_VIEW 30.0
#define NEAR_CP 1.0
#define FAR_CP 10000.0

#define VERTEX_DATA 0
#define INDEX_DATA 1
#define NORMAL_DATA 2
#define TEXTURE_DATA 3

struct ColorMask
{
    Uint32 red;
    Uint32 green;
    Uint32 blue;
    Uint32 alpha;
};

class Module;

class DisplayEngine
{
    public:
        static void start(Module* inModule);
        static void initialize();
        static Surface blankSurface(int inWidth, int inHeight);
        static Surface loadImage(const char* inFile);
        static bool loadTexture(Surface inSurface, GLuint inTexture,
            bool inDelete = true);
        static bool loadTexture(const char* inFile, GLuint inTexture);
        static void openGLDriverInfo(std::ostream& inStream);

        static int processKey(SDLKey inSym, SDLMod inMod);

        static inline const std::string& getShaderFolder()
        {
            return mShaderFolder;
        }

        static inline float getAspectRatio()
        {
            return mAspectRatio;
        }

        static inline void render();

    private:
        static void cleanup();


        static Surface mWindowIcon;
        static SDL_Rect** mModes;
        static unsigned int mNextFrame;
        static unsigned int mCurrentFrame;
        static int mMipmapping;
        static ColorMask mMask;
        static std::string mShaderFolder;
        static float mAspectRatio;


        static LogFile mLogFile;
};

template<class T>
T linearInterpolate(const T& inMin, const T& inMax, double inT)
{
    return (inMin * (1.0 - inT)) + (inMax * inT);
}

inline void DisplayEngine::render()
{
    SDL_GL_SwapBuffers();
}

#endif
