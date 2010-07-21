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

#include "CardInstance.h"

CardInstance::CardInstance(GLuint inFront, GLuint inBack) : mFront(inFront),
    mBack(inBack)
{
}

CardInstance::~CardInstance()
{
}

void CardInstance::update()
{
    mMatrix.loadIdentity();
    mMatrix.translate(mTranslate[0], mTranslate[1], mTranslate[2]);
    mMatrix.rotateZ(mRotate[1]);
    mMatrix.rotateY(mRotate[0]);
}

void CardInstance::setPosition(const Vector3D<float>& inVector)
{
    mTranslate = inVector;
    update();
}

bool CardInstance::isInside(const Vector3D<float>& inVector)
{
    float w = 6.3f / 2.0f;
    float h = 8.8f / 2.0f;
    return (inVector[0] >= mTranslate[0] - w
        && inVector[0] <= mTranslate[0] + w
        && inVector[1] >= mTranslate[1] - h
        && inVector[1] <= mTranslate[1] + h);
}
