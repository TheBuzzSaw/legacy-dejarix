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

#ifndef _MODULE_H_
#define _MODULE_H_

#include <SDL.h>
#include <string>

class Module
{
    public:

        class Exception
        {
            public:
                Exception(const std::string& inReason) : reason(inReason) {}
                const std::string reason;
        };

        Module();
        virtual ~Module();

        void onEvent(SDL_Event* inEvent);
        inline bool isRunning() { return mRunning; };
        inline bool isDead() { return mDead; }
        inline Module* next() { return mNextModule; }

        /// module operation
        virtual void onLoad();
        virtual void onOpen();
        virtual void onLoop();
        virtual void onFrame();
        virtual void onClose();
        virtual void onUnload();

    protected:
        bool mRunning;
        bool mDead;
        Module* mNextModule;

        /// input events
        virtual void onInputFocus();
        virtual void onInputBlur();
        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onMouseFocus();
        virtual void onMouseBlur();
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);
        virtual void onMouseWheel(bool inUp, bool inDown);
        virtual void onLButtonDown(int inX, int inY);
        virtual void onLButtonUp(int inX, int inY);
        virtual void onRButtonDown(int inX, int inY);
        virtual void onRButtonUp(int inX, int inY);
        virtual void onMButtonDown(int inX, int inY);
        virtual void onMButtonUp(int inX, int inY);
        virtual void onJoyAxis(Uint8 inWhich, Uint8 inAxis, Sint16 inValue);
        virtual void onJoyButtonDown(Uint8 inWhich, Uint8 inButton);
        virtual void onJoyButtonUp(Uint8 inWhich, Uint8 inButton);
        virtual void onJoyHat(Uint8 inWhich, Uint8 inHat, Uint8 inValue);
        virtual void onJoyBall(Uint8 inWhich, Uint8 inBall, Sint16 inXRel,
            Sint16 inYRel);
        virtual void onMinimize();
        virtual void onRestore();
        virtual void onResize(int inWidth, int inHeight);
        virtual void onExpose();
        virtual void onExit();
        virtual void onUser(Uint8 inType, int inCode, void* inData1,
            void* inData2);
};

#endif

