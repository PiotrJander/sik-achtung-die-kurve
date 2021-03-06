//
// Created by Piotr Jander on 28/08/2017.
//

#ifndef PROJECT_DYNAMICBUFFER_H
#define PROJECT_DYNAMICBUFFER_H


#include <vector>
#include <string>
#include <gtest/gtest_prod.h>

class DynamicBuffer {
public:
    FRIEND_TEST(DynamicBufferTest, CharString);

    DynamicBuffer() : buffer()
    {}

    template<typename T>
    DynamicBuffer& operator<<(const T& obj)
    {
        const char *start = reinterpret_cast<const char *>(&obj);
        const char *end = start + sizeof(T);
        std::copy(start, end, std::back_inserter(buffer));
        return *this;
    }

    DynamicBuffer& operator<<(const std::string& s)
    {
        std::copy(s.begin(), s.end(), std::back_inserter(buffer));
        buffer.push_back('\0');
        return *this;
    }

    const char *getStartPointer()
    {
        return &buffer[0];
    }

    unsigned long size()
    {
        return buffer.size();
    }

    const char *currentLocation()
    {
        return getStartPointer() + size();
    }

    void writeCharString(const std::string &str);

    const std::vector<char> &getBuffer() const
    {
        return buffer;
    };

private:
    std::vector<char> buffer;
};


#endif //PROJECT_DYNAMICBUFFER_H
