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

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "MathEngine.h"

#include <iostream>

template <class T>
class Vector2D
{
    public:
        Vector2D<T>();
        Vector2D<T>(const Vector2D<T>& inVector);
        Vector2D<T>(T inValue);
        Vector2D<T>(T inX, T inY);

        void normalize();
        void normalizeTo(T inValue);
        const Vector2D<T> normalized() const;
        void negate();

        Vector2D<T>& operator=(const Vector2D<T>& inVector);
        Vector2D<T>& operator+=(const Vector2D<T>& inVector);
        Vector2D<T>& operator-=(const Vector2D<T>& inVector);

        const Vector2D<T> operator-() const;

        inline const Vector2D<T> operator+(const Vector2D<T>& inVector) const
        {
            return Vector2D<T>(*this) += inVector;
        }

        inline const Vector2D<T> operator-(const Vector2D<T>& inVector) const
        {
            return Vector2D<T>(*this) -= inVector;
        }

        inline T& operator[](size_t inIndex) { return mVector[inIndex]; }
        inline T operator[](size_t inIndex) const { return mVector[inIndex]; }
        inline T* array() { return mVector; }
        inline T length() const
        {
            return sqrt((mVector[0] * mVector[0]) + (mVector[1] * mVector[1]));
        }

    private:
        T mVector[2];
};

typedef Vector2D<float> Point;
typedef Vector2D<int> Pixel;

template<class T>
Vector2D<T>::Vector2D()
{
    mVector[0] = static_cast<T>(0);
    mVector[1] = static_cast<T>(0);
}

template<class T>
Vector2D<T>::Vector2D(const Vector2D<T>& inVector)
{
    mVector[0] = inVector[0];
    mVector[1] = inVector[1];
}

template<class T>
Vector2D<T>::Vector2D(T inValue)
{
    mVector[0] = inValue;
    mVector[1] = inValue;
}

template<class T>
Vector2D<T>::Vector2D(T inX, T inY)
{
    mVector[0] = inX;
    mVector[1] = inY;
}

template<class T>
void Vector2D<T>::normalize()
{
    T l = length();
    if (fabs(l) < PSEUDO_ZERO) return;
    mVector[0] /= l;
    mVector[1] /= l;
}

template<class T>
void Vector2D<T>::normalizeTo(T inLength)
{
    T l = length();
    if (fabs(l) < PSEUDO_ZERO) return;

    l = inLength / l;
    mVector[0] *= l;
    mVector[1] *= l;
}

template<class T>
const Vector2D<T> Vector2D<T>::normalized() const
{
    Vector2D<T> outVector(*this);
    outVector.normalize();
    return outVector;
}

template<class T>
void Vector2D<T>::negate()
{
    mVector[0] = -mVector[0];
    mVector[1] = -mVector[1];
}

template<class T>
Vector2D<T>& Vector2D<T>::operator=(const Vector2D<T>& inVector)
{
    mVector[0] = inVector[0];
    mVector[1] = inVector[1];
    return *this;
}

template<class T>
Vector2D<T>& Vector2D<T>::operator+=(const Vector2D<T>& inVector)
{
    mVector[0] += inVector[0];
    mVector[1] += inVector[1];
    return *this;
}

template<class T>
Vector2D<T>& Vector2D<T>::operator-=(const Vector2D<T>& inVector)
{
    mVector[0] -= inVector[0];
    mVector[1] -= inVector[1];
    return *this;
}

template<class T>
const Vector2D<T> Vector2D<T>::operator-() const
{
    Vector2D<T> outVector(*this);
    outVector.negate();
    return outVector;
}

template<class T>
std::istream& operator>>(std::istream& inStream, Vector2D<T>& inVector)
{
    inStream >> inVector[0] >> inVector[1];
    return inStream;
}

template<class T>
std::ostream& operator<<(std::ostream& inStream, const Vector2D<T>& inVector)
{
    inStream << inVector[0] << ' ' << inVector[1];
    return inStream;
}

#endif

