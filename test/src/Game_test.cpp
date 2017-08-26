//
// Created by Piotr Jander on 24/08/2017.
//

#include <nietacka/PlayerConnection.h>
#include <nietacka/Game.h>
#include <nietacka/event/NewGameEvent.h>
#include <nietacka/event/PixelEvent.h>
#include <nietacka/event/GameOverEvent.h>
#include <nietacka/event/PlayerEliminatedEvent.h>
#include "gtest/gtest.h"
#include "setup.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "CannotResolve"


class GameTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        ipv4_1.sin_family = AF_INET;
        ipv4_1.sin_port = 1234;
        ipv4_1.sin_addr.s_addr = 987654;

        ipv4_2.sin_family = AF_INET;
        ipv4_2.sin_port = 3456;
        ipv4_2.sin_addr.s_addr = 876543;

        ipv6.sin6_family = AF_INET6;
        ipv6.sin6_port = 2345;
        uint8_t ipv6Address[16] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        memcpy((uint8_t *) ipv6.sin6_addr.__u6_addr.__u6_addr8, ipv6Address, 16);

        PlayerConnection pc1(reinterpret_cast<sockaddr *>(&ipv4_1), 123, -1, "Piotr");
        PlayerConnection pc2(reinterpret_cast<sockaddr *>(&ipv6), 321, 1, "Stan");
        conns = {
                {pc1.hash(), pc1},
                {pc2.hash(), pc2}
        };
    }

    sockaddr_in ipv4_1;
    sockaddr_in ipv4_2;
    sockaddr_in6 ipv6;

    PlayerConnectionMap conns;
};

TEST_F(GameTest, PlayerGetters)
{
    TEST_DESCRIPTION("getName, getDirection");
    Game::Player player(175173704325, 0, 123, conns);
    ASSERT_EQ(player.getTurnDirection(), -1);
    ASSERT_EQ(player.getName(), "Piotr");
}

TEST_F(GameTest, PlayerCoordinates)
{
    TEST_DESCRIPTION("setCoordinates, getCoordinates");
    CoordinateLong ints(12, 23);
    Game::Player player(175173704325, 0, 456, conns);
    player.setCoordinates(ints);
    ASSERT_EQ(player.getCoordinates(), ints);
}

TEST_F(GameTest, PlayerSessionIdChanges)
{
    TEST_DESCRIPTION("When sessionId changes, should act as if the player disconnected");
    CoordinateLong ints(12, 23);
    Game::Player player(175173704325, 0, 456, conns);
    conns.at(175173704325).sessionId += 1;
    ASSERT_EQ(player.getTurnDirection(), 0);
    ASSERT_EQ(player.getName(), "");
}

TEST_F(GameTest, AddPlayers)
{
TEST_DESCRIPTION(
            "Empty names should be discarded. "
            "Players should be sorted by name. "
            "Player objects should be created. "
    );
    PlayerConnection pc1(reinterpret_cast<sockaddr *>(&ipv4_1), 321, 1, "Stan");
    PlayerConnection pc2(reinterpret_cast<sockaddr *>(&ipv4_2), 123, -1, "Piotr");
    PlayerConnection pc3(reinterpret_cast<sockaddr *>(&ipv6), 890, 0, "");
    PlayerConnectionMap conns2 = {
            {pc1.hash(), pc1},
            {pc2.hash(), pc2},
            {pc3.hash(), pc3}
    };
    Random random(123);
    Game game(random, 6, 800, 600);
    game.addPlayers(conns2);

    ASSERT_EQ(game.players.size(), 2);
    ASSERT_EQ(game.players.at(0).sessionId, 123);
    ASSERT_EQ(game.players.at(1).sessionId, 321);
    ASSERT_EQ(game.players.at(0).getName(), "Piotr");
}

