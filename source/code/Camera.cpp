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

Camera::Camera() : mZoom(1.0f)
{
}

Camera::~Camera()
{
}

void Camera::update()
{
    mMatrix.loadIdentity();
    mMatrix.scale(mZoom);
    mMatrix.translate(-mFocus[0], -mFocus[1], 0.0f);
}

void Camera::zoom(float inOffset)
{
    mZoom += inOffset;
    if (mZoom < 0.0f) mZoom = 0.0f;
    glPointSize(mZoom);
}
