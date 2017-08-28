//
// Created by Piotr Jander on 27/08/2017.
//

#include <cstdint>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <fcntl.h>
#include "Socket.h"
#include "Exceptions.h"
#include "easylogging++.h"

//Socket::Socket(uint16_t port)
//{
//    if ((socket_fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
//        throw SocketException(errno);
//    }
//
//    // TODO maybe use getaddrinfo
//    sockaddr_in address;
//    address.sin_family = AF_INET;
//    address.sin_addr.s_addr = INADDR_ANY;
//    address.sin_port = htons(port);
//
//    int res = bind(socket_fd, reinterpret_cast<sockaddr *>(&address), sizeof(sockaddr_in));
//    if (res == -1) {
//        throw SocketException(errno);
//    }
//}

/**
 * Call freeaddrinfo(servinfo) after use!
 */
struct addrinfo *Socket::getAddrInfo(const char *hostname, const char *port)
{
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;  // will point to the results

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    // TODO customise

    status = getaddrinfo(hostname, port, &hints, &servinfo);
    if (status == -1) {
        throw SocketException(errno);
    }

    return servinfo;
}

Socket::Socket()
{
    socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (socket_fd == -1) {
        throw SocketException(errno);
    }
}

void Socket::bindSocket(struct addrinfo *servinfo)
{
    int res = bind(socket_fd, servinfo->ai_addr, sizeof(sockaddr_in));
    if (res == -1) {
        throw SocketException(errno);
    }
}

void Socket::bindSocket(const char *hostname, const char *port)
{
    addrinfo *addrInfo = getAddrInfo(hostname, port);
    bindSocket(addrInfo);
    freeaddrinfo(addrInfo);
}

Socket::~Socket()
{
    if (close(socket_fd) == -1) {
        // I know that throwing from dtors is dangerous
        throw SocketException(errno);
    }
}

ssize_t Socket::recvFrom(void *buffer, size_t length)
{
    socklen_t storageSize = sizeof(sockaddr_storage);
    ssize_t len = recvfrom(socket_fd, buffer, length, 0, getSockaddr(), &storageSize);
    if (len == -1) {
        if (errno == EWOULDBLOCK) {
            throw WouldBlockException();
        } else if (errno == EINTR) {
            throw InterruptedException();
        } else {
            throw SocketException(errno);
        }
    } else {
        return len;
    }
}

ssize_t Socket::sendTo(const void *buffer, size_t length, const sockaddr *sockAddr)
{
    ssize_t len = sendto(socket_fd, buffer, length, 0, sockAddr, sizeof(sockaddr_in));  // TODO what about IPv6?
    if (len == -1) {
        if (errno == EWOULDBLOCK) {
            throw WouldBlockException();
        } else if (errno == EINTR) {
            throw InterruptedException();
        } else {
            throw SocketException(errno);
        }
    } else {
        return len;
    }
}


short Socket::socketPoll(long long int timeout, bool send)
{
    struct pollfd fds[1];

    memset(fds, 0 , sizeof(fds));
    fds[0].fd = socket_fd;
    fds[0].events = POLLIN | (send ? POLLOUT : 0);

    int rc = poll(fds, 1, timeout);
    if (rc == -1) {
        if (errno == EINTR) {
            throw InterruptedException();
        } else {
            throw SocketException(errno);
        }
    } else if (rc == 0) {
        // timeout
        return 0;
    } else if (fds[0].revents & POLLHUP || fds[0].revents & POLLERR || fds[0].revents & POLLNVAL) {
        std::ostringstream stringStream;
        stringStream << "Poll revents error: " << fds[0].revents;
        throw SocketException(0, stringStream.str());
    } else {
        return fds[0].revents;
    }
}

/**
 * I hope I don't mess with other flags.
 *
 * TODO ioctl vs fcntl
 */
void Socket::setBlocking()
{
    if (fcntl(socket_fd, F_SETFL, 0) < 0) {
        throw SocketException(errno);
    }
}

void Socket::setNonBlocking()
{
    if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0) {
        throw SocketException(errno);
    }
}

sockaddr_storage Socket::copySockAddrToStorage(const sockaddr *sockAddr)
{
    sockaddr_storage res;
    switch (sockAddr->sa_family) {
        case AF_INET: {
            memcpy(&res, sockAddr, sizeof(sockaddr_in));
        }
        case AF_INET6: {
            memcpy(&res, sockAddr, sizeof(sockaddr_in6));
        }
    }
    return res;
}
