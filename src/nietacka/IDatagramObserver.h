//
// Created by Piotr Jander on 26/08/2017.
//

#ifndef PROJECT_IDATAGRAMOBSERVER_H
#define PROJECT_IDATAGRAMOBSERVER_H


#include "ClientMessage.h"
#include "Game.h"

class IDatagramObserver {
public:
    virtual void processDatagram(const ClientMessage::SelfPacked *buffer, ssize_t length, const sockaddr *socket) = 0;

    virtual ~IDatagramObserver()
    {}
};


#endif //PROJECT_IDATAGRAMOBSERVER_H
