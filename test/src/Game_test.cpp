//
// Created by Piotr Jander on 24/08/2017.
//

#include "gtest/gtest.h"

TEST(GameTest, One)
{
    std::pair<double, double> ds(1.2, 2.3);
    std::pair<uint32_t, uint32_t> is(ds);
    ASSERT_EQ(1, is.first);
    ASSERT_EQ(2, is.second);
}