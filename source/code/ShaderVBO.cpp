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

#include "ShaderVBO.h"

ShaderVBO::ShaderVBO()
{
}

ShaderVBO::~ShaderVBO()
{
    for (size_t i = 0; i < mData.size(); ++i)
        glDeleteBuffers(1, &mData[i].VBOindex);
}

void ShaderVBO::displayLinear(GLenum inMode, GLint inFirst, GLsizei inCount)
{
    for (size_t i = 0; i < mData.size(); ++i)
    {
        DataArray& da = mData[i];
        glBindBuffer(GL_ARRAY_BUFFER, da.VBOindex);
        glVertexAttribPointer(da.VAindex, da.valuesPerVertex, GL_FLOAT,
            GL_FALSE, 0, 0);
        glEnableVertexAttribArray(da.VAindex);
    }

    glDrawArrays(inMode, inFirst, inCount);

    for (size_t i = 0; i < mData.size(); ++i)
    {
        glDisableVertexAttribArray(mData[i].VAindex);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ShaderVBO::displayIndexed(const IndexVBO& inVBO)
{
    for (size_t i = 0; i < mData.size(); ++i)
    {
        DataArray& da = mData[i];
        glBindBuffer(GL_ARRAY_BUFFER, da.VBOindex);
        glVertexAttribPointer(da.VAindex, da.valuesPerVertex, GL_FLOAT,
            GL_FALSE, 0, 0);
        glEnableVertexAttribArray(da.VAindex);
    }

    inVBO.draw();

    for (size_t i = 0; i < mData.size(); ++i)
    {
        glDisableVertexAttribArray(mData[i].VAindex);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/// load Vertex Attribute Array
void ShaderVBO::loadVAA(GLuint inVAindex, GLuint inValuesPerVertex,
    GLuint inSize, const GLfloat* inData, GLenum inUsage)
{
    DataArray da;
    da.VAindex = inVAindex;
    da.valuesPerVertex = inValuesPerVertex;
    glGenBuffers(1, &da.VBOindex);
    glBindBuffer(GL_ARRAY_BUFFER, da.VBOindex);
    glBufferData(GL_ARRAY_BUFFER, inSize * sizeof(GLfloat) * inValuesPerVertex,
        inData, inUsage);
    mData.push_back(da);
}
