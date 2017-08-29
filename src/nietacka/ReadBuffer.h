//
// Created by Piotr Jander on 29/08/2017.
//

#ifndef PROJECT_READBUFFER_H
#define PROJECT_READBUFFER_H


#include <vector>
#include <string>
#include "DynamicBuffer.h"


class ReadBufferException: public std::runtime_error {
public:
    ReadBufferException(const std::string &desc);
};

class ReadBuffer {
public:
    ReadBuffer(int length);

    ReadBuffer(const DynamicBuffer &dynamic);

    template<typename T>
    ReadBuffer &operator>>(T &value)
    {
        if (readLocation + sizeof(T) > vector.size()) {
            throw ReadBufferException("Trying to read past the buffer");
        }
        value = *reinterpret_cast<T *>(&vector[readLocation]);
        readLocation += sizeof(T);
        return *this;
    }
    
    template<typename T>
    const T *castAndAdvance()
    {
        if (readLocation + sizeof(T) > vector.size()) {
            throw ReadBufferException("Trying to read past the buffer");
        }
        const T *ptr = reinterpret_cast<const T *>(&vector[readLocation]);
        readLocation += sizeof(T);
        return ptr;
    }

    const char *peek()
    {
        return &vector[readLocation];
    }

    char *getWriteable()
    {
        readLocation = 0;
        return &vector[0];
    }

    std::string readString(int length);

    int length() const
    {
        return static_cast<int>(vector.size());
    }

private:
    std::vector<char> vector;
    int readLocation = 0;
};


#endif //PROJECT_READBUFFER_H
