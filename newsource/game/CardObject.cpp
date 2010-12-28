#include "CardObject.h"

GLint CardObject::mMatrixUniform;

CardObject::CardObject() : mRotate(0.0f)
{
    for (size_t i = 0; i < 3; ++i) mPosition[i] = 0.0f;
}

CardObject::~CardObject()
{
}

void CardObject::update(const mat4f& inMatrix)
{
    mMatrix = inMatrix;
    mMatrix.translate(mPosition[0], mPosition[1], mPosition[2]);
    mMatrix.rotateZ(mRotate);
}

void CardObject::sendMatrix()
{
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix);
}
