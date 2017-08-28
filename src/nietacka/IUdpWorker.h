//
// Created by Piotr Jander on 26/08/2017.
//

#ifndef PROJECT_IUDPWORKER_H
#define PROJECT_IUDPWORKER_H


#include <memory>
#include <chrono>
#include "IDatagram.h"
#include "IDatagramObserver.h"

class IUdpWorker {
public:

    virtual void enqueue(std::unique_ptr<IDatagram>) = 0;

    virtual void workUntil(std::chrono::milliseconds time, IDatagramObserver &observer) = 0;

    virtual void work(IDatagramObserver &observer) = 0;

private:
    std::unique_ptr<IDatagramObserver> observer;
};


#endif //PROJECT_IUDPWORKER_H
