//
// Created by Piotr Jander on 24/08/2017.
//

#include <nietacka/PlayerConnection.h>
#include <nietacka/Game.h>
#include "gtest/gtest.h"
#include "setup.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "CannotResolve"

TEST(GameTest, PlayerGetters)
{
    TEST_DESCRIPTION("getName, getDirection");
    std::map<uint64_t, PlayerConnection> conns = {
            {456, PlayerConnection(456, 123, -1, "Piotr")},
            {654, PlayerConnection(654, 321, 1, "Stan")}
    };
    Game::Player player(0, 456, conns);
    ASSERT_EQ(player.getTurnDirection(), -1);
    ASSERT_EQ(player.getName(), "Piotr");
}

TEST(GameTest, PlayerCoordinates)
{
    TEST_DESCRIPTION("setCoordinates, getCoordinates");
    CoordinateLong ints(12, 23);
    std::map<uint64_t, PlayerConnection> conns;
    Game::Player player(0, 456, conns);
    player.setCoordinates(ints);
    ASSERT_EQ(player.getCoordinates(), ints);
}

TEST(GameTest, AddPlayers)
{
    TEST_DESCRIPTION(
            "Empty names should be discarded. "
            "Players should be sorted by name. "
            "Player objects should be created. "
    );
    std::map<uint64_t, PlayerConnection> conns = {
            {654, PlayerConnection(654, 321, 1, "Stan")},
            {456, PlayerConnection(456, 123, -1, "Piotr")},
            {123, PlayerConnection(123, 890, 0, "")}
    };
    Random random(123);
    Game game(random, 6, 800, 600);
    game.addPlayers(conns);

    ASSERT_EQ(game.players.size(), 2);
    ASSERT_EQ(game.players.at(0).sessionId, 456);
    ASSERT_EQ(game.players.at(1).sessionId, 654);
    ASSERT_EQ(game.players.at(0).getName(), "Piotr");
}

#pragma clang diagnostic pop












