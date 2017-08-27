//
// Created by Piotr Jander on 27/08/2017.
//

#ifndef PROJECT_UDPWORKER_H
#define PROJECT_UDPWORKER_H


#include <queue>
#include "IUdpWorker.h"

class UdpWorker: public IUdpWorker {
public:
    void enqueue(std::unique_ptr<IDatagram> datagram) override;

    std::pair<const ClientMessage::SelfPacked *, const sockaddr *> getDatagram() override;

    void workUntil(std::chrono::milliseconds time, IDatagramObserver &observer) override;

    UdpWorker(uint16_t port);

    virtual ~UdpWorker();

private:
    int socket_fd;
    ClientMessage::SelfPacked buffer;
    sockaddr_storage sockaddrStorage;
    socklen_t storageSize = sizeof(sockaddr_storage);
    std::queue<std::unique_ptr<IDatagram>> queue;

    sockaddr *getSockaddr()
    {
        return reinterpret_cast<sockaddr *>(&sockaddrStorage);
    }
};


#endif //PROJECT_UDPWORKER_H
