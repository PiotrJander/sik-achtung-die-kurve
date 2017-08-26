//
// Created by Piotr Jander on 17/08/2017.
//

#ifndef PROJECT_INPUTSTREAMWRAPPER_H
#define PROJECT_INPUTSTREAMWRAPPER_H

#include <istream>

using std::istream;
using std::ostream;
using std::streamsize;

class StreamUtils {
public:
    template<typename T>
    static std::pair<T, char *> read_int(char *buffer)
    {
        T res = *reinterpret_cast<T *>(buffer);
        return std::make_pair(res, buffer + sizeof(T));
    }

    template<typename T>
    static char * write_int(char *buffer, T n)
    {
        *reinterpret_cast<T *>(buffer) = n;
        return buffer + sizeof(T);
    }
};


#endif //PROJECT_INPUTSTREAMWRAPPER_H
