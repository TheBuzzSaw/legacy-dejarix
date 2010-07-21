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

#ifndef CARDINSTANCE_H
#define CARDINSTANCE_H

#include "Matrix3D.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "CardModel.h"
#include "OGL.h"

class CardInstance
{
    public:
        CardInstance(GLuint inFront, GLuint inBack);
        ~CardInstance();

        inline const Matrix3D& matrix() { return mMatrix; }
        inline const Vector3D<float>& getPosition() { return mTranslate; }
        inline GLuint front() { return mFront; }
        inline GLuint back() { return mBack; }

        void setPosition(const Vector3D<float>& inVector);
        bool isInside(const Vector3D<float>& inVector);

    private:
        void update();

        Matrix3D mMatrix;
        Vector3D<float> mTranslate;
        Point mRotate; // 0 -- flip, 1 -- rotate
        GLuint mFront;
        GLuint mBack;
};

#endif
