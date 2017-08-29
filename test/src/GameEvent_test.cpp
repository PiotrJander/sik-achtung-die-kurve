//
// Created by Piotr Jander on 22/08/2017.
//

#include <sstream>
#include <nietacka/event/PixelEvent.h>
#include <nietacka/event/PlayerEliminatedEvent.h>
#include <nietacka/event/GameOverEvent.h>
#include <nietacka/event/NewGameEvent.h>

#include "gtest/gtest.h"

TEST(GameEventTest, NewGameEvent)
{
    std::vector<std::string> names {"Piotr", "John", "Jane"};
    NewGameEvent e1 {123, 800, 600, names};

    DynamicBuffer buffer;
    e1.write(buffer);

    ReadBuffer readBuffer(buffer);

    auto _e2 = GameEvent::read(readBuffer);
    auto *e2 = (NewGameEvent *) _e2.get();

    EXPECT_EQ(e1.getEventNo(), e2->getEventNo());
    EXPECT_EQ(e1.getType(), e2->getType());
    EXPECT_EQ(e1.getMaxx(), e2->getMaxx());
    EXPECT_EQ(e1.getMaxy(), e2->getMaxy());
    EXPECT_EQ(e1.getPlayerNames().at(0), e2->getPlayerNames().at(0));
    EXPECT_EQ(e1.getPlayerNames().at(1), e2->getPlayerNames().at(1));
    EXPECT_EQ(e1.getPlayerNames().at(2), e2->getPlayerNames().at(2));
}

TEST(GameEventTest, PixelEvent)
{
    PixelEvent e1 {123, 4, 567, 890};

    DynamicBuffer buffer;
    e1.write(buffer);

    ReadBuffer readBuffer(buffer);

    auto _e2 = GameEvent::read(readBuffer);
    auto *e2 = (PixelEvent *) _e2.get();

    EXPECT_EQ(e1.getEventNo(), e2->getEventNo());
    EXPECT_EQ(e1.getType(), e2->getType());
    EXPECT_EQ(e1.getPlayerNumber(), e2->getPlayerNumber());
    EXPECT_EQ(e1.getX(), e2->getX());
    EXPECT_EQ(e1.getY(), e2->getY());
}

TEST(GameEventTest, PlayerEliminatedEvent)
{
    PlayerEliminatedEvent e1 {123, 2};

    DynamicBuffer buffer;
    e1.write(buffer);

    ReadBuffer readBuffer(buffer);

    auto _e2 = GameEvent::read(readBuffer);
    auto *e2 = (PlayerEliminatedEvent *) _e2.get();

    EXPECT_EQ(e1.getEventNo(), e2->getEventNo());
    EXPECT_EQ(e1.getType(), e2->getType());
    EXPECT_EQ(e1.getPlayerNumber(), e2->getPlayerNumber());
}

TEST(GameEventTest, GameOverEvent)
{
    GameOverEvent e1 {123};

    DynamicBuffer buffer;
    e1.write(buffer);

    ReadBuffer readBuffer(buffer);

    auto _e2 = GameEvent::read(readBuffer);
    auto *e2 = (GameOverEvent *) _e2.get();

    EXPECT_EQ(e1.getEventNo(), e2->getEventNo());
    EXPECT_EQ(e1.getType(), e2->getType());
}
