//
// Created by Piotr Jander on 27/08/2017.
//

#include <chrono>
#include <unistd.h>
#include <poll.h>
#include "UdpWorker.h"
#include "Exceptions.h"
#include "easylogging++.h"

using namespace std::chrono;

void UdpWorker::enqueue(std::unique_ptr<IDatagram> datagram)
{
    queue.emplace_back(std::move(datagram));
}

void UdpWorker::work(IDatagramObserver &observer)
{
    // -1 means block indefinitely; can do it as game not in progress
    short rc = socket.socketPoll(-1, !queue.empty());
    actOnSocket(rc, observer);
}

void UdpWorker::workUntil(std::chrono::milliseconds endOfFrame, IDatagramObserver &observer)
{
    while (true) {
        milliseconds remainingTime = endOfFrame - duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        long long remainingTimeLong = remainingTime.count();

        if (remainingTimeLong <= 0) {
            break;
        }

        short rc = socket.socketPoll(remainingTimeLong, !queue.empty());
        if (rc == 0) {
            // timeout
            break;
        } else {
            actOnSocket(rc, observer);
        }
    }
}

void UdpWorker::actOnSocket(short pollResult, IDatagramObserver &observer)
{
        if (pollResult & POLLIN) {
            receiveDatagram(observer);
        }
        if (pollResult & POLLOUT) {
            sendDatagram();
        }
}

void UdpWorker::receiveDatagram(IDatagramObserver &observer)
{
    ssize_t length=0;
    try {
        length = socket.recvFrom(&buffer, sizeof(ClientMessage::SelfPacked));
    } catch (WouldBlockException &e) {
        return;
    } catch (InterruptedException &e) {
        return;
    };

    if (length < sizeof(ClientMessage::SelfPacked)) {
        LOG(WARNING) << "Datagram too small to be valid";
        return;
    }

    observer.processDatagram(&buffer, socket.getSockaddr());
    LOG(INFO) << "Got a client message";
}

void UdpWorker::sendDatagram()
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
        } catch (InterruptedException &e) {
            queue.emplace_front(std::move(datagram));
        }
    }
}

UdpWorker::UdpWorker(const string &port) : queue(), socket()
{
    socket.bindSocket(NULL, port.c_str());
    socket.setNonBlocking();
}
