//
// Created by Piotr Jander on 29/08/2017.
//

#include <nietacka/DynamicBuffer.h>
#include <nietacka/ReadBuffer.h>
#include "gtest/gtest.h"


TEST(ReadBuffer, First)
{
    DynamicBuffer dynbuffer;
    uint32_t gameId = 123;
    uint32_t length = 456;
    std::string name1("Piotr");
    std::string name2("Jed");
    uint32_t checksum = 789;

    dynbuffer << gameId << length << name1 << name2 << checksum;

    ReadBuffer readBuffer(dynbuffer);
    uint32_t gameId_;
    uint32_t length_;
    uint32_t checksum_;

    readBuffer >> &gameId_ >> &length_;
    std::string name1_(readBuffer.readString());
    std::string name2_(readBuffer.readString());
    readBuffer >> &checksum_;

    EXPECT_EQ(gameId, gameId_);
    EXPECT_EQ(length, length_);
    EXPECT_EQ(name1, name1_);
    EXPECT_EQ(name2, name2_);
    EXPECT_EQ(checksum, checksum_);
}