//
// Created by Piotr Jander on 26/08/2017.
//

#ifndef PROJECT_IDATAGRAM_H
#define PROJECT_IDATAGRAM_H


#include <memory>

class IDatagram {
public:
    virtual std::unique_ptr<char[]> getBuffer() = 0;

    virtual int getLength() = 0;

    virtual std::size_t getSocketHash = 0;
};


#endif //PROJECT_IDATAGRAM_H
