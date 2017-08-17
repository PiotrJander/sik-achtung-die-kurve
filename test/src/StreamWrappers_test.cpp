//
// Created by Piotr Jander on 17/08/2017.
//

#include <sstream>

#include "gtest/gtest.h"
#include "nietacka/InputStreamWrapper.h"
#include "nietacka/OutputStreamWrapper.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ClangTidyInspection"

class StreamWrappersTest : public ::testing::Test {

};


TEST_F(StreamWrappersTest, One) {
    std::stringstream s;
    InputStreamWrapper isw(s);
    OutputStreamWrapper osw(s);

    int8_t k = 54;
    uint32_t m = 93845;
    char str[4] = "foo";

    osw.write_int<int8_t>(k);
    osw.write_int<uint32_t>(m);
    osw.write(str, sizeof(str));

    auto k_ = isw.read_int<int8_t>();
    auto m_ = isw.read_int<uint32_t>();
    char str_[4];
    isw.read(str_, sizeof(str_));

    EXPECT_EQ(k, k_);
    EXPECT_EQ(m, m_);
    EXPECT_TRUE(strcmp(str, str_) == 0);
}
#pragma clang diagnostic pop