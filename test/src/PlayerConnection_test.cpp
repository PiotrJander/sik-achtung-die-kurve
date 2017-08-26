//
// Created by Piotr Jander on 26/08/2017.
//

#include <netinet/in.h>
#include <nietacka/PlayerConnection.h>
#include "gtest/gtest.h"

TEST(PlayerConnectioTest, Hash)
{
    sockaddr_in ipv4;
    ipv4.sin_family = AF_INET;
    ipv4.sin_port = 1234;
    ipv4.sin_addr.s_addr = 987654;

    sockaddr_in6 ipv6;
    ipv6.sin6_family = AF_INET6;
    ipv6.sin6_port = 2345;
    uint8_t ipv6Address[16] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    memcpy((uint8_t *) ipv6.sin6_addr.__u6_addr.__u6_addr8, ipv6Address, 16);

    PlayerConnection pc1(reinterpret_cast<sockaddr *>(&ipv4), 123, -1, "Piotr");
    PlayerConnection pc2(reinterpret_cast<sockaddr *>(&ipv6), 123, -1, "Jed");

    ASSERT_EQ(pc1.hash(), 175173704325);
    ASSERT_EQ(pc2.hash(), 7801689049451974840);

}