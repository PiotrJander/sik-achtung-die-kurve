//
// Created by Piotr Jander on 19/08/2017.
//

#include "gtest/gtest.h"

#include "nietacka/MockSocket.h"

TEST(MockSocket__Test, First)
{
    MockSocket ms;
    char writeBuffer[] = "foo";
    char readBuffer[4];
    ms.clientWrite()->write(writeBuffer, 4);
    ms.serverRead()->read(readBuffer, 4);
    ASSERT_EQ(0, strncmp(readBuffer, writeBuffer, 4));
}