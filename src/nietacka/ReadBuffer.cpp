//
// Created by Piotr Jander on 29/08/2017.
//

#include "ReadBuffer.h"
#include "DynamicBuffer.h"

ReadBuffer::ReadBuffer(int length) : vector(length)
{}

ReadBuffer::ReadBuffer(const DynamicBuffer &dynamic) : vector(dynamic.getBuffer())
{}

std::string ReadBuffer::readString()
{
//    std::string ret(vector.begin() + readLocation, vector.end());
    std::string ret(&vector[readLocation], 0, vector.size() - readLocation);
    readLocation += ret.size() + 1;  // TODO test
    return ret;
}
