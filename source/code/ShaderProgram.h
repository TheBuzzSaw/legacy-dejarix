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

        void attachShader(const char* inFile);
        void setMatrix(const Matrix3D& inMatrix);
        GLuint getBinding(const std::string& inName) const;
        void bindAndLink();

        inline void use() { glUseProgram(mHandle); }

        inline void addVariable(const std::string& inName)
        {
            mBindings[inName] = 0;
        }

        inline GLint getUniformLocation(const char* inName) const
        {
            return glGetUniformLocation(mHandle, inName);
        }

    protected:
    private:
        GLuint mHandle;
        Shader** mShaders;
        size_t mCapacity;
        size_t mSize;
        GLint mUniformMatrix;
        bool mLink;
        bool mCreate;

        std::map<std::string, GLuint> mBindings;
};

#endif
