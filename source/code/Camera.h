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

#ifndef CAMERA_H
#define CAMERA_H

#include "Matrix3D.h"
#include "Vector3D.h"
#include "Vector2D.h"

class Camera
{
    public:
        Camera();
        ~Camera();

        void update();
        void spin(float inOffset);
        void rise(float inOffset);
        void zoom(float inOffset);
        void move(Point inRelative);

        inline const Matrix3D& matrix()
        {
            return mMatrix;
        }

    private:
        Matrix3D mMatrix;
        Vector3D<float> mFocus;
        Vector3D<float> mTrackball;
};

#endif
