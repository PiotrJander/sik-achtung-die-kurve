//
// Created by Piotr Jander on 17/08/2017.
//

#include "MockSocket.h"

using std::stringstream;
using std::make_shared;

MockSocket::MockSocket()
{
    serverToClient = make_shared<stringstream>();
    clientToServer = make_shared<stringstream>();
}

shared_ptr<stringstream> MockSocket::serverRead()
{
    return clientToServer;
}

shared_ptr<stringstream> MockSocket::serverWrite()
{
    return serverToClient;
}

shared_ptr<stringstream> MockSocket::clientRead()
{
    return serverToClient;
}

shared_ptr<stringstream> MockSocket::clientWrite()
{
    return clientToServer;
}
