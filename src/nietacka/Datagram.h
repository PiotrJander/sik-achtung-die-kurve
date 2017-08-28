//
// Created by Piotr Jander on 28/08/2017.
//

#ifndef PROJECT_DATAGRAM_H
#define PROJECT_DATAGRAM_H

#include "EventBatch.h"

class Datagram: public IDatagram {
public:
    Datagram(const EventBatch &eventBatch, const sockaddr_storage &sockaddrStorage)
            : eventBatch(eventBatch), sockaddrStorage(sockaddrStorage)
    {}

    std::unique_ptr<char[]> getBuffer() override
    {
        return eventBatch.getBuffer();
    }

    int getLength() override
    {
        return eventBatch.getLength();
    }

    const sockaddr *getSockAddr() override
    {
        return reinterpret_cast<const sockaddr *>(&sockaddrStorage);
    }

    const EventBatch &getEventBatch() const
    {
        return eventBatch;
    }

private:
    EventBatch eventBatch;
    sockaddr_storage sockaddrStorage;
};


#endif //PROJECT_DATAGRAM_H