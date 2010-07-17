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

#include "Matrix3D.h"

#include <iomanip>
using namespace std;

const float Matrix3D::mIdentity[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f};

Matrix3D::Matrix3D()
{
    // default to identity matrix
    loadIdentity();
}

Matrix3D::Matrix3D(const Matrix3D& inMatrix)
{
    copy(inMatrix);
}

void Matrix3D::loadIdentity()
{
    memcpy(mData, mIdentity, 16 * sizeof(float));
}

void Matrix3D::rotate(float inDegrees, float inX, float inY, float inZ)
{
    /// arbitrary rotation about an axis
    /// http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml

    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float ci = 1.0f - c;
    float s = sin(r);
    Matrix3D transform;

    transform[0] = inX * inX * ci + c;
    transform[1] = inX * inY * ci - (inZ * s);
    transform[2] = inX * inZ * ci + (inY * s);
    transform[4] = inY * inX * ci + (inZ * s);
    transform[5] = inY * inY * ci + c;
    transform[6] = inY * inZ * ci - (inX * s);
    transform[8] = inX * inZ * ci - (inY * s);
    transform[9] = inY * inZ * ci + (inX * s);
    transform[10] = inZ * inZ * ci + c;

    multiply(transform);
}

void Matrix3D::rotateX(float inDegrees)
{
    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float s = sin(r);
    Matrix3D transform;

    transform[5] = c;
    transform[6] = -s;
    transform[9] = s;
    transform[10] = c;

    multiply(transform);
}

void Matrix3D::rotateY(float inDegrees)
{
    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float s = sin(r);
    Matrix3D transform;

    transform[0] = c;
    transform[2] = s;
    transform[8] = -s;
    transform[10] = c;

    multiply(transform);
}

void Matrix3D::rotateZ(float inDegrees)
{
    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float s = sin(r);
    Matrix3D transform;

    transform[0] = c;
    transform[1] = -s;
    transform[4] = s;
    transform[5] = c;

    multiply(transform);
}

void Matrix3D::scale(float inScale)
{
    Matrix3D transform;

    transform[0] = inScale;
    transform[5] = inScale;
    transform[10] = inScale;

    multiply(transform);
}

void Matrix3D::scale(float inX, float inY, float inZ)
{
    Matrix3D transform;

    transform[0] = inX;
    transform[5] = inY;
    transform[10] = inZ;

    multiply(transform);
}

void Matrix3D::translate(float inX, float inY, float inZ)
{
    Matrix3D transform;

    transform[3] = inX;
    transform[7] = inY;
    transform[11] = inZ;

    multiply(transform);
}

void Matrix3D::frustrum(float inLeft, float inRight, float inBottom,
    float inTop, float inNear, float inFar)
{
    Matrix3D transform;

    transform[0] = (2 * inNear) / (inRight - inLeft);
    transform[2] = (inRight + inLeft) / (inRight - inLeft);
    transform[5] = (2 * inNear) / (inTop - inBottom);
    transform[6] = (inTop + inBottom) / (inTop - inBottom);
    transform[10] = (inFar + inNear) / (inNear - inFar);
    transform[11] = (2.0f * inFar * inNear) / (inNear - inFar);
    transform[14] = -1.0f;
    transform[15] = 0.0f;

    multiply(transform);
}

void Matrix3D::perspective(float inFieldOfView, float inRatio, float inNear,
    float inFar)
{
    /// adaptation of gluPerspective
    /// http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
    float r = TO_RADIANS(inFieldOfView);
    float f = 1.0f / tan(r / 2.0f);

    Matrix3D transform;

    transform[0] = f / inRatio;
    transform[5] = f;
    transform[10] = (inFar + inNear) / (inNear - inFar);
    transform[11] = (2 * inFar * inNear) / (inNear - inFar);
    transform[14] = -1.0f;
    transform[15] = 0.0f;

    multiply(transform);
}

void Matrix3D::orthographic(float inLeft, float inRight, float inBottom,
    float inTop, float inNear, float inFar)
{
    Matrix3D transform;

    transform[0] = 2.0f / (inRight - inLeft);
    transform[3] = (inRight + inLeft) / (inRight - inLeft);
    transform[5] = 2.0f / (inTop - inBottom);
    transform[7] = (inTop + inBottom) / (inTop - inBottom);
    transform[10] = 2.0f / (inNear - inFar);
    transform[11] = (inFar + inNear) / (inFar - inNear);

    multiply(transform);
}

void Matrix3D::orthographic(float inRange, float inRatio)
{
    if (inRatio < 1.0f)
    {
        orthographic(-inRange, inRange, -inRange / inRatio, inRange / inRatio,
            -10.0f, 10.0f);
    }
    else
    {
        orthographic(-inRange * inRatio, inRange * inRatio, -inRange, inRange,
            -10.0f, 10.0f);
    }
}

