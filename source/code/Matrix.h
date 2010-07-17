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

#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <iomanip>

template<class T>
class Matrix
{
    public:
        Matrix();
        Matrix(const Matrix<T>& inMatrix);
        Matrix(size_t inRows, size_t inCols);
        Matrix(size_t inEdge);
        ~Matrix();

        void transpose();
        T determinant() const;
        Matrix<T> minorMatrix(size_t inRow, size_t inCol) const;
        Matrix<T> subMatrix(size_t inRow, size_t inCol, size_t inHeight,
            size_t inWidth);

        Matrix<T>& operator=(const Matrix<T>& inMatrix);
        Matrix<T>& operator+=(const Matrix<T>& inMatrix);
        Matrix<T>& operator-=(const Matrix<T>& inMatrix);
        Matrix<T>& operator*=(const Matrix<T>& inMatrix);
        Matrix<T>& operator+=(const T inValue);
        Matrix<T>& operator-=(const T inValue);
        Matrix<T>& operator*=(const T inValue);
        Matrix<T>& operator/=(const T inValue);

        inline size_t rows() const { return mRows; }
        inline size_t cols() const { return mCols; }
        inline size_t size() const { return mSize; }
        inline size_t lastRow() const { return mRows - 1; }
        inline size_t lastCol() const { return mCols - 1; }
        inline bool square() const { return mRows == mCols; }

        inline size_t toIndex(size_t inRow, size_t inCol) const
        {
            return (inRow * mCols) + inCol;
        }

        inline void set(size_t inRow, size_t inCol, T inValue)
        {
            mMatrix[(inRow * mCols) + inCol] = inValue;
        }

        inline const Matrix<T> transposed()
        {
            return Matrix<T>(*this).transpose();
        }

        inline T& operator()(size_t inRow, size_t inCol)
        {
            return mMatrix[(inRow * mCols) + inCol];
        }

        inline T operator()(size_t inRow, size_t inCol) const
        {
            return mMatrix[(inRow * mCols) + inCol];
        }

        inline T& operator[](size_t inIndex)
        {
            return mMatrix[inIndex];
        }

        inline T operator[](size_t inIndex) const
        {
            return mMatrix[inIndex];
        }

        inline bool operator==(const Matrix<T>& inMatrix) const
        {
            return mRows == inMatrix.mRows && mCols == inMatrix.mCols
                && !memcmp(mMatrix, inMatrix.mMatrix, mSize * sizeof(T));
        }

        inline bool operator!=(const Matrix<T>& inMatrix) const
        {
            return mRows != inMatrix.mRows || mCols != inMatrix.mCols
                || memcmp(mMatrix, inMatrix.mMatrix, mSize * sizeof(T));
        }

        inline const Matrix<T> operator+(const Matrix<T>& inMatrix) const
        {
            return Matrix<T>(*this) += inMatrix;
        }

        inline const Matrix<T> operator-(const Matrix<T>& inMatrix) const
        {
            return Matrix<T>(*this) -= inMatrix;
        }

        inline const Matrix<T> operator*(const Matrix<T>& inMatrix) const
        {
            return Matrix<T>(*this) *= inMatrix;
        }

        inline const Matrix<T> operator+(const T inValue) const
        {
            return Matrix<T>(*this) += inValue;
        }

        inline const Matrix<T> operator-(const T inValue) const
        {
            return Matrix<T>(*this) -= inValue;
        }

        inline const Matrix<T> operator*(const T inValue) const
        {
            return Matrix<T>(*this) *= inValue;
        }

        inline const Matrix<T> operator/(const T inValue) const
        {
            return Matrix<T>(*this) /= inValue;
        }

        inline T* array() { return mMatrix; }

    private:
        void copy(const Matrix<T>& inMatrix);

        inline T at(int inRow, int inCol) const
        {
            return mMatrix[(inRow * mCols) + inCol];
        }

        size_t mRows;
        size_t mCols;
        size_t mSize;
        T* mMatrix;
};

template<class T>
Matrix<T>::Matrix()
{
    // produces a square matrix
    mRows = 4;
    mCols = 4;
    mSize = mRows * mCols;
    mMatrix = new T[mSize];

    // default to identity matrix
    for (size_t i = 0; i < mSize; ++i)
        mMatrix[i] = (i % (mCols + 1) == 0 ? 1 : 0);
}

template<class T>
Matrix<T>::Matrix(size_t inRows, size_t inCols)
{
    mRows = inRows > 0 ? inRows : 1;
    mCols = inCols > 0 ? inCols : 1;
    mSize = mRows * mCols;
    mMatrix = new T[mSize];

    memset(mMatrix, 0, mSize * sizeof(T));
}

template<class T>
Matrix<T>::Matrix(size_t inEdge)
{
    // produces a square matrix
    mRows = inEdge > 0 ? inEdge : 1;
    mCols = mRows;
    mSize = mRows * mCols;
    mMatrix = new T[mSize];

    // default to identity matrix
    for (size_t i = 0; i < mSize; ++i)
        mMatrix[i] = (i % (mCols + 1) == 0 ? 1 : 0);
}

