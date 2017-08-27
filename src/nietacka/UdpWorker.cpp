//
// Created by Piotr Jander on 27/08/2017.
//

#include <unistd.h>
#include "UdpWorker.h"
#include "Exceptions.h"

void UdpWorker::enqueue(std::unique_ptr<IDatagram> datagram)
{
    queue.emplace(std::move(datagram));
}

std::pair<const ClientMessage::SelfPacked *, const sockaddr *> UdpWorker::getDatagram()
{
    ssize_t length = socket.recvFrom(&buffer, sizeof(ClientMessage::SelfPacked));
    if (length < sizeof(ClientMessage::SelfPacked)) {
        throw ProtocolException("Datagram too small to be valid");
    } else {
        return std::make_pair(&buffer, socket.getSockaddr());
    }
}

void UdpWorker::workUntil(std::chrono::milliseconds time, IDatagramObserver &observer)
{

}

UdpWorker::UdpWorker(const string &port) : queue(), socket()
{
    socket.bindSocket(NULL, port.c_str());
}
