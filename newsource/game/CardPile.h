#ifndef CARDPILE_H
#define CARDPILE_H

#include <CGE/ClusterVBO.h>

#include "CardObject.h"

class CardPile : public CardObject
{
    public:
        CardPile();
        virtual ~CardPile();

        virtual void display();
        virtual void update(const mat4f& inMatrix);

    private:
        float mTransparency;
        float mThickness;
        float mFlip; // Y-axis rotation (face up or face down)
};

#endif
