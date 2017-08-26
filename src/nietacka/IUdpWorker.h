//
// Created by Piotr Jander on 26/08/2017.
//

#ifndef PROJECT_IUDPWORKER_H
#define PROJECT_IUDPWORKER_H


#include <memory>
#include "IDatagram.h"

class IUdpWorker {
public:
    virtual void enqueue(std::unique_ptr<IDatagram>) = 0;

    virtual std::unique_ptr<char[]> dequeue() = 0;

    virtual void workUntil(int time) = 0;
};


#endif //PROJECT_IUDPWORKER_H
