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

#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include "Config.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <sstream>

#include <iostream>

typedef Mix_Chunk* SoundEffect;
typedef Mix_Music* Music;

#define NUM_CHANNELS 32

class SoundEngine
{
    public:
        static void initialize();

        static void loadBackgroundMusic(const char* inFile);

        static void playBackgroundMusic();
        static void stopBackgroundMusic();

        static void increaseMusicVolume();
        static void decreaseMusicVolume();

        static SoundEffect loadSound(const char* inFile);
        static void unloadSound(SoundEffect inSound);

        static void playSound(SoundEffect inSound);
        static void playPositionalSound(SoundEffect inSound, int inAngle,
            int inDistance);

        static void cleanup();

    private:
        static int mAudioRate;
        static Uint16 mAudioFormat;
        static int mNumChannels;
        static int mAudioBufferSize;

        static Music mBackgroundMusic;
        static SoundEffect mTankFire;
        static int mChannel;
        static int mMusicVolume;
};

#endif
