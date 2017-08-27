//
// Created by Piotr Jander on 27/08/2017.
//

#include <cstdint>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
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

Socket::Socket(struct addrinfo *servinfo)
{
    socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (socket_fd == -1) {
        throw SocketException(errno);
    }

    int res = bind(socket_fd, servinfo->ai_addr, sizeof(sockaddr_in));
    if (res == -1) {
        throw SocketException(errno);
    }

    freeaddrinfo(servinfo);
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
        LOG(ERROR) << "errno is " << errno;
        throw SocketException(errno);
    } else {
        return len;
    }
}

ssize_t Socket::sendTo(const void *buffer, size_t length, const sockaddr *sockAddr)
{
    ssize_t len = sendto(socket_fd, buffer, length, 0, sockAddr, sizeof(sockaddr_storage));
    if (len == -1) {
        throw SocketException(errno);
    } else {
        return len;
    }
}
