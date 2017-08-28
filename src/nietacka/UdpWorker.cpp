//
// Created by Piotr Jander on 27/08/2017.
//

#include <chrono>
#include <unistd.h>
#include <poll.h>
#include "UdpWorker.h"
#include "Exceptions.h"
#include "GameManager.h"
#include "easylogging++.h"

using namespace std::chrono;

void UdpWorker::enqueue(std::unique_ptr<IDatagram> datagram)
{
    queue.emplace_back(std::move(datagram));
}

void UdpWorker::getDatagram(IDatagramObserver &observer)
{
     ssize_t length = socket.recvFrom(&buffer, sizeof(ClientMessage::SelfPacked));

    if (length < sizeof(ClientMessage::SelfPacked)) {
        LOG(WARNING) << "Datagram too small to be valid";
    }

    observer.processDatagram(&buffer, socket.getSockaddr());
    LOG(INFO) << "Got a client message";
}

/**
 * 
 */
void UdpWorker::workUntil(std::chrono::milliseconds endOfFrame, IDatagramObserver &observer)
{
    socket.setNonBlocking();

    while (true) {
        milliseconds remainingTime = endOfFrame - duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        long long remainingTimeLong = remainingTime.count();

        if (remainingTimeLong <= 0) {
            break;
        }
        
        int rc = socket.socketPoll(remainingTimeLong, !queue.empty());
        if (rc == 0) {
            // timeout
            break;
        } else {
            if (rc & POLLIN) {
                try {
                    getDatagram(observer);
                } catch (WouldBlockException &e) {};
            }
            if (rc & POLLOUT) {
                sendDatagramToNonblockingSocket(observer);
            }
        }
    }

    socket.setBlocking();
}

void UdpWorker::sendDatagramToNonblockingSocket(IDatagramObserver &observer)
{
    if (!queue.empty()) {
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
}

UdpWorker::UdpWorker(const string &port) : queue(), socket()
{
    socket.bindSocket(NULL, port.c_str());
}
