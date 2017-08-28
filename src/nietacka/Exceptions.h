//
// Created by Piotr Jander on 27/08/2017.
//

#ifndef PROJECT_EXCEPTIONS_H
#define PROJECT_EXCEPTIONS_H

#include <stdexcept>
#include <cstring>

class SocketException: public std::runtime_error {
public:
    SocketException(int errorCode, const std::string &desc) : runtime_error(desc), errorCode(errorCode)
    {}

    SocketException(int errorCode) : runtime_error(strerror(errorCode)), errorCode(errorCode)
    {}

private:
    int errorCode;
};


class WouldBlockException: public SocketException {
public:
    WouldBlockException() : SocketException(EWOULDBLOCK)
    {}
};


class InterruptedException: public SocketException {
public:
    InterruptedException() : SocketException(EINTR)
    {}
};


class ProtocolException: public std::runtime_error {
public:
    ProtocolException(const std::string &desc) : runtime_error(desc)
    {}
};

#endif //PROJECT_EXCEPTIONS_H
