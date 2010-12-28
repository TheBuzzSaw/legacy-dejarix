#ifndef CARDLAYOUT_H
#define CARDLAYOUT_H

#include "CardObject.h"

#include <list>

class CardLayout : public CardObject
{
    public:
        CardLayout();
        virtual ~CardLayout();

        void add(CardObject* inObject);
        void remove(CardObject* inObject);
        virtual void display();
        virtual void update(const mat4f& inMatrix);

    private:
        std::list<CardObject*> mObjects;
};

#endif
