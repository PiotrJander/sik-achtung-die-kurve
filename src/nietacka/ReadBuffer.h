//
// Created by Piotr Jander on 29/08/2017.
//

#ifndef PROJECT_READBUFFER_H
#define PROJECT_READBUFFER_H


#include <vector>
#include <string>

class ReadBuffer {
public:
    ReadBuffer();

    template<typename T>
    ReadBuffer &operator<<(T *value)
    {
        if (readLocation + sizeof(T) > vector.size()) {
            throw std::runtime_error("Trying to read past the buffer");
        }
        *value = reinterpret_cast<T *>(&vector[readLocation]);
        readLocation += sizeof(T);
        return *this;
    }

    const char *peek()
    {
        return &vector[readLocation];
    }

    std::string readString()
    {
        std::string ret(vector.begin() + readLocation, vector.end());
        readLocation += ret.size() + 1;  // TODO test
        return ret;
    }

private:
    std::vector<char> vector;
    int readLocation = 0;
};


#endif //PROJECT_READBUFFER_H
