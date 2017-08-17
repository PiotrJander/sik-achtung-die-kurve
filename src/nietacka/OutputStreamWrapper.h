//
// Created by Piotr Jander on 17/08/2017.
//

#ifndef PROJECT_OUTPUTSTREAMWRAPPER_H
#define PROJECT_OUTPUTSTREAMWRAPPER_H

#include <ostream>

using std::ostream;
using std::streamsize;

class OutputStreamWrapper {
public:
    explicit OutputStreamWrapper(ostream &s);

    ostream &write(const char *s, streamsize n);

    template<typename T>
    ostream &write_int(T n)
    {
        return s.write((char *) &n, sizeof(n));
    }

//    void write_int8(int8_t);
//
//    void write_uint32(uint32_t);
//
//    void write_uint64(uint64_t);

private:
    ostream &s;
};


#endif //PROJECT_OUTPUTSTREAMWRAPPER_H
