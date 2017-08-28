//
// Created by Piotr Jander on 27/08/2017.
//

#ifndef PROJECT_UDPWORKER_H
#define PROJECT_UDPWORKER_H


#include <queue>
#include <netdb.h>
#include "IUdpWorker.h"
#include "Socket.h"
#include "GameManager.h"

class UdpWorker: public IUdpWorker {
public:
    void enqueue(std::unique_ptr<IDatagram> datagram) override;

    void getDatagram(IDatagramObserver &observer) override;

    void workUntil(std::chrono::milliseconds endOfFrame, IDatagramObserver &observer) override;

    UdpWorker(const string &port);

//    void setSocketToNonblocking()
//    {
//        socket.setNonBlocking();
//    }
//
//    void setSocketToBlocking()
//    {
//        socket.setBlocking();
//    }

private:
    ClientMessage::SelfPacked buffer;
    std::deque<std::unique_ptr<IDatagram>> queue;
    Socket socket;

    void getDatagramFromNonblockingSocket(IDatagramObserver &observer);

    void sendDatagramToNonblockingSocket(IDatagramObserver &observer);
};


#endif //PROJECT_UDPWORKER_H
