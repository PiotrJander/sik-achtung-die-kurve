//
// Created by Piotr Jander on 27/08/2017.
//

#include <unistd.h>
#include "UdpWorker.h"

void UdpWorker::enqueue(std::unique_ptr<IDatagram> datagram)
{
    queue.emplace(datagram);
}

std::pair<const ClientMessage::SelfPacked *, const sockaddr *> UdpWorker::getDatagram()
{
    ssize_t length = recvfrom(socket_fd, &buffer, sizeof(ClientMessage::SelfPacked), 0, getSockaddr(), &storageSize);
    if (length == -1) {
        throw new SocketException(errno);
    } else if (length < sizeof(ClientMessage::SelfPacked)) {
        throw new ProtocolException("Datagram too small to be valid");
    } else {
        return std::make_pair(&buffer, getSockaddr());
    }
}

void UdpWorker::workUntil(std::chrono::milliseconds time, IDatagramObserver &observer)
{

}

UdpWorker::UdpWorker(uint16_t port, const std::queue &queue) : queue(queue)
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

UdpWorker::~UdpWorker()
{
    close(socket_fd);
}

