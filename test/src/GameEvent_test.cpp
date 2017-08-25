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
    std::stringstream s;

    std::vector<std::string> names {"Piotr", "John", "Jane"};
    NewGameEvent e1 {123, 800, 600, names};

    e1.writeTo(s);

    auto _e2 = GameEvent::readFrom(s);
    auto *e2 = (NewGameEvent *) _e2.get();

    ASSERT_EQ(e1.getEventNo(), e2->getEventNo());
    ASSERT_EQ(e1.getType(), e2->getType());
    ASSERT_EQ(e1.getMaxx(), e2->getMaxx());
    ASSERT_EQ(e1.getMaxy(), e2->getMaxy());
    ASSERT_EQ(e1.getPlayerNames().at(0), e2->getPlayerNames().at(0));
    ASSERT_EQ(e1.getPlayerNames().at(1), e2->getPlayerNames().at(1));
    ASSERT_EQ(e1.getPlayerNames().at(2), e2->getPlayerNames().at(2));
}

TEST(GameEventTest, PixelEvent)
{
    std::stringstream s;

    PixelEvent e1 {123, 4, 567, 890};
    e1.writeTo(s);
    
    auto _e2 = GameEvent::readFrom(s);
    auto *e2 = (PixelEvent *) _e2.get();

    ASSERT_EQ(e1.getEventNo(), e2->getEventNo());
    ASSERT_EQ(e1.getType(), e2->getType());
    ASSERT_EQ(e1.getPlayerNumber(), e2->getPlayerNumber());
    ASSERT_EQ(e1.getX(), e2->getX());
    ASSERT_EQ(e1.getY(), e2->getY());
}

TEST(GameEventTest, PlayerEliminatedEvent)
{
    std::stringstream s;

    PlayerEliminatedEvent e1 {123, 2};
    e1.writeTo(s);

    auto _e2 = GameEvent::readFrom(s);
    auto *e2 = (PlayerEliminatedEvent *) _e2.get();

    ASSERT_EQ(e1.getEventNo(), e2->getEventNo());
    ASSERT_EQ(e1.getType(), e2->getType());
    ASSERT_EQ(e1.getPlayerNumber(), e2->getPlayerNumber());
}

TEST(GameEventTest, GameOverEvent)
{
    std::stringstream s;

    GameOverEvent e1 {123};
    e1.writeTo(s);

    auto _e2 = GameEvent::readFrom(s);
    auto *e2 = (GameOverEvent *) _e2.get();

    ASSERT_EQ(e1.getEventNo(), e2->getEventNo());
    ASSERT_EQ(e1.getType(), e2->getType());
}
