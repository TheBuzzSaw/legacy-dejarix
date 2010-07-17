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

#include "SoundEngine.h"

using namespace std;

int SoundEngine::mAudioRate;
Uint16 SoundEngine::mAudioFormat;
int SoundEngine::mNumChannels;
int SoundEngine::mAudioBufferSize;

Music SoundEngine::mBackgroundMusic;
SoundEffect SoundEngine::mTankFire;
int SoundEngine::mChannel = 1;
int SoundEngine::mMusicVolume = 100;


void SoundEngine::initialize()
{
#if SDL_MIXER_PATCHLEVEL > 15
    // SDL_mixer 1.2.10 introduced Mix_Init() and Mix_Quit()
    int flags = MIX_INIT_MP3 | MIX_INIT_OGG;

    int init = Mix_Init(flags);

    if ((init & flags) != flags)
    {
        cerr << "something couldn't load" << endl;
    }
#endif

    mBackgroundMusic = NULL;

    mAudioRate = Config::get("audio rate", 22050);
    mAudioFormat = Config::get("audio format", AUDIO_S16SYS);
    mNumChannels = Config::get("audio channels", 2);
    mAudioBufferSize = Config::get("audio buffer size", 1024);



    if (Mix_OpenAudio(mAudioRate, mAudioFormat, mNumChannels, mAudioBufferSize) == -1)
    {
        cerr << "Audio Initialization failed." << endl;
        cerr << Mix_GetError() << endl;
        exit(2);
    }



    Mix_AllocateChannels(NUM_CHANNELS);

}


void SoundEngine::loadBackgroundMusic(const char* inFile)
{
    stringstream music;
    music << "data/audio/" << inFile;
    //music << Config::get<string>("background music", inFile);


    /* Actually loads up the music */
    if ((mBackgroundMusic = Mix_LoadMUS(music.str().c_str())) == NULL)
    {
        cerr << Mix_GetError()  << " " << music.str() << endl;
    }

}


void SoundEngine::playBackgroundMusic()
{
    if (!Mix_PlayingMusic())
    {
        /* This begins playing the music - the first argument is a
           pointer to Mix_Music structure, and the second is how many
           times you want it to loop (use -1 for infinite, and 0 to
           have it just play once) */
        if ((Mix_PlayMusic(mBackgroundMusic, -1)) == -1)
        {
            cerr << Mix_GetError() << endl;
        }
    }



    /* We want to know when our music has stopped playing so we
       can free it up and set 'music' back to NULL.  SDL_Mixer
       provides us with a callback routine we can use to do
       exactly that */
    //Mix_HookMusicFinished(musicDone);

}

void SoundEngine::stopBackgroundMusic()
{
    //cerr << "Music playing?: " << Mix_PlayingMusic() << endl;

    if (Mix_PlayingMusic())
    {
        /* Stop the music from playing */
        Mix_HaltMusic();
    }

    /* Unload the music from memory, since we don't need it
       anymore */
    //Mix_FreeMusic(mBackgroundMusic);

    //mBackgroundMusic = NULL;
}

SoundEffect SoundEngine::loadSound(const char* inFile)
{
    SoundEffect sound;
    stringstream soundFile;
    soundFile << "data/audio/" << inFile;

    if ((sound = Mix_LoadWAV(soundFile.str().c_str())) == NULL)
    {
        cerr << "something wrong: " << Mix_GetError() << endl;
    }

    return sound;
}

void SoundEngine::playSound(SoundEffect inSound)
{
    if (Mix_PlayChannel(mChannel, inSound, 0) == -1)
    {
        cerr << Mix_GetError() << endl;
    }
    mChannel = (mChannel + 1) % NUM_CHANNELS;
}

void SoundEngine::playPositionalSound(SoundEffect inSound, int inAngle, int inDistance)
{
    if (!Mix_SetPosition(mChannel, inAngle, inDistance))
    {
        cerr << Mix_GetError() << endl;
    }
    if (Mix_PlayChannel(mChannel, inSound, 0) == -1)
    {
        cerr << Mix_GetError() << endl;
    }
    mChannel = (mChannel + 1) % NUM_CHANNELS;
}

void unloadSound(SoundEffect inSound)
{
    Mix_FreeChunk(inSound);
    inSound = NULL;
}


void SoundEngine::increaseMusicVolume()
{
    mMusicVolume += 5;

    if (mMusicVolume > 128)
    {
        mMusicVolume = 128;
    }
    Mix_VolumeMusic(mMusicVolume);
    cerr << "\nVolume now: " << Mix_VolumeMusic(-1) << endl;
}


void SoundEngine::decreaseMusicVolume()
{
    mMusicVolume -= 5;

    if (mMusicVolume < 0)
    {
        mMusicVolume = 0;
    }
    Mix_VolumeMusic(mMusicVolume);

    cerr << "\nVolume now: " << Mix_VolumeMusic(-1) << endl;
}

/*
void SoundEngine::tankFire(int inAngle, int inDistance)
{

    if (!Mix_SetPosition(mChannel, inAngle, inDistance))
    {
        cerr << Mix_GetError() << endl;
    }
    if (Mix_PlayChannel(mChannel, mTankFire, 0) == -1)
    {
        cerr << Mix_GetError() << endl;
    }
    mChannel = (mChannel + 1) % 16;
}
*/

void SoundEngine::cleanup()
{
#if SDL_MIXER_PATCHLEVEL > 15
    // SDL_mixer 1.2.10 introduced Mix_Init() and Mix_Quit()
    Mix_Quit();
#endif

    Mix_CloseAudio();
}
