//
// Created by Piotr Jander on 27/08/2017.
//

#ifndef PROJECT_SOCKET_H
#define PROJECT_SOCKET_H


#include <sys/socket.h>
#include <string>

class Socket {
public:
    static struct addrinfo *getAddrInfo(const char *hostname, const char *port);

    Socket();
    Socket(const Socket&) = delete;
    Socket & operator=(const Socket&) = delete;

    virtual ~Socket();

    void bindSocket(struct addrinfo *servinfo);

    void bindSocket(const char *hostname, const char *port);

    ssize_t recvFrom(void *buffer, size_t length);

    ssize_t sendTo(const void *buffer, size_t length, const sockaddr *sockAddr);

    sockaddr *getSockaddr()
    {
        return reinterpret_cast<sockaddr *>(&sockaddrStorage);
    }

    short socketPoll(long long int timeout);
    
    static sockaddr_storage copySockAddrToStorage(const sockaddr *sockAddr);

private:
    int socket_fd;
    sockaddr_storage sockaddrStorage;

    void setBlocking();

    void setNonBlocking();
};


#endif //PROJECT_SOCKET_H
