//
// Created by Piotr Jander on 24/08/2017.
//

#ifndef PROJECT_RANDOM_H
#define PROJECT_RANDOM_H


#include <cstdint>

class Random {
public:
    Random(int64_t seed) : seed(seed)
    {}

    Random() : seed(time(NULL))
    {}

    uint32_t rand()
    {
        int64_t r = seed;
        seed = (seed * 279470273) % 4294967291;
        return (uint32_t) r;
    }

private:
    int64_t seed;
};


#endif //PROJECT_RANDOM_H
