#include "CardLayout.h"

#include <cassert>

CardLayout::CardLayout()
{
}

CardLayout::~CardLayout()
{
}

void CardLayout::add(CardObject* inObject)
{
    assert(inObject);
    mObjects.push_back(inObject);
}

void CardLayout::remove(CardObject* inObject)
{
    assert(inObject);
    mObjects.remove(inObject);
}

void CardLayout::display()
{
    for (std::list<CardObject*>::iterator i = mObjects.begin();
        i != mObjects.end(); ++i)
    {
        (*i)->display();
    }
}

void CardLayout::update(const mat4f& inMatrix)
{
    CardObject::update(inMatrix);

    for (std::list<CardObject*>::iterator i = mObjects.begin();
        i != mObjects.end(); ++i)
    {
        (*i)->update(mMatrix);
    }
}
