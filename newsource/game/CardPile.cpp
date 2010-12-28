#include "CardPile.h"

CardPile::CardPile() : mTransparency(1.0f), mThickness(1.0f), mFlip(0.0f)
{
}

CardPile::~CardPile()
{
}

void CardPile::display()
{
}

void CardPile::update(const mat4f& inMatrix)
{
    mMatrix = inMatrix;
    mMatrix.translate(mPosition[0], mPosition[1], mPosition[2]);
    mMatrix.rotateZ(mRotate);
    mMatrix.rotateY(mFlip);
    mMatrix.scaleZ(mThickness);
}
