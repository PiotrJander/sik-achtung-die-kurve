//
// Created by Piotr Jander on 17/08/2017.
//

#ifndef PROJECT_INPUTSTREAMWRAPPER_H
#define PROJECT_INPUTSTREAMWRAPPER_H

#include <istream>

using std::istream;
using std::streamsize;

class InputStreamWrapper {
public:
    explicit InputStreamWrapper(istream &s);

    istream &read(char *buffer, streamsize n);

    template<typename T>
    T read_int()
    {
        T ret;
        s.read((char *) &ret, sizeof(ret));
        return ret;
    }

//    int8_t read_int8();
//
//    uint32_t read_uint32();
//
//    uint64_t read_uint64();

private:
    std::istream &s;
};


#endif //PROJECT_INPUTSTREAMWRAPPER_H