Matrix3D& Matrix3D::operator=(const Matrix3D& inMatrix)
{
    if (this != &inMatrix) copy(inMatrix);
    return *this;
}

//Matrix3D& Matrix3D::operator*=(const Matrix3D& inMatrix)
void Matrix3D::multiply(const Matrix3D& inMatrix)
{
    Matrix3D result;

    // keep for reference
//    for (size_t i = 0; i < 4; ++i)
//    {
//        for (size_t j = 0; j < 4; ++j)
//        {
//            float value = 0.0f;
//            for (size_t k = 0; k < 4; ++k)
//                value += (*this)(i, k) * inMatrix(k, j);
//
//            result(i, j) = value;
//        }
//    }

    result[0] = (mData[0] * inMatrix[0]) + (mData[1] * inMatrix[4])
        + (mData[2] * inMatrix[8]) + (mData[3] * inMatrix[12]);
    result[1] = (mData[0] * inMatrix[1]) + (mData[1] * inMatrix[5])
        + (mData[2] * inMatrix[9]) + (mData[3] * inMatrix[13]);
    result[2] = (mData[0] * inMatrix[2]) + (mData[1] * inMatrix[6])
        + (mData[2] * inMatrix[10]) + (mData[3] * inMatrix[14]);
    result[3] = (mData[0] * inMatrix[3]) + (mData[1] * inMatrix[7])
        + (mData[2] * inMatrix[11]) + (mData[3] * inMatrix[15]);
    result[4] = (mData[4] * inMatrix[0]) + (mData[5] * inMatrix[4])
        + (mData[6] * inMatrix[8]) + (mData[7] * inMatrix[12]);
    result[5] = (mData[4] * inMatrix[1]) + (mData[5] * inMatrix[5])
        + (mData[6] * inMatrix[9]) + (mData[7] * inMatrix[13]);
    result[6] = (mData[4] * inMatrix[2]) + (mData[5] * inMatrix[6])
        + (mData[6] * inMatrix[10]) + (mData[7] * inMatrix[14]);
    result[7] = (mData[4] * inMatrix[3]) + (mData[5] * inMatrix[7])
        + (mData[6] * inMatrix[11]) + (mData[7] * inMatrix[15]);
    result[8] = (mData[8] * inMatrix[0]) + (mData[9] * inMatrix[4])
        + (mData[10] * inMatrix[8]) + (mData[11] * inMatrix[12]);
    result[9] = (mData[8] * inMatrix[1]) + (mData[9] * inMatrix[5])
        + (mData[10] * inMatrix[9]) + (mData[11] * inMatrix[13]);
    result[10] = (mData[8] * inMatrix[2]) + (mData[9] * inMatrix[6])
        + (mData[10] * inMatrix[10]) + (mData[11] * inMatrix[14]);
    result[11] = (mData[8] * inMatrix[3]) + (mData[9] * inMatrix[7])
        + (mData[10] * inMatrix[11]) + (mData[11] * inMatrix[15]);
    result[12] = (mData[12] * inMatrix[0]) + (mData[13] * inMatrix[4])
        + (mData[14] * inMatrix[8]) + (mData[15] * inMatrix[12]);
    result[13] = (mData[12] * inMatrix[1]) + (mData[13] * inMatrix[5])
        + (mData[14] * inMatrix[9]) + (mData[15] * inMatrix[13]);
    result[14] = (mData[12] * inMatrix[2]) + (mData[13] * inMatrix[6])
        + (mData[14] * inMatrix[10]) + (mData[15] * inMatrix[14]);
    result[15] = (mData[12] * inMatrix[3]) + (mData[13] * inMatrix[7])
        + (mData[14] * inMatrix[11]) + (mData[15] * inMatrix[15]);

/** The program below was used to generate the code above.

#include <iostream>
#include <fstream>
using namespace std;

size_t toIndex(size_t inRow, size_t inCol)
{
    return inRow * 4 + inCol;
}

int main(int argc, char** argv)
{
    ofstream fout;
    fout.open("code.txt");
    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            fout << "result[" << toIndex(i, j) << "] =";
            for (size_t k = 0; k < 4; ++k)
            {
                if (k > 0) fout << " +";
                fout << " (mData[" << toIndex(i, k) << "] * inMatrix["
                    << toIndex(k, j) << "])";
            }
            fout << ';' << endl;
        }
    }
    fout.close();
    return 0;
}

**/

    copy(result);
}

ostream& operator<<(ostream& inStream, const Matrix3D& inMatrix)
{
    inStream << setprecision(2);
    for (size_t i = 0; i < 16; ++i)
    {
        if (i > 0 && i % 4 == 0) inStream << '\n';
        inStream << setw(6) << inMatrix[i];
    }

    return inStream;
}
