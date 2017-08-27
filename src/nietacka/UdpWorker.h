//
// Created by Piotr Jander on 27/08/2017.
//

#ifndef PROJECT_UDPWORKER_H
#define PROJECT_UDPWORKER_H


#include <queue>
#include "IUdpWorker.h"

class SocketException: public std::runtime_error {
public:
    SocketException(int errorCode, const string &desc) : runtime_error(desc), errorCode(errorCode)
    {}

    SocketException(int errorCode) : runtime_error(strerror(errorCode)), errorCode(errorCode)
    {}

private:
    int errorCode;
};


class UdpWorker: public IUdpWorker {
public:
    void enqueue(const IDatagram &datagram) override;

    std::pair<const ClientMessage::SelfPacked *, const sockaddr *> getDatagram() override;

    void workUntil(std::chrono::milliseconds time, IDatagramObserver &observer) override;

    UdpWorker(uint16_t port, const std::queue &queue);

    virtual ~UdpWorker();

private:
    int socket_fd;
    std::queue queue;
};


#endif //PROJECT_UDPWORKER_H
