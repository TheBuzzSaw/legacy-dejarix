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

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Shader.h"
#include "ShaderVBO.h"
#include "Matrix3D.h"

#include <map>

class ShaderProgram
{
    public:
        ShaderProgram(size_t inCapacity = 2);
        virtual ~ShaderProgram();

        void setMatrix(const Matrix3D& inMatrix);
        inline void use() { glUseProgram(mHandle); }

    protected:
        void attachShader(const char* inFile);
        void linkAndBind();
        virtual void bindUniforms() = 0;

        inline GLuint handle() { return mHandle; }

    private:
        GLuint mHandle;
        Shader** mShaders;
        size_t mCapacity;
        size_t mSize;
        GLint mUniformMatrix;
        bool mLink;
};

#endif
