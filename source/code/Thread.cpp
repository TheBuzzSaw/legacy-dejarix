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

#include "Thread.h"

Thread::Thread() : mRunning(false), mStop(false)
{
}

Thread::~Thread()
{
}

void Thread::start(Uint32 inDelay)
{
    if (mRunning) return;
    mThread = SDL_CreateThread(startThread, this);
    SDL_Delay(inDelay);
}

int Thread::startThread(void* inData)
{
    Thread* t = (Thread*)inData;
    t->mStop = false;
    t->mRunning = true;
    t->run();
    t->mRunning = false;
    return 0;
}

/**
 *  Waits for a thread to finish its run() function. WARNING: Needs to be called
 *  on a thread that will end eventually without any kind of stop() call (or
 *  where you have called it prior).
 */
void Thread::wait()
{
    if (mRunning) SDL_WaitThread(mThread, NULL);
}

/**
 *  Both requests a stop in the thread and waits until the thread is no longer
 *  running. This is just an alternative to having to call stop() and wait()
 *  separately.
 */
void Thread::stopAndWait()
{
    stop();
    wait();
}
