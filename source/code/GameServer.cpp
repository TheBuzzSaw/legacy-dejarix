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

#include "GameServer.h"
#include "Config.h"
#include "NetworkStream.h"

#include <SDL_net.h>
#include <iostream>
using namespace std;

GameServer::GameServer()
{
}

GameServer::~GameServer()
{
}

void GameServer::run()
{
    NetworkStream net;
    net.listen(Config::get<Uint16>("server port", 9421));

    Uint8 buffer[PACKET_SIZE];

    while (!stopRequested())
    {
        while (net.receiveData(buffer))
        {
            cout << "SERVER -- received message: " << buffer << endl;
        }

        SDL_Delay(1);
    }
}
