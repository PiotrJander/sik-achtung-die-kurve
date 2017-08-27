//
// Created by Piotr Jander on 27/08/2017.
//

#include <cstdint>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Socket.h"
#include "Exceptions.h"

Socket::Socket(uint16_t port)
{
    if ((socket_fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        throw new SocketException(errno);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int res = bind(socket_fd, reinterpret_cast<sockaddr *>(&address), sizeof(sockaddr_in));
    if (res == -1) {
        throw new SocketException(errno);
    }
}

Socket::~Socket()
{
    if (close(socket_fd) == -1) {
        // I know that throwing from dtors is dangerous
        throw new SocketException(errno);
    }
}

ssize_t Socket::recvFrom(void *buffer, size_t length)
{
    socklen_t storageSize = sizeof(sockaddr_storage);
    ssize_t len = recvfrom(socket_fd, buffer, length, 0, getSockaddr(), &storageSize);
    if (len == -1) {
        throw new SocketException(errno);
    } else {
        return len;
    }
}
