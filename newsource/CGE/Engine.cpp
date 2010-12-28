#include "Engine.h"
#include "Module.h"
#include "Tools.h"

#include <SDL_ttf.h>
#include <SDL_net.h>

#include <iostream>
#include <list>

namespace CGE
{
    using namespace std;

    Engine::Engine() : mWindowIcon(NULL), mFPS(0)
    {
        if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
        {
            cerr << "-- error on SDL_Init --" << endl;
            exit(1);
        }

        if (TTF_Init() == -1)
        {
            cerr << "-- error on TTF_Init -- " << TTF_GetError() << endl;
            exit(1);
        }

        if (SDLNet_Init() == -1)
        {
            cerr << "-- error on SDLNet_Init -- " << SDLNet_GetError() << endl;
            exit(1);
        }

#ifdef __WIN32__
        // redirect output to screen (instead of text files)
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
#endif

        // get available full screen modes
        mModes = SDL_ListModes(NULL, SDL_FULLSCREEN);

        if (mModes == (SDL_Rect**)0)
        {
            cout << "No modes available!\n";
        }
        else if (mModes == (SDL_Rect**)-1)
        {
            cout << "All resolutions available.\n";
        }
        else
        {
            cout << "-- Available Full Screen Resolutions --";
            for (size_t i = 0; mModes[i]; ++i)
                cout << "\n    " << mModes[i]->w << " x " << mModes[i]->h;

            cout << endl;
        }

        int width = mConfig.get("display width", 800);
        int height = mConfig.get("display height", 600);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        mMask.red   = 0xff000000;
        mMask.green = 0x00ff0000;
        mMask.blue  = 0x0000ff00;
        mMask.alpha = 0x000000ff;
#else
        mMask.red   = 0x000000ff;
        mMask.green = 0x0000ff00;
        mMask.blue  = 0x00ff0000;
        mMask.alpha = 0xff000000;
#endif

        Uint32 flags = SDL_OPENGL;
        if (mConfig.get("full screen", 0)) flags |= SDL_FULLSCREEN;
        SDL_WM_SetCaption("paroxysm 0.0.1", "paroxysm");
        mDisplay = SDL_SetVideoMode(width, height,
            mConfig.get("bits per pixel", 24), flags);

#ifndef __APPLE__
        // OSX does not support window icons
        mWindowIcon = loadImage("data/images/icon.bmp");
        if (mWindowIcon != NULL)
        {
            SDL_WM_SetIcon(mWindowIcon, NULL);
        }
        else
        {
            cerr << "window icon failed" << endl;
        }
#endif

    }

    Engine::~Engine()
    {
#ifndef __APPLE__
        SDL_FreeSurface(mWindowIcon);
#endif
        SDLNet_Quit();
        TTF_Quit();
        SDL_Quit();
    }

    void Engine::run(Module* inModule)
    {
        if (!inModule) return;

        inModule->onOpen();
        inModule->onResize(mDisplay->w, mDisplay->h);

        Uint32 nextFrame = SDL_GetTicks() + FRAME_LENGTH;
        Uint32 nextSecond = SDL_GetTicks() + 1000;
        Uint32 framesPerSecond = 0;

        while (inModule->isRunning())
        {
            SDL_Event event;
            while (SDL_PollEvent(&event)) inModule->onEvent(&event);

            Uint32 ticks = SDL_GetTicks();

            if (ticks > nextSecond)
            {
                nextSecond += 1000;
                mFPS = framesPerSecond;
                framesPerSecond = 0;
            }

            if (ticks > nextFrame)
            {
                inModule->onFrame();
                nextFrame += FRAME_LENGTH;
            }
            else
            {
                inModule->onLoop();
                SDL_GL_SwapBuffers();
                ++framesPerSecond;
            }

            SDL_Delay(1); // prevent CPU abuse
        }

        inModule->onClose();
    }

    void Engine::manage(ManagedModule* inModule)
    {
        if (!inModule) return;

        ManagedModule* currentModule = inModule;
        list<ManagedModule*> moduleStack;

        while (currentModule || moduleStack.size())
        {
            if (!currentModule)
            {
                currentModule = moduleStack.back();
                moduleStack.pop_back();
            }

            currentModule->onLoad(mConfig);

            run(currentModule);

            ManagedModule* deadModule = currentModule;
            currentModule = currentModule->nextModule();

            if (deadModule->isDead())
            {
                deadModule->onUnload();
                delete deadModule;
            }
            else
            {
                moduleStack.push_back(deadModule);
                deadModule = NULL;
            }
        }
    }
}
