//
// Created by Piotr Jander on 17/08/2017.
//

#include "OutputStreamWrapper.h"

ostream &OutputStreamWrapper::write(const char *buffer, streamsize n)
{
    return s.write(buffer, n);
}

//void OutputStreamWrapper::write_int8(int8_t n)
//{
//    s.write((char *) &n, sizeof(n));
//}
//
//void OutputStreamWrapper::write_uint32(uint32_t n)
//{
//    s.write((char *) &n, sizeof(n));
//}
//
//void OutputStreamWrapper::write_uint64(uint64_t n)
//{
//    s.write((char *) &n, sizeof(n));
//}

OutputStreamWrapper::OutputStreamWrapper(ostream &s) : s(s)
{}

