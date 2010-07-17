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

#include "Module.h"

#include <iostream>
using namespace std;

Module::Module() : mRunning(true), mDead(true), mNextModule(NULL)
{
}

Module::~Module()
{
}

void Module::onLoad()
{
}

void Module::onUnload()
{
}

void Module::onOpen()
{
}

void Module::onClose()
{
}

void Module::onLoop()
{
}

void Module::onFrame()
{
}

/// event handlers
void Module::onEvent(SDL_Event* inEvent)
{
    switch (inEvent->type)
    {
        case SDL_ACTIVEEVENT:
        {
            switch (inEvent->active.state)
            {
                case SDL_APPMOUSEFOCUS:
                {
                    if (inEvent->active.gain) onMouseFocus();
                    else onMouseBlur();

                    break;
                }
                case SDL_APPINPUTFOCUS:
                {
                    if (inEvent->active.gain) onInputFocus();
                    else onInputBlur();

                    break;
                }
                case SDL_APPACTIVE:
                {
                    if (inEvent->active.gain) onRestore();
                    else onMinimize();

                    break;
                }
            }
            break;
        }

        case SDL_KEYDOWN:
        {
            onKeyDown(inEvent->key.keysym.sym, inEvent->key.keysym.mod,
                inEvent->key.keysym.unicode);
            break;
        }

        case SDL_KEYUP:
        {
            onKeyUp(inEvent->key.keysym.sym, inEvent->key.keysym.mod,
                inEvent->key.keysym.unicode);
            break;
        }

        case SDL_MOUSEMOTION:
        {
            onMouseMove(inEvent->motion.x, inEvent->motion.y,
                inEvent->motion.xrel, inEvent->motion.yrel,
                (inEvent->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0,
                (inEvent->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0,
                (inEvent->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        {
            switch (inEvent->button.button)
            {
                case SDL_BUTTON_LEFT:
                {
                    onLButtonDown(inEvent->button.x, inEvent->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT:
                {
                    onRButtonDown(inEvent->button.x, inEvent->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE:
                {
                    onMButtonDown(inEvent->button.x, inEvent->button.y);
                    break;
                }
            }
            break;
        }

        case SDL_MOUSEBUTTONUP:
        {
            switch (inEvent->button.button)
            {
                case SDL_BUTTON_LEFT:
                {
                    onLButtonUp(inEvent->button.x, inEvent->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT:
                {
                    onRButtonUp(inEvent->button.x, inEvent->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE:
                {
                    onMButtonUp(inEvent->button.x, inEvent->button.y);
                    break;
                }
                case SDL_BUTTON_WHEELUP:
                {
                    onMouseWheel(true, false);
                    break;
                }
                case SDL_BUTTON_WHEELDOWN:
                {
                    onMouseWheel(false, true);
                    break;
                }
            }
            break;
        }

        case SDL_JOYAXISMOTION:
        {
            onJoyAxis(inEvent->jaxis.which, inEvent->jaxis.axis,
                inEvent->jaxis.value);
            break;
        }

        case SDL_JOYBALLMOTION:
        {
            onJoyBall(inEvent->jball.which, inEvent->jball.ball,
                inEvent->jball.xrel, inEvent->jball.yrel);
            break;
        }

        case SDL_JOYHATMOTION:
        {
            onJoyHat(inEvent->jhat.which, inEvent->jhat.hat,
                inEvent->jhat.value);
            break;
        }
        case SDL_JOYBUTTONDOWN:
        {
            onJoyButtonDown(inEvent->jbutton.which, inEvent->jbutton.button);
            break;
        }

        case SDL_JOYBUTTONUP:
        {
            onJoyButtonUp(inEvent->jbutton.which, inEvent->jbutton.button);
            break;
        }

        case SDL_QUIT:
        {
            onExit();
            break;
        }

        case SDL_SYSWMEVENT:
        {
            //Ignore
            break;
        }

        case SDL_VIDEORESIZE:
        {
            onResize(inEvent->resize.w, inEvent->resize.h);
            break;
        }

        case SDL_VIDEOEXPOSE:
        {
            onExpose();
            break;
        }

        default:
        {
            onUser(inEvent->user.type, inEvent->user.code, inEvent->user.data1,
                inEvent->user.data2);
            break;
        }
    }
}

void Module::onInputFocus()
{
}

void Module::onInputBlur()
{
}

void Module::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    if (inSym == SDLK_ESCAPE) mRunning = false;
}

void Module::onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
}

void Module::onMouseFocus()
{
}

void Module::onMouseBlur()
{
}


void Module::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
}

void Module::onMouseWheel(bool inUp, bool inDown)
{
}

void Module::onLButtonDown(int inX, int inY)
{
}

void Module::onLButtonUp(int inX, int inY)
{
}

void Module::onRButtonDown(int inX, int inY)
{
}

void Module::onRButtonUp(int inX, int inY)
{
}

void Module::onMButtonDown(int inX, int inY)
{
}

void Module::onMButtonUp(int inX, int inY)
{
}

void Module::onJoyAxis(Uint8 inWhich, Uint8 inAxis, Sint16 inValue)
{
}

void Module::onJoyButtonDown(Uint8 inWhich, Uint8 inButton)
{
}

void Module::onJoyButtonUp(Uint8 inWhich, Uint8 inButton)
{
}

void Module::onJoyHat(Uint8 inWhich, Uint8 inHat, Uint8 inValue)
{
}

void Module::onJoyBall(Uint8 inWhich, Uint8 inBall, Sint16 inXRel,
    Sint16 inYRel)
{
}

void Module::onMinimize()
{
}

void Module::onRestore()
{
}

void Module::onResize(int inWidth, int inHeight)
{
}

void Module::onExpose()
{
}

void Module::onExit()
{
    mRunning = false;
}

void Module::onUser(Uint8 inType, int inCode, void* inData1, void* inData2)
{
}
