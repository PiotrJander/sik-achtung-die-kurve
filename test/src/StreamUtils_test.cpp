//
// Created by Piotr Jander on 17/08/2017.
//

#include "gtest/gtest.h"
#include "nietacka/StreamUtils.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ClangTidyInspection"


TEST(StreamUtilsTest, One)
{
    char buffer[sizeof(int8_t) + sizeof(uint32_t)];
    char *location = buffer;
    int8_t k = 54;
    uint32_t m = 93845;

    location = StreamUtils::write_int<int8_t>(location, k);
    StreamUtils::write_int<uint32_t>(location, m);

    location = buffer;
    auto k_pair = StreamUtils::read_int<int8_t>(location);
    auto m_pair = StreamUtils::read_int<uint32_t>(k_pair.second);

    EXPECT_EQ(k, k_pair.first);
    EXPECT_EQ(m, m_pair.first);
}

#pragma clang diagnostic pop