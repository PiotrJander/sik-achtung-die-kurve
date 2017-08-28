//
// Created by Piotr Jander on 28/08/2017.
//

#include <nietacka/DynamicBuffer.h>
#include <nietacka/event/PixelEvent.h>
#include "gtest/gtest.h"

TEST(DynamicBufferTest, Simple)
{
    DynamicBuffer buffer;
    uint32_t foo;
    PixelEvent pe(123, 4, 56, 78);
    PixelEvent::SelfPacked packed(pe);
    buffer << foo << packed;
    EXPECT_EQ(buffer.size(), sizeof(uint32_t) + sizeof(PixelEvent::SelfPacked));
}

TEST(DynamicBufferTest, CharString)
{
    DynamicBuffer buffer;
    std::string str1("Piotr");
    std::string str2("Jed");
    buffer.writeCharString(str1);
    buffer.writeCharString(str2);
    unsigned long expectedSize = str1.size() + str2.size() + 2;
    std::string str1_(&buffer.buffer[0], &buffer.buffer[str1.size()]);
    std::string str2_(&buffer.buffer[str1.size() + 1], &buffer.buffer[expectedSize - 1]);
    EXPECT_EQ(buffer.size(), expectedSize);
    EXPECT_EQ(str1, str1_);
    EXPECT_EQ(str2, str2_);
}