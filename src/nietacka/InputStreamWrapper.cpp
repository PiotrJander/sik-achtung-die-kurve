//
// Created by Piotr Jander on 17/08/2017.
//

#include "InputStreamWrapper.h"

istream &InputStreamWrapper::read(char *buffer, streamsize n)
{
    return s.read(buffer, n);
}

InputStreamWrapper::InputStreamWrapper(istream &s) : s(s)
{}


//int8_t InputStreamWrapper::read_int8()
//{
//    int8_t ret;
//    s.read((char *) &ret, sizeof(ret));
//    return ret;
//}
//
//uint32_t InputStreamWrapper::read_uint32()
//{
//    uint32_t ret;
//    s.read((char *) &ret, sizeof(ret));
//    return ret;
//}
//
//uint64_t InputStreamWrapper::read_uint64()
//{
//    uint64_t ret;
//    s.read((char *) &ret, sizeof(ret));
//    return ret;
//}
