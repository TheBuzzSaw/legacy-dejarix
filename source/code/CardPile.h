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

#ifndef CARDPILE_H
#define CARDPILE_H

#include <list>

#include "CardInstance.h"
#include "CardModel.h"

class CardPile
{
    public:
        CardPile();
        ~CardPile();

        inline size_t size() const { return mCards.size(); }
        inline bool isTemporary() { return !mGhost; }
        inline bool isEmpty() { return mCards.empty(); }
        inline void remove(CardInstance* inCard) { mCards.remove(inCard); }

        CardInstance* getFront();
        CardInstance* getBack();

    private:
        std::list<CardInstance*> mCards;
        GLuint mGhost;
        bool mCascade;
};

#endif
