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

#include "Camera.h"
#include "OGL.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::update()
{
    mMatrix.loadIdentity();
    mMatrix.translate(0.0f, 0.0f, -mTrackball[2]);
    mMatrix.rotateX(mTrackball[0]);
    mMatrix.rotateY(mTrackball[1]);
    mMatrix.translate(-mFocus[0], -mFocus[1], -mFocus[2]);
}

void Camera::spin(float inOffset)
{
    mTrackball[1] += inOffset;
    if (mTrackball[1] > 180.0f)
        mTrackball[1] -= 360.0f;
    else if (mTrackball[1] < -180.0f)
        mTrackball[1] += 360.0f;
}

void Camera::rise(float inOffset)
{
    mTrackball[0] += inOffset;
    if (mTrackball[0] > 0.0f)
        mTrackball[0] = 0.0f;
    else if (mTrackball[0] < -90.0f)
        mTrackball[0] = -90.0f;
}

void Camera::zoom(float inOffset)
{
    mTrackball[2] += inOffset;
    if (mTrackball[2] < 0.0f) mTrackball[2] = 0.0f;
}
