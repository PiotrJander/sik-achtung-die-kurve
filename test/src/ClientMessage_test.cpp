//
// Created by Piotr Jander on 19/08/2017.
//

#include <sstream>
#include "gtest/gtest.h"
#include <nietacka/ClientMessage.h>


TEST(ClientMessageTest, First)
{
    std::stringstream stream;

    ClientMessage cm1(123, 1, 456, "PiotrJander");
    ClientMessage::SelfPacked packedCm1(cm1);
    ClientMessage::SelfPacked packedCm2;

    stream.write(reinterpret_cast<const char *>(&packedCm1), sizeof(ClientMessage::SelfPacked));
    stream.read(reinterpret_cast<char *>(&packedCm2), sizeof(ClientMessage::SelfPacked));

    ClientMessage cm2(packedCm2);

    EXPECT_EQ(cm1.getSessionId(), cm2.getSessionId());
    EXPECT_EQ(cm1.getTurnDirection(), cm2.getTurnDirection());
    EXPECT_EQ(cm1.getNextExpectedEventNo(), cm2.getNextExpectedEventNo());
    EXPECT_EQ(cm1.getPlayerName(), cm2.getPlayerName());
}

TEST(ClientMessageTest, ThrowsErrorOnInvalidName)
{
    ClientMessage cm1(123, 1, 456, "Piotr Jander");
    ClientMessage::SelfPacked packed(cm1);
    
    ASSERT_THROW({
         ClientMessage cm2(packed); 
    }, ProtocolException);
}

TEST(ClientMessageTest, ShortName)
{
    ClientMessage cm1(123, 1, 456, "Piotr Jander");  // name has 13
    ClientMessage::SelfPacked packed(cm1);
    
    ClientMessage cm2(*reinterpret_cast<ClientMessage::SelfPackedNoName *>(&packed));
    cm2.setPlayerName(reinterpret_cast<const char *>(&packed) + sizeof(ClientMessage::SelfPackedNoName), 13);

    EXPECT_EQ(cm1.getSessionId(), cm2.getSessionId());
    EXPECT_EQ(cm1.getTurnDirection(), cm2.getTurnDirection());
    EXPECT_EQ(cm1.getNextExpectedEventNo(), cm2.getNextExpectedEventNo());
    EXPECT_EQ(cm1.getPlayerName(), cm2.getPlayerName());
}