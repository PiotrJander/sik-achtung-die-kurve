//
// Created by Piotr Jander on 27/08/2017.
//

#include <chrono>
#include <unistd.h>
#include <poll.h>
#include "UdpWorker.h"
#include "Exceptions.h"
#include "GameManager.h"

using namespace std::chrono;

void UdpWorker::enqueue(std::unique_ptr<IDatagram> datagram)
{
    queue.emplace_back(std::move(datagram));
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

/**
 * 
 */
void UdpWorker::workUntil(std::chrono::milliseconds endOfFrame, IDatagramObserver &observer)
{
    while (true) {
        milliseconds remainingTime = endOfFrame - duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        long long remainingTimeLong = remainingTime.count();
        
        int rc = socket.socketPoll(remainingTimeLong);
        if (rc == 0) {
            // timeout
            break;
        } else {
            if (rc & POLLIN) {
                getDatagramFromNonblockingSocket(observer);
            }
            if (rc & POLLOUT) {
                sendDatagramToNonblockingSocket(observer);
            }
        }
    }
}

void UdpWorker::sendDatagramToNonblockingSocket(IDatagramObserver &observer)
{
    auto datagram = std::move(queue.front());
    const sockaddr *sockAddr = datagram->getSockAddr();
    int length = datagram->getLength();
    auto bufferBox = datagram->getBuffer();
    const char *buffer = bufferBox.get();

    try {
        socket.sendTo(buffer, static_cast<size_t>(length), sockAddr);
    } catch (WouldBlockException &e) {
        // try again next time
        queue.emplace_front(std::move(datagram));
    };
}

void UdpWorker::getDatagramFromNonblockingSocket(IDatagramObserver &observer)
{
    ssize_t length=0;

    try {
        length = socket.recvFrom(&buffer, sizeof(ClientMessage::SelfPacked));
    } catch (WouldBlockException &e) {};

    if (length < sizeof(ClientMessage::SelfPacked)) {
        throw ProtocolException("Datagram too small to be valid");
    } else {
        observer.processDatagram(&buffer, socket.getSockaddr());
    }
}

UdpWorker::UdpWorker(const string &port) : queue(), socket()
{
    socket.bindSocket(NULL, port.c_str());
}
