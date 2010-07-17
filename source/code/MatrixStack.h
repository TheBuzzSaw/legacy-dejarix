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

#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include "Matrix3D.h"

class MatrixStack
{
    public:
        MatrixStack(size_t inCapacity = 16);
        ~MatrixStack();

        inline Matrix3D& matrix() { return mMatrices[mSize - 1]; }
        void push();
        void pop();
        void reset();

    private:
        Matrix3D* mMatrices;
        size_t mCapacity;
        size_t mSize;
};

#endif
