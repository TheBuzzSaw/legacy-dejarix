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

#ifndef MATRIX3D_H
#define MATRIX3D_H

#include "MathEngine.h"

#include <cmath>
#include <cstring>
#include <iostream>

class Matrix3D
{
    public:
        Matrix3D();
        Matrix3D(const Matrix3D& inMatrix);

        void loadIdentity();
        void rotate(float inDegrees, float inX, float inY, float inZ);
        void rotateX(float inDegrees);
        void rotateY(float inDegrees);
        void rotateZ(float inDegrees);
        void scale(float inScale);
        void scale(float inX, float inY, float inZ);
        void translate(float inX, float inY, float inZ);
        void frustrum(float inLeft, float inRight, float inBottom, float inTop,
            float inNear, float inFar);
        void perspective(float inFieldOfView, float inRatio, float inNear,
            float inFar);
        void orthographic(float inLeft, float inRight, float inBottom,
            float inTop, float inNear, float inFar);
        void orthographic(float inRange, float inRatio);

        /// matrix operators
        Matrix3D& operator=(const Matrix3D& inMatrix);
        void multiply(const Matrix3D& inMatrix);

        inline float* array() { return mData; }
        inline const float* array() const { return mData; }
        inline float& operator[](size_t inIndex) { return mData[inIndex]; }
        inline float operator[](size_t inIndex) const { return mData[inIndex]; }

        inline float& operator()(size_t inRow, size_t inCol)
        {
            return mData[inCol * 4 + inRow];
        }

        inline float operator()(size_t inRow, size_t inCol) const
        {
            return mData[inCol * 4 + inRow];
        }

        inline bool operator==(const Matrix3D& inMatrix) const
        {
            return !memcmp(mData, inMatrix.mData, 16 * sizeof(float));
        }

        inline bool operator!=(const Matrix3D& inMatrix) const
        {
            return memcmp(mData, inMatrix.mData, 16 * sizeof(float));
        }

        inline Matrix3D& operator*=(const Matrix3D& inMatrix)
        {
            multiply(inMatrix);
            return *this;
        }

        inline const Matrix3D operator*(const Matrix3D& inMatrix) const
        {
            return Matrix3D(*this) *= inMatrix;
        }

    private:
        inline void copy(const Matrix3D& inMatrix)
        {
            memcpy(mData, inMatrix.mData, 16 * sizeof(float));
        }

        float mData[16];

        static const float mIdentity[16];
};

std::ostream& operator<<(std::ostream& inStream, const Matrix3D& inMatrix);

#endif

