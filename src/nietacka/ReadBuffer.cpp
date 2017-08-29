//
// Created by Piotr Jander on 29/08/2017.
//

#include "ReadBuffer.h"
#include "DynamicBuffer.h"

ReadBuffer::ReadBuffer(int length) : vector(length)
{}

ReadBuffer::ReadBuffer(const DynamicBuffer &dynamic) : vector(dynamic.getBuffer())
{}

std::string ReadBuffer::readString(int length)
{
    std::string ret(&vector[readLocation], 0, static_cast<unsigned long>(length));
    readLocation += ret.size() + 1;
    return ret;
}
