//
// Created by Piotr Jander on 17/08/2017.
//

#ifndef PROJECT_INPUTSTREAMWRAPPER_H
#define PROJECT_INPUTSTREAMWRAPPER_H

#include <istream>

using std::istream;
using std::streamsize;

class InputStreamUtils {
public:
    template<typename T>
    static T read_int(istream &s)
    {
        T ret;
        s.read((char *) &ret, sizeof(ret));
        return ret;
    }
};


#endif //PROJECT_INPUTSTREAMWRAPPER_H
