//
// Created by Piotr Jander on 27/08/2017.
//

#ifndef PROJECT_UDPWORKER_H
#define PROJECT_UDPWORKER_H


#include <queue>
#include <netdb.h>
#include "IUdpWorker.h"
#include "Socket.h"

class UdpWorker: public IUdpWorker {
public:
    void enqueue(std::unique_ptr<IDatagram> datagram) override;

    std::pair<const ClientMessage::SelfPacked *, const sockaddr *> getDatagram() override;

    void workUntil(std::chrono::milliseconds time, IDatagramObserver &observer) override;

    UdpWorker(const string &port) : queue(), socket(Socket::getAddrInfo(NULL, port.c_str()))
    {}

private:
    ClientMessage::SelfPacked buffer;
    std::queue<std::unique_ptr<IDatagram>> queue;
    Socket socket;
};


#endif //PROJECT_UDPWORKER_H
