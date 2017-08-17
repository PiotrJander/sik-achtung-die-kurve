//
// Created by Piotr Jander on 17/08/2017.
//

#include "gtest/gtest.h"
#include "nietacka/Foo.h"
using namespace std;

class DivisibleTest : public ::testing::Test {

};


TEST_F(DivisibleTest, FooCase) {
    Foo foo;
    EXPECT_EQ(3, foo.foo());
}