template<class T>
Matrix<T>::Matrix(const Matrix<T>& inMatrix)
{
    copy(inMatrix);
}

template<class T>
Matrix<T>::~Matrix()
{
    delete [] mMatrix;
}

template<class T>
void Matrix<T>::copy(const Matrix<T>& inMatrix)
{
    mRows = inMatrix.mRows;
    mCols = inMatrix.mCols;
    mSize = inMatrix.mSize;
    mMatrix = new T[mSize];

    memcpy(mMatrix, inMatrix.mMatrix, mSize * sizeof(T));
}

template<class T>
T Matrix<T>::determinant() const
{
    if (mRows != mCols)
    {
        return 0;
    }
    else if (mRows == 1)
    {
        return mMatrix[0];
    }
    else if (mRows == 2)
    {
        return (mMatrix[0] * mMatrix[3]) - (mMatrix[1] * mMatrix[2]);
    }

    T outValue = 0;

    for (size_t i = 0; i < mRows; ++i)
    {
        Matrix m = minorMatrix(0, i);
        int neg = (i % 2 == 0) ? 1 : -1;
        outValue += neg * at(0, i) * m.determinant();
    }

    return outValue;
}

template<class T>
Matrix<T> Matrix<T>::subMatrix(size_t inRow, size_t inCol, size_t inHeight,
    size_t inWidth)
{
    size_t bottom = inRow + inHeight - 1;
    size_t right = inCol + inWidth - 1;

    if (inHeight < 1 || inWidth < 1 || inRow < 0 || bottom >= mRows || inCol < 0
        || right >= mCols)
        return Matrix(1, 1);

    Matrix<T> outMatrix(inHeight, inWidth);
    for (size_t i = 0; i < inHeight; ++i)
    {
        for (size_t j = 0; j < inWidth; ++j)
        {
            outMatrix(i, j) = at(i + inRow, j + inCol);
        }
    }

    return outMatrix;
}

template<class T>
Matrix<T> Matrix<T>::minorMatrix(size_t inRow, size_t inCol) const
{
    if (mRows != mCols || inRow >= mRows || inCol >= mCols)
        return Matrix(1, 1);

    size_t p = 0;

    Matrix outMatrix(mRows - 1);

    for (size_t i = 0; i < mRows; ++i)
    {
        if (i != inRow)
        {
            size_t q = 0;
            for (size_t j = 0; j < mCols; ++j)
            {
                if (j != inCol)
                {
                    outMatrix(p, q) = at(i, j);
                    ++q;
                }
            }
            ++p;
        }
    }

    return outMatrix;
}

template<class T>
void Matrix<T>::transpose()
{
    if (mRows < 2 || mRows != mCols) return;
    for (size_t i = 0; i < mRows; ++i)
    {
        for (size_t j = i + 1; j < mCols; ++j)
        {
            T t = at(i, j);
            (*this)(i, j) = at(j, i);
            (*this)(j, i) = t;
        }
    }
}

template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& inMatrix)
{
    if (this != &inMatrix)
    {
        delete [] mMatrix;
        copy(inMatrix);
    }
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& inMatrix)
{
    if (mRows == inMatrix.mRows && mCols == inMatrix.mCols)
    {
        for (size_t i = 0; i < mSize; ++i) mMatrix[i] += inMatrix.mMatrix[i];
    }
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator+=(const T inValue)
{
    for (size_t i = 0; i < mSize; ++i) mMatrix[i] += inValue;
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& inMatrix)
{
    if (mRows == inMatrix.mRows && mCols == inMatrix.mCols)
    {
        for (size_t i = 0; i < mSize; ++i) mMatrix[i] -= inMatrix.mMatrix[i];
    }
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator-=(const T inValue)
{
    for (size_t i = 0; i < mSize; ++i) mMatrix[i] -= inValue;
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& inMatrix)
{
    if (mCols != inMatrix.mRows) return *this;
    Matrix<T> result = *this * inMatrix;
    delete [] mMatrix;
    copy(result);
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator*=(const T inValue)
{
    for (size_t i = 0; i < mSize; ++i) mMatrix[i] *= inValue;
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator/=(const T inValue)
{
    for (size_t i = 0; i < mSize; ++i) mMatrix[i] /= inValue;
    return *this;
}

template<class T>
std::ostream& operator<<(std::ostream& inStream, const Matrix<T>& inMatrix)
{
    inStream << std::setprecision(3);
    for (size_t i = 0; i < inMatrix.size(); ++i)
    {
        if (i % inMatrix.cols() == 0 && i > 0) inStream << std::endl;
        inStream << std::setw(8) << inMatrix[i];
    }

    inStream << std::endl;

    return inStream;
}

template<class T>
std::istream& operator>>(std::istream& inStream, Matrix<T>& inMatrix)
{
    size_t rows;
    size_t cols;
    inStream >> rows >> cols;
    inMatrix = Matrix<T>(rows, cols);

    for (size_t i = 0; i < inMatrix.size(); ++i) inStream >> inMatrix[i];

    return inStream;
}

#endif
