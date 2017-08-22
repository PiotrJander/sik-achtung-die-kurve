//
// Created by Piotr Jander on 17/08/2017.
//

#include <sstream>

#include "gtest/gtest.h"
#include "nietacka/InputStreamUtils.h"
#include "nietacka/OutputStreamUtils.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ClangTidyInspection"

class StreamWrappersTest : public ::testing::Test {

};


TEST_F(StreamWrappersTest, One)
{
    std::stringstream s;

    int8_t k = 54;
    uint32_t m = 93845;
    char str[4] = "foo";

    OutputStreamUtils::write_int<int8_t>(s, k);
    OutputStreamUtils::write_int<uint32_t>(s, m);
    s.write(str, sizeof(str));

    auto k_ = InputStreamUtils::read_int<int8_t>(s);
    auto m_ = InputStreamUtils::read_int<uint32_t>(s);
    char str_[4];
    s.read(str_, sizeof(str_));

    EXPECT_EQ(k, k_);
    EXPECT_EQ(m, m_);
    EXPECT_TRUE(strcmp(str, str_) == 0);
}

#pragma clang diagnostic pop