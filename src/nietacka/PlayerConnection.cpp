//
// Created by Piotr Jander on 26/08/2017.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include "PlayerConnection.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"

template <class T>
inline void hash_combine(std::size_t & s, const T & v)
{
    std::hash<T> h;
    s^= h(v) + 0x9e3779b9 + (s<< 6) + (s>> 2);
}

//template<> struct hash<sockaddr_in>
//{
//    typedef struct sockaddr_in argument_type;
//    typedef std::size_t result_type;
//    result_type operator()(argument_type const& s) const
//    {
//        std::size_t res = 0;
//        hash_combine(res, s.sin_addr.s_addr);
//        hash_combine(res, s.sin_port);
//        return res;
//    }
//};
//
//template<> struct hash<sockaddr_in6>
//{
//    typedef struct sockaddr_in argument_type;
//    typedef std::size_t result_type;
//    result_type operator()(argument_type const& s) const
//    {
//        std::size_t res = 0;
//        hash_combine(res, s.sin_addr.s_addr);
//        hash_combine(res, s.sin_port);
//        return res;
//    }
//};

std::size_t PlayerConnection::getHashFor(sockaddr *structsockaddr)
{
    switch (structsockaddr->sa_family) {
        case AF_INET: {
            auto ipv4 = reinterpret_cast<sockaddr_in *>(structsockaddr);
            std::size_t res = 0;
            hash_combine(res, ipv4->sin_addr.s_addr);
            hash_combine(res, ipv4->sin_port);
            return res;
        }
        case AF_INET6: {
            auto ipv6 = reinterpret_cast<sockaddr_in6 *>(structsockaddr);
            std::size_t res = 0;
            uint8_t *addr = reinterpret_cast<uint8_t *>(ipv6->sin6_addr.__u6_addr.__u6_addr8);  // TODO different for linux here!
            for (int i = 0; i < 16; ++i) {
                hash_combine(res, addr[i]);
            }
            hash_combine(res, ipv6->sin6_port);
            return res;
        }
        default:
            // TODO handle
            return 0;
    }
}

#pragma clang diagnostic pop

