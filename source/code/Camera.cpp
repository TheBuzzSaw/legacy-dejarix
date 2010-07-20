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

#include <cmath>

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
    mMatrix.rotateZ(mTrackball[1]);
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

void Camera::move(Point inRelative)
{
    for (size_t i = 0; i < 2; ++i)
    {
        bool neg = inRelative[i] < 0.0f;
        inRelative[i] = fabs(inRelative[i]);

        if (inRelative[i] < 0.5f)
        {
            inRelative[i] = 0.0f;
            continue;
        }
        else
        {
            inRelative[i] -= 0.5f;
            inRelative[i] += 1.0f;
            inRelative[i] = pow(inRelative[i], 3.0f);
            inRelative[i] -= 1.0f;
        }

        if (neg) inRelative[i] = -inRelative[i];
        inRelative[i] *= 0.015f * mTrackball[2];
    }

    float r = TO_RADIANS(mTrackball[1]);
    float s = sin(r);
    float c = cos(r);

    mFocus[0] += c * inRelative[0] + s * inRelative[1];
    mFocus[1] += -s * inRelative[0] + c * inRelative[1];
}
