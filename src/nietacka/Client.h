//
// Created by Piotr Jander on 29/08/2017.
//

#ifndef PROJECT_CLIENT_H
#define PROJECT_CLIENT_H


#include "Socket.h"
#include "ClientMessage.h"

class Client {
public:
    Client(sockaddr *serverAddr);

    void loop();

private:
    Socket socket;
    ClientMessage message;
    sockaddr *serverAddr;
};


#endif //PROJECT_CLIENT_H
