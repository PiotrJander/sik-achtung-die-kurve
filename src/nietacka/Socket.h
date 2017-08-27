//
// Created by Piotr Jander on 27/08/2017.
//

#ifndef PROJECT_SOCKET_H
#define PROJECT_SOCKET_H


#include <sys/socket.h>

class Socket {
public:
    Socket(uint16_t port);

    virtual ~Socket();

    ssize_t recvFrom(void *buffer, size_t length);

    sockaddr *getSockaddr()
    {
        return reinterpret_cast<sockaddr *>(&sockaddrStorage);
    }

private:
    int socket_fd;
    sockaddr_storage sockaddrStorage;
};


#endif //PROJECT_SOCKET_H
