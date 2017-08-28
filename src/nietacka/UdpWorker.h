//
// Created by Piotr Jander on 27/08/2017.
//

#ifndef PROJECT_UDPWORKER_H
#define PROJECT_UDPWORKER_H


#include <queue>
#include <chrono>
#include <netdb.h>
#include "IUdpWorker.h"
#include "Socket.h"
#include "GameManager.h"

class UdpWorker: public IUdpWorker {
public:
    void enqueue(std::unique_ptr<IDatagram> datagram) override;

    void workUntil(std::chrono::milliseconds endOfFrame, IDatagramObserver &observer) override;

    void work(IDatagramObserver &observer) override;

    UdpWorker(const string &port);

private:
    ClientMessage::SelfPacked buffer;
    std::deque<std::unique_ptr<IDatagram>> queue;
    Socket socket;

    void actOnSocket(short pollResult, IDatagramObserver &observer);

    void sendDatagram();

    void receiveDatagram(IDatagramObserver &observer);
};


#endif //PROJECT_UDPWORKER_H
