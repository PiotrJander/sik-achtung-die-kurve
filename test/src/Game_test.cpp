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

//#define FAIL_ON_BAD_CAST(block) try { \
//    block; \
//} catch (std::bad_cast &e) { \
//    FAIL() << "Bad event type"; \
//}

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

TEST(GameTest, StartPixelEvents)
{
    TEST_DESCRIPTION(
        "NewGameEvent should be generated. "
        "PixelEvent should be generated for every player. "
        "Should return true. "
    );
    std::map<uint64_t, PlayerConnection> conns = {
            {654, PlayerConnection(654, 321, 1, "Stan")},
            {456, PlayerConnection(456, 123, -1, "Piotr")},
    };
    Random random(123);
    Game game(random, 6, 800, 600);
    game.addPlayers(conns);

    ASSERT_TRUE(game.start());

    try {
        auto &newGameEvent = dynamic_cast<NewGameEvent &>(*game.getEvents().at(0));
        ASSERT_EQ(newGameEvent, NewGameEvent(0, 800, 600));
        ASSERT_EQ(newGameEvent.playerNames.at(0), "Piotr");
        ASSERT_EQ(newGameEvent.playerNames.at(1), "Stan");
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }

    try {
        auto &pixelEvent = dynamic_cast<PixelEvent &>(*game.getEvents().at(1));
        ASSERT_EQ(pixelEvent, PixelEvent(1, 0, 123, 251));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }

    try {
        auto &pixelEvent = dynamic_cast<PixelEvent &>(*game.getEvents().at(2));
        ASSERT_EQ(pixelEvent, PixelEvent(2, 1, 261, 193));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }
}

TEST(GameTest, StartPlayerEliminated)
{
    TEST_DESCRIPTION(
            "When pixel already taken, player should be eliminated. "
            "When one player left, should generate GameOverEvent. "
    );
    std::map<uint64_t, PlayerConnection> conns = {
            {654, PlayerConnection(654, 321, 1, "Stan")},
            {456, PlayerConnection(456, 123, -1, "Piotr")},
    };
    Random random(123);
    Game game(random, 6, 2, 2);
    game.addPlayers(conns);

    ASSERT_FALSE(game.start());

    try {
        auto &pixelEvent = dynamic_cast<PixelEvent &>(*game.getEvents().at(1));
        ASSERT_EQ(pixelEvent, PixelEvent(1, 0, 1, 1));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }

    try {
        auto &playerEliminatedEvent = dynamic_cast<PlayerEliminatedEvent &>(*game.getEvents().at(2));
        ASSERT_EQ(playerEliminatedEvent, PlayerEliminatedEvent(2, 1));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }

    try {
        auto &gameOverEvent = dynamic_cast<GameOverEvent &>(*game.getEvents().at(3));
        ASSERT_EQ(gameOverEvent, GameOverEvent(3));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }
}

TEST(GameTest, TickGeneratesPixel)
{
    TEST_DESCRIPTION(
        "Players headings are updated. "
        "Should not generate events when player position doesn't change. "
        "Should generate a PixelEvent when a player position changes. "
        "Should return true if the game is going on."
    );
    std::map<uint64_t, PlayerConnection> conns = {
            {456, PlayerConnection(456, 123, -1, "Piotr")},
            {654, PlayerConnection(654, 321, 1, "Stan")},
    };
    Random random(123);
    Game game(random, 6, 10, 10);
    game.addPlayers(conns);

    game.players.at(0)
            .setCoordinates(CoordinateUnsignedLong(1, 1))
            .heading = 45 + 6;
    game.players.at(1)
            .setCoordinates(CoordinateUnsignedLong(5, 5))
            .heading = 45 + 180 - 6;

    ASSERT_TRUE(game.tick());

    ASSERT_EQ(game.getEvents().size(), 1);
    ASSERT_EQ(game.players.at(0).heading, 45);
    ASSERT_EQ(game.players.at(1).heading, 45 + 180);

    try {
        auto &pixelEvent = dynamic_cast<PixelEvent &>(*game.getEvents().at(0));
        ASSERT_EQ(pixelEvent, PixelEvent(0, 1, 4, 4));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }
}

TEST(GameTest, TickEliminatePlayerGameOver)
{
    TEST_DESCRIPTION(
        "Should eliminate the player who steps on a taken pixel. "
        "Should generate GameOverEvent when there is one player left. "
    );
    std::map<uint64_t, PlayerConnection> conns = {
            {456, PlayerConnection(456, 123, 0, "Piotr")},
            {654, PlayerConnection(654, 321, 0, "Stan")},
    };
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

    ASSERT_FALSE(game.tick());

    try {
        auto &playerEliminatedEvent = dynamic_cast<PlayerEliminatedEvent &>(*game.getEvents().at(1));
        ASSERT_EQ(playerEliminatedEvent, PlayerEliminatedEvent(1, 1));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }

    try {
        auto &gameOverEvent = dynamic_cast<GameOverEvent &>(*game.getEvents().at(2));
        ASSERT_EQ(gameOverEvent, GameOverEvent(2));
    } catch (std::bad_cast &e) {
        FAIL() << "Bad event type";
    }
}

#pragma clang diagnostic pop












