//
// Created by Piotr Jander on 17/08/2017.
//

#ifndef PROJECT_OUTPUTSTREAMWRAPPER_H
#define PROJECT_OUTPUTSTREAMWRAPPER_H

#include <ostream>

using std::ostream;
using std::streamsize;

class OutputStreamUtils {
public:
    template<typename T>
    static ostream &write_int(ostream &s, T n)
    {
        return s.write((char *) &n, sizeof(n));
    }
};


#endif //PROJECT_OUTPUTSTREAMWRAPPER_H
