//
// Created by Piotr Jander on 28/08/2017.
//

#include <string>
#include "DynamicBuffer.h"

void DynamicBuffer::writeCharString(const std::string &str)
{
    std::copy(str.c_str(), str.c_str() + str.size(), std::back_inserter(buffer));
    char null_char = '\0';
    *this << null_char;
}
