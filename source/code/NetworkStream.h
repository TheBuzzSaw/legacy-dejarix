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

#ifndef NETWORKSTREAM_H
#define NETWORKSTREAM_H

#include <SDL.h>
#include <SDL_net.h>

#define PACKET_SIZE 1024

class NetworkStream
{
    public:
        NetworkStream();
        ~NetworkStream();

        void listen(Uint16 inPort);
        void connect(const char* inAddress, Uint16 inPort);
        void sendData(const void* inData, size_t inLength);
        bool receiveData(Uint8* inCapture);

        inline UDPpacket* getPacket() { return mPacket; }

    private:
        IPaddress mAddress;
        UDPsocket mSocket;
        UDPpacket* mPacket;
};

#endif
