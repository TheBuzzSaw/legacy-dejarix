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

#ifndef SPRITE_H
#define SPRITE_H

#include "ShaderProgram.h"
#include "Vector2D.h"

#include <string>
#include <vector>

class Sprite
{
    public:
        Sprite(const char* inTitle);
        ~Sprite();

        struct Frame
        {
            Pixel location;
            Pixel size;
            Pixel base;
            size_t sheet;
        };

        struct Sheet
        {
            GLuint texture;
            Pixel size;
        };

        struct SubState
        {
            size_t frame;
            unsigned int duration;
        };

        struct State
        {
            size_t size;
            SubState* animation;
        };

        void draw(size_t inIndex, bool inFacingRight);

        static inline void setProgram(ShaderProgram* inProgram)
        {
            mShaderProgram = inProgram;
        }

    private:
        static ShaderProgram* mShaderProgram;

        ShaderVBO mSVBO;
        const std::string mKey;
        std::string mName;

        std::vector<Frame> mFrames;
        std::vector<Sheet> mSheets;
        std::vector<State> mStates;
};

#endif
