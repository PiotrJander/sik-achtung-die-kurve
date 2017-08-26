//
// Created by Piotr Jander on 26/08/2017.
//

#ifndef PROJECT_IUDPWORKER_H
#define PROJECT_IUDPWORKER_H


#include <memory>
#include "IDatagram.h"
#include "IDatagramObserver.h"

class IUdpWorker {
public:
    virtual void enqueue(std::unique_ptr<IDatagram>) = 0;

    virtual const char *getDatagram() = 0;

    virtual void workUntil(std::chrono::milliseconds time) = 0;

private:
    IDatagramObserver observer;
};


#endif //PROJECT_IUDPWORKER_H
