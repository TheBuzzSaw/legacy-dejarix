#include "Engine.h"
#include "Module.h"
#include "Platforms.h"
#include "Image.h"

#include <SDL_ttf.h>
#include <SDL_net.h>

#include <ctime>
#include <fstream>
#include <string>
#include <list>

namespace CGE
{
    using namespace std;

    const char* Engine::logFile = NULL;
    const char* Engine::configFile = NULL;

    void Engine::prepareFiles()
    {
        if (logFile) return; // only proceed if the pointer is null

        // TODO: adjust to store in home folder on UNIX platform
        logFile = "data/logs/init.txt";
        configFile = "data/settings.txt";
    }

    void Engine::logOpenGL(ostream& inStream)
    {
        inStream << "Vendor:   " << glGetString(GL_VENDOR);
        inStream << "\nRenderer: " << glGetString(GL_VERSION);
        const GLubyte* c = glGetString(GL_VERSION);
        inStream << "\nVersion:  " << c;

        if (c[0] >= '2')
        {
            inStream << "\nShader:   "
                << glGetString(GL_SHADING_LANGUAGE_VERSION);
        }

        inStream << "\n\n---[ EXTENSIONS ]---";
        const char* e = reinterpret_cast<const char*>
            (glGetString(GL_EXTENSIONS));
        string extensions(e);

        for (size_t i = 0; i < extensions.length(); ++i)
        {
            inStream << '\n';

            size_t j = extensions.find_first_of(' ', i);
            if (j != string::npos)
            {
                inStream << extensions.substr(i, j - i);
                i = j;
            }
            else
            {
                inStream << extensions.substr(i);
                i = extensions.length();
            }
        }

        inStream << "\n\n";
    }

    Engine::Engine(const Settings& inSettings) : mWindowIcon(NULL),
        mSettings(inSettings)
    {
        initialize();
    }

    Engine::~Engine()
    {
        ofstream fout(logFile, ofstream::app);
        if (fout)
        {
            fout << "-- settings used --\n" << mConfig;
            fout.close();
        }

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

    void Engine::initialize()
    {
        prepareFiles();

        ofstream fout(logFile);
        if (!fout)
        {
            cerr << "unable to create " << logFile << '\n';
            exit(1);
        }

        {
            time_t rawtime;
            tm* timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            fout << "log begun at " << asctime(timeinfo) << "\n\n";
        }

        ifstream fin(configFile);
        if (fin)
        {
            fin >> mConfig;
            fin.close();
        }
        else
        {
            fout << "missing " << configFile << " -- loading defaults\n\n";
        }

        if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
        {
            cerr << "-- error on SDL_Init --" << endl;
            fout.close();
            exit(1);
        }

        if (TTF_Init() == -1)
        {
            cerr << "-- error on TTF_Init -- " << TTF_GetError() << endl;
            fout.close();
            exit(1);
        }

        if (SDLNet_Init() == -1)
        {
            cerr << "-- error on SDLNet_Init -- " << SDLNet_GetError() << endl;
            fout.close();
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
            fout << "No modes available!\n";
        }
        else if (mModes == (SDL_Rect**)-1)
        {
            fout << "All resolutions available.\n";
        }
        else
        {
            fout << "-- Available Full Screen Resolutions --";
            for (SDL_Rect** i = mModes; *i; ++i)
            {
                const SDL_Rect* r = *i;
                fout << "\n    " << r->w << " x " << r->h;
            }

            fout << "\n\n";
        }

        int width = mConfig.get("display width", 800);
        int height = mConfig.get("display height", 600);

        Uint32 flags = SDL_OPENGL;
        if (mConfig.get("full screen", 0)) flags |= SDL_FULLSCREEN;

        if (!mSettings.windowTitle)
            mSettings.windowTitle = "CYBORUS Game Engine";

        SDL_WM_SetCaption(mSettings.windowTitle, mSettings.windowTitle2);

        mDisplay = SDL_SetVideoMode(width, height,
            mConfig.get("bits per pixel", 24), flags);

#ifndef __APPLE__
        // OSX does not support window icons
        setWindowIcon(Image("data/images/icon.bmp"));
#endif

        logOpenGL(fout);

        fout.close();
    }
}
