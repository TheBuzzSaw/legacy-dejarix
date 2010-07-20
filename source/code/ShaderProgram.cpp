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

#include "ShaderProgram.h"

#include <iostream>
using namespace std;

ShaderProgram::ShaderProgram(size_t inCapacity) : mHandle(0),
    mCapacity(inCapacity), mSize(0), mLink(false)
{
    if (mCapacity < 2) mCapacity = 2;
    mShaders = new Shader*[mCapacity];
    mHandle = glCreateProgram();
    if (!mHandle)
        throw Shader::Exception("unable to create program (glCreateProgram)");
}

ShaderProgram::~ShaderProgram()
{
    for (size_t i = 0; i < mSize; ++i)
        glDetachShader(mHandle, mShaders[i]->handle());

    glDeleteProgram(mHandle);
    delete [] mShaders;
}

void ShaderProgram::attachShader(const char* inFile)
{
    if (inFile == NULL || mSize >= mCapacity)
        throw Shader::Exception(string("exceeded shader capacity -- ")
            + inFile);

    Shader* s = Shader::load(inFile);

    glAttachShader(mHandle, s->handle());
    mShaders[mSize] = s;
    ++mSize;
}

void ShaderProgram::linkAndBind()
{
    // can only be bound and linked once
    if (mLink) throw Shader::Exception("shader program already linked");
    mLink = true;

    glLinkProgram(mHandle);

    GLint linked;
    glGetProgramiv(mHandle, GL_LINK_STATUS, &linked);

    if (!linked)
        throw Shader::Exception("failed to link program (glLinkProgram)");

    use();

    mUniformMatrix = glGetUniformLocation(mHandle, "MVPM");
    bindUniforms();
}

void ShaderProgram::setMatrix(const Matrix3D& inMatrix)
{
    glUniformMatrix4fv(mUniformMatrix, 1, GL_FALSE, inMatrix.array());
}
