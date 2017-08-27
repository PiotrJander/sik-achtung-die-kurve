//
// Created by Piotr Jander on 27/08/2017.
//

#include <unistd.h>
#include "UdpWorker.h"

void UdpWorker::enqueue(const IDatagram &datagram)
{

}

std::pair<const ClientMessage::SelfPacked *, const sockaddr *> UdpWorker::getDatagram()
{
    return nullptr;
}

void UdpWorker::workUntil(std::chrono::milliseconds time, IDatagramObserver &observer)
{

}

UdpWorker::UdpWorker(uint16_t port, const std::queue &queue) : queue(queue)
{
    if ((socket_fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        throw new SocketException(socket_fd);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int res = 0;
    if ((res = bind(socket_fd, reinterpret_cast<sockaddr *>(&address), sizeof(sockaddr_in))) == -1) {
        throw new SocketException(res);
    }
}

UdpWorker::~UdpWorker()
{
    close(socket_fd);
}

