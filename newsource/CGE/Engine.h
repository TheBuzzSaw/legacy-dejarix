#ifndef ENGINE_H
#define ENGINE_H

#include "Graphics.h"
#include "ManagedModule.h"

namespace CGE
{
    const Uint32 ENGINE_FPS = 40;
    const Uint32 FRAME_LENGTH = 1000 / ENGINE_FPS;

    struct ColorMask
    {
        Uint32 red;
        Uint32 green;
        Uint32 blue;
        Uint32 alpha;
    };

    class Engine
    {
        public:
            Engine();
            ~Engine();

            void run(Module* inModule);
            void manage(ManagedModule* inModule);

        private:
            ColorMask mMask;
            PropertyList mConfig;
            Surface mDisplay;
            Surface mWindowIcon;
            SDL_Rect** mModes;
            Uint32 mFPS;
    };
}

#endif
