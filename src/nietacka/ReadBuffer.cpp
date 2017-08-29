//
// Created by Piotr Jander on 29/08/2017.
//

#include "ReadBuffer.h"

ReadBuffer::ReadBuffer(int length) : vector(length)
{}

ReadBuffer::ReadBuffer(const DynamicBuffer &dynamic) : vector(dynamic.getBuffer())
{}

std::string ReadBuffer::readString(int length)
{
    length = std::min(length, static_cast<const int &>(vector.size() - readLocation));
    std::string ret(&vector[readLocation], 0, static_cast<unsigned long>(length));
    readLocation += ret.size() + 1;
    return ret;
}

ReadBufferException::ReadBufferException(const std::string &desc) : runtime_error(desc)
{}