TEST_F(GameTest, StartPixelEvents)
{
    TEST_DESCRIPTION(
        "NewGameEvent should be generated. "
        "PixelEvent should be generated for every player. "
        "Should return true. "
    );
    Random random(123);
    Game game(random, 6, 800, 600);
    game.addPlayers(conns);
    game.start();
    ASSERT_TRUE(game.isInProgress());

    try {
        auto &newGameEvent = dynamic_cast<NewGameEvent &>(*game.getEventHistory().at(0));
        ASSERT_EQ(newGameEvent, NewGameEvent(0, 800, 600));
        ASSERT_EQ(newGameEvent.getPlayerNames().at(0), "Piotr");
        ASSERT_EQ(newGameEvent.getPlayerNames().at(1), "Stan");
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }

    try {
        auto &pixelEvent = dynamic_cast<PixelEvent &>(*game.getEventHistory().at(1));
        ASSERT_EQ(pixelEvent, PixelEvent(1, 0, 123, 251));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }

    try {
        auto &pixelEvent = dynamic_cast<PixelEvent &>(*game.getEventHistory().at(2));
        ASSERT_EQ(pixelEvent, PixelEvent(2, 1, 261, 193));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }
}

TEST_F(GameTest, StartPlayerEliminated)
{
    TEST_DESCRIPTION(
            "When pixel already taken, player should be eliminated. "
            "When one player left, should generate GameOverEvent. "
    );
    Random random(123);
    Game game(random, 6, 2, 2);
    game.addPlayers(conns);
    game.start();
    ASSERT_FALSE(game.isInProgress());

    try {
        auto &pixelEvent = dynamic_cast<PixelEvent &>(*game.getEventHistory().at(1));
        ASSERT_EQ(pixelEvent, PixelEvent(1, 0, 1, 1));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }

    try {
        auto &playerEliminatedEvent = dynamic_cast<PlayerEliminatedEvent &>(*game.getEventHistory().at(2));
        ASSERT_EQ(playerEliminatedEvent, PlayerEliminatedEvent(2, 1));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }

    try {
        auto &gameOverEvent = dynamic_cast<GameOverEvent &>(*game.getEventHistory().at(3));
        ASSERT_EQ(gameOverEvent, GameOverEvent(3));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }
}

TEST_F(GameTest, TickGeneratesPixel)
{
    TEST_DESCRIPTION(
        "Players headings are updated. "
        "Should not generate events when player position doesn't change. "
        "Should generate a PixelEvent when a player position changes. "
        "Should return true if the game is going on."
    );
    Random random(123);
    Game game(random, 6, 10, 10);
    game.addPlayers(conns);

    game.players.at(0)
            .setCoordinates(CoordinateUnsignedLong(1, 1))
            .heading = 45 + 6;
    game.players.at(1)
            .setCoordinates(CoordinateUnsignedLong(5, 5))
            .heading = 45 + 180 - 6;

    game.tick();
    ASSERT_TRUE(game.isInProgress());

    ASSERT_EQ(game.getEventHistory().size(), 1);
    ASSERT_EQ(game.players.at(0).heading, 45);
    ASSERT_EQ(game.players.at(1).heading, 45 + 180);

    try {
        auto &pixelEvent = dynamic_cast<PixelEvent &>(*game.getEventHistory().at(0));
        ASSERT_EQ(pixelEvent, PixelEvent(0, 1, 4, 4));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }
}

TEST_F(GameTest, TickEliminatePlayerGameOver)
{
    TEST_DESCRIPTION(
        "Should eliminate the player who steps on a taken pixel. "
        "Should generate GameOverEvent when there is one player left. "
    );
    Random random(123);
    Game game(random, 6, 10, 10);
    game.addPlayers(conns);

    game.players.at(0)
            .setCoordinates(CoordinateUnsignedLong(1, 1))
            .heading = 0;
    game.players.at(1)
            .setCoordinates(CoordinateDouble(5.5, 5))
            .heading = 0;

    game.setPixel(CoordinateUnsignedLong(6, 5));

    game.tick();
    ASSERT_FALSE(game.isInProgress());

    try {
        auto &playerEliminatedEvent = dynamic_cast<PlayerEliminatedEvent &>(*game.getEventHistory().at(1));
        ASSERT_EQ(playerEliminatedEvent, PlayerEliminatedEvent(1, 1));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }

    try {
        auto &gameOverEvent = dynamic_cast<GameOverEvent &>(*game.getEventHistory().at(2));
        ASSERT_EQ(gameOverEvent, GameOverEvent(2));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }
}

#pragma clang diagnostic pop

