//
// Created by Piotr Jander on 26/08/2017.
//

#ifndef PROJECT_IDATAGRAMOBSERVER_H
#define PROJECT_IDATAGRAMOBSERVER_H


class IDatagramObserver {
public:
    virtual void processDatagram(const char *buffer) = 0;
};


#endif //PROJECT_IDATAGRAMOBSERVER_H
