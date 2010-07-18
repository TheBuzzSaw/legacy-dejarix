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

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "MathEngine.h"

#include <cmath>
#include <cstring>
#include <iostream>

template<class T>
class Vector3D
{
    public:
        Vector3D<T>();
        Vector3D<T>(const Vector3D<T>& inVector);
        Vector3D<T>(T inValue);
        Vector3D<T>(T inX, T inY, T inZ);
        Vector3D<T>(T inX, T inY, T inZ, T inW);
        void set(T inValue);
        void set(T inX, T inY, T inZ);
        void normalizeTo(T inLength);
        void normalize();
        const Vector3D<T> normalized() const;
        void negate();

        Vector3D<T>& operator=(T inValue);
        Vector3D<T>& operator=(const Vector3D<T>& inVector);
        Vector3D<T>& operator+=(const Vector3D<T>& inVector);
        Vector3D<T>& operator-=(const Vector3D<T>& inVector);
        Vector3D<T>& operator*=(T inValue);

        const Vector3D<T> operator-() const;
        T operator*(const Vector3D<T>& inVector) const;
        const Vector3D<T> operator*(const T inRight) const;
        const Vector3D<T> operator^(const Vector3D<T>& inVector) const;

        inline const Vector3D<T> operator+(const Vector3D<T>& inVector) const
        {
            return Vector3D<T>(*this) += inVector;
        }

        inline const Vector3D<T> operator-(const Vector3D<T>& inVector) const
        {
            return Vector3D<T>(*this) -= inVector;
        }

        inline bool isZero() const
        {
            return fabs(mVector[0]) < PSEUDO_ZERO
                && fabs(mVector[1]) < PSEUDO_ZERO
                && fabs(mVector[2]) < PSEUDO_ZERO;
        }

        inline T& operator[](size_t inIndex)
        {
            return mVector[inIndex];
        }

        inline T operator[](size_t inIndex) const
        {
            return mVector[inIndex];
        }

        inline T* array()
        {
            return mVector;
        }

        inline const T* array() const
        {
            return mVector;
        }

        inline T length() const
        {
            return sqrt((mVector[0] * mVector[0]) + (mVector[1] * mVector[1])
                + (mVector[2] * mVector[2]));
        }

    private:
        T mVector[4];
};

template<class T>
Vector3D<T>::Vector3D()
{
    mVector[0] = static_cast<T>(0);
    mVector[1] = static_cast<T>(0);
    mVector[2] = static_cast<T>(0);
    mVector[3] = static_cast<T>(1);
}

template<class T>
Vector3D<T>::Vector3D(const Vector3D<T>& inVector)
{
    memcpy(mVector, inVector.mVector, 4 * sizeof(T));
}

template<class T>
Vector3D<T>::Vector3D(T inValue)
{
    mVector[0] = inValue;
    mVector[1] = inValue;
    mVector[2] = inValue;
    mVector[3] = static_cast<T>(1);
}

template<class T>
Vector3D<T>::Vector3D(T inX, T inY, T inZ)
{
    mVector[0] = inX;
    mVector[1] = inY;
    mVector[2] = inZ;
    mVector[3] = static_cast<T>(1);
}

template<class T>
Vector3D<T>::Vector3D(T inX, T inY, T inZ, T inW)
{
    mVector[0] = inX;
    mVector[1] = inY;
    mVector[2] = inZ;
    mVector[3] = inW;
}

template<class T>
void Vector3D<T>::set(T inValue)
{
    mVector[0] = inValue;
    mVector[1] = inValue;
    mVector[2] = inValue;
}

template<class T>
void Vector3D<T>::set(T inX, T inY, T inZ)
{
    mVector[0] = inX;
    mVector[1] = inY;
    mVector[2] = inZ;
}

template<class T>
void Vector3D<T>::normalizeTo(T inLength)
{
    T l = length();
    if (fabs(l) < PSEUDO_ZERO) return;

    l = inLength / l;
    mVector[0] *= l;
    mVector[1] *= l;
    mVector[2] *= l;
}

template<class T>
void Vector3D<T>::normalize()
{
    T l = length();
    if (fabs(l) < PSEUDO_ZERO) return;
    mVector[0] /= l;
    mVector[1] /= l;
    mVector[2] /= l;
}

template<class T>
const Vector3D<T> Vector3D<T>::normalized() const
{
    Vector3D<T> outVector(*this);
    outVector.normalize();
    return outVector;
}

template<class T>
void Vector3D<T>::negate()
{
    mVector[0] = -mVector[0];
    mVector[1] = -mVector[1];
    mVector[2] = -mVector[2];
}

template<class T>
Vector3D<T>& Vector3D<T>::operator=(T inValue)
{
    mVector[0] = inValue;
    mVector[1] = inValue;
    mVector[2] = inValue;
    return *this;
}

template<class T>
Vector3D<T>& Vector3D<T>::operator=(const Vector3D<T>& inVector)
{
    memcpy(mVector, inVector.mVector, 4 * sizeof(T));
    return *this;
}

template<class T>
Vector3D<T>& Vector3D<T>::operator+=(const Vector3D<T>& inVector)
{
    mVector[0] += inVector.mVector[0];
    mVector[1] += inVector.mVector[1];
    mVector[2] += inVector.mVector[2];
    return *this;
}

template<class T>
Vector3D<T>& Vector3D<T>::operator-=(const Vector3D<T>& inVector)
{
    mVector[0] -= inVector.mVector[0];
    mVector[1] -= inVector.mVector[1];
    mVector[2] -= inVector.mVector[2];
    return *this;
}

template<class T>
Vector3D<T>& Vector3D<T>::operator*=(T inValue)
{
    mVector[0] *= inValue;
    mVector[1] *= inValue;
    mVector[2] *= inValue;
    return *this;
}

template<class T>
const Vector3D<T> Vector3D<T>::operator-() const
{
    Vector3D<T> outVector(*this);
    outVector.negate();
    return outVector;
}

template<class T>
T Vector3D<T>::operator*(const Vector3D<T>& inVector) const
{
    return (mVector[0] * inVector.mVector[0])
        + (mVector[1] * inVector.mVector[1])
        + (mVector[2] * inVector.mVector[2]);
}

template<class T>
const Vector3D<T> Vector3D<T>::operator*(const T inRight) const
{
    Vector3D<T> outVector;
    outVector.mVector[0] = mVector[0] * inRight;
    outVector.mVector[1] = mVector[1] * inRight;
    outVector.mVector[2] = mVector[2] * inRight;
    return outVector;
}

template<class T>
const Vector3D<T> Vector3D<T>::operator^(const Vector3D<T>& inVector) const
{
    Vector3D<T> outVector;
    outVector[0] = (mVector[1] * inVector.mVector[2]) -
        (mVector[2] * inVector.mVector[1]);
    outVector[1] = (mVector[2] * inVector.mVector[0]) -
        (mVector[0] * inVector.mVector[2]);
    outVector[2] = (mVector[0] * inVector.mVector[1]) -
        (mVector[1] * inVector.mVector[0]);
    return outVector;
}

template<class T>
std::istream& operator>>(std::istream& inStream, Vector3D<T>& inVector)
{
    inStream >> inVector[0] >> inVector[1] >> inVector[2] >> inVector[3];
    return inStream;
}

template<class T>
std::ostream& operator<<(std::ostream& inStream, const Vector3D<T>& inVector)
{
    inStream << inVector[0] << ' ' << inVector[1] << ' ' << inVector[2]
         << ' ' << inVector[3];
    return inStream;
}

#endif
