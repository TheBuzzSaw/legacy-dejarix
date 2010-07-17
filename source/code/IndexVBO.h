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

#ifndef INDEXVBO_H
#define INDEXVBO_H

#include "OGL.h"

class IndexVBO
{
    public:
        IndexVBO(GLenum inDraw = GL_TRIANGLES);
        ~IndexVBO();

        void loadData(size_t inSize, const GLuint* inData,
            GLenum inUsage = GL_STATIC_DRAW);

        inline void draw() const
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
            glDrawElements(mDraw, mSize, GL_UNSIGNED_INT, 0);
        }

    private:
        GLenum mDraw;
        GLuint mBuffer;
        size_t mSize;
};

#endif
