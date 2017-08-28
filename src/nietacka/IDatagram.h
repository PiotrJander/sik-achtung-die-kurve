//
// Created by Piotr Jander on 26/08/2017.
//

#ifndef PROJECT_IDATAGRAM_H
#define PROJECT_IDATAGRAM_H


#include <memory>
#include <sys/socket.h>
#include "DynamicBuffer.h"

class IDatagram {
public:
    virtual DynamicBuffer getBuffer() = 0;

    virtual const sockaddr * getSockAddr() = 0;

    virtual ~IDatagram()
    {}
};


#endif //PROJECT_IDATAGRAM_H
