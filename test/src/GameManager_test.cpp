//
// Created by Piotr Jander on 27/08/2017.
//

#include <netinet/in.h>
#include <nietacka/GameManager.h>
#include "gtest/gtest.h"

/**
 * processDatagram
        should respond with events from next expected

    updateConnectedPlayers
        if new socket
            should add player
            unless 42 players already
            unless name already taken

        if exising socket
            if same session, should update turn and next expected
            if greater session, should replace
            if smaller, should ignore

    canGameStart
        positive and negative case

    enqueueNewDatagramBatches
        should pack all events in one datagram
        should split into many if greater than 521

    isPlayerNameTaken
        false for empty
        positive and negative case
 */
class GameManagerTest: public ::testing::Test {
public:
    GameManagerTest() : gameManager(600, 800, 12345, 50, 6, 6789)
    {
        ipv4_1_.sin_family = AF_INET;
        ipv4_1_.sin_port = 1234;
        ipv4_1_.sin_addr.s_addr = 987654;

        ipv4_2_.sin_family = AF_INET;
        ipv4_2_.sin_port = 12345;
        ipv4_2_.sin_addr.s_addr = 987654;

        ipv4_3_.sin_family = AF_INET;
        ipv4_3_.sin_port = 12346;
        ipv4_3_.sin_addr.s_addr = 987654;

        ipv4_1 = reinterpret_cast<sockaddr *>(&ipv4_1_);
        ipv4_2 = reinterpret_cast<sockaddr *>(&ipv4_2_);
        ipv4_3 = reinterpret_cast<sockaddr *>(&ipv4_3_);
    }
    sockaddr_in ipv4_1_;
    sockaddr_in ipv4_2_;
    sockaddr_in ipv4_3_;

    GameManager gameManager;
    
    sockaddr *ipv4_1;
    sockaddr *ipv4_2;
    sockaddr *ipv4_3;
};

TEST_F(GameManagerTest, UpdateConnectedPlayers)
{
    gameManager.connectedPlayers.insert(std::make_pair(
            456,
            PlayerConnection(ipv4_1, 890, 0, "Piotr")));

    // should add player
    ClientMessage message(345, 1, 0, "Jed");
    gameManager.updateConnectedPlayers(message, ipv4_2);
    EXPECT_NE(gameManager.connectedPlayers.find(175173711646), gameManager.connectedPlayers.end());

    // unless name already taken
    ClientMessage message2(345, 1, 0, "Piotr");
    gameManager.updateConnectedPlayers(message2, ipv4_3);
    EXPECT_EQ(gameManager.connectedPlayers.find(175173711645), gameManager.connectedPlayers.end());

    // NOW using exising socket

    // if same session, should update turn and next expected
    ClientMessage message3(345, -1, 5, "Jed");
    gameManager.updateConnectedPlayers(message3, ipv4_2);
    PlayerConnection &jed = gameManager.connectedPlayers.at(175173711646);
    EXPECT_EQ(jed.getTurnDirection(), -1);
    EXPECT_EQ(jed.getNextExpectedEvent(), 5);

    // if greater session, should replace
    ClientMessage message4(345 + 1, 0, 0, "Rish");
    gameManager.updateConnectedPlayers(message4, ipv4_2);
    PlayerConnection &rish = gameManager.connectedPlayers.at(175173711646);
    EXPECT_EQ(rish.getSessionId(), 345 + 1);
    EXPECT_EQ(rish.getName(), "Rish");
}

TEST_F(GameManagerTest, IsPlayerNameTaken)
{
    gameManager.connectedPlayers.insert(std::make_pair(
            456,
            PlayerConnection(ipv4_1, 890, 0, "Piotr")));
    EXPECT_FALSE(gameManager.isPlayerNameTaken(""));
    EXPECT_FALSE(gameManager.isPlayerNameTaken("Jed"));
    EXPECT_TRUE(gameManager.isPlayerNameTaken("Piotr"));
}

TEST_F(GameManagerTest, CanGameStart)
{
    // positive
    gameManager.connectedPlayers.insert(std::make_pair(
            456,
            PlayerConnection(ipv4_1, 890, 0, "Piotr")));
    gameManager.connectedPlayers.insert(std::make_pair(
            4567,
            PlayerConnection(ipv4_2, 890, 0, "")));
    gameManager.connectedPlayers.at(456).readyForGame = true;
    EXPECT_TRUE(gameManager.canGameStart());

    // negative
    gameManager.connectedPlayers.insert(std::make_pair(
            678,
            PlayerConnection(ipv4_1, 890, 0, "Piotr")));
    EXPECT_FALSE(gameManager.canGameStart());
}







