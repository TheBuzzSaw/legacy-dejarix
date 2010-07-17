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

#include "NetworkStream.h"

#include <iostream>
#include <cstring>
using namespace std;

/**
 *  ABSTRACT -- NetworkStream has a very simple focus: move blocks of memory
 *              between the client and server. In other words, regardless of how
 *              big the message is, it needs to be properly divided, labeled,
 *              and transmitted with respect to the allotted packet size. In
 *              many respects, it will mimic TCP's reliability but have the
 *              benefit of speedy UDP through reduced overhead and added
 *              tolerance for lost packets.
 *
 *  (1) break messages across packets -- Every packet needs a small header
 *      indicating what message it is a part of (message ID), how many packets
 *      constitute the message, and what packet this is in the series.
 *
 *  (2) manage message pieces -- Every packet needs to be tracked carefully. If
 *      pieces of unrelated messages show up, they need to be stored properly
 *      for when the rest of the message arrives.
 */

NetworkStream::NetworkStream() : mSocket(0)
{
    mPacket = SDLNet_AllocPacket(PACKET_SIZE);
}

NetworkStream::~NetworkStream()
{
    SDLNet_FreePacket(mPacket);
    if (mSocket) SDLNet_UDP_Close(mSocket);
}

void NetworkStream::listen(Uint16 inPort)
{
    if (!mSocket)
    {
        mSocket = SDLNet_UDP_Open(inPort);
        if (!mSocket)
        {
            cerr << "failed to listen on port " << inPort << endl;
        }
    }
}

void NetworkStream::connect(const char* inAddress, Uint16 inPort)
{
    if (!mSocket)
    {
        mSocket = SDLNet_UDP_Open(0);
        if (!mSocket)
        {
            cerr << "failed to listen on port " << inPort << endl;
        }
    }

    if (SDLNet_ResolveHost(&mAddress, inAddress, inPort) == -1)
    {
        cerr << "failed to resolve host -- " << inAddress << " on port "
            << inPort << endl;
    }
}

void NetworkStream::sendData(const void* inData, size_t inLength)
{
    if (inLength > PACKET_SIZE)
    {
        cerr << "data too large to transmit" << endl;
    }

    memcpy(mPacket->data, inData, inLength);
    mPacket->len = inLength;
    mPacket->address.host = mAddress.host;
    mPacket->address.port = mAddress.port;
    SDLNet_UDP_Send(mSocket, -1, mPacket);
}

bool NetworkStream::receiveData(Uint8* inCapture)
{
    if (SDLNet_UDP_Recv(mSocket, mPacket))
    {
        memcpy(inCapture, mPacket->data, mPacket->len);
        cerr << "mPacket->address/port: " << mPacket->address.host << ", " << SDL_Swap16(mPacket->address.port) << endl;
        return true;
    }

    return false;
}
