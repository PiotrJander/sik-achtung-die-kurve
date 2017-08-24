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

    NewGameEvent e1 {123, 800, 600};
    e1.playerNames->emplace_back("Piotr");
    e1.playerNames->emplace_back("John");
    e1.playerNames->emplace_back("Jane");

    e1.writeTo(s);

    auto _e2 = GameEvent::readFrom(s);
    auto *e2 = (NewGameEvent *) _e2.get();

    ASSERT_EQ(e1.header.getEventNo(), e2->header.getEventNo());
    ASSERT_EQ(e1.header.getType(), e2->header.getType());
    ASSERT_EQ(e1.data.getMaxx(), e2->data.getMaxx());
    ASSERT_EQ(e1.data.getMaxy(), e2->data.getMaxy());
    ASSERT_EQ(e1.playerNames.get()[0], e2->playerNames.get()[0]);
    ASSERT_EQ(e1.playerNames.get()[1], e2->playerNames.get()[1]);
    ASSERT_EQ(e1.playerNames.get()[2], e2->playerNames.get()[2]);
}

TEST(GameEventTest, PixelEvent)
{
    std::stringstream s;

    PixelEvent e1 {123, 4, 567, 890};
    e1.writeTo(s);
    
    auto _e2 = GameEvent::readFrom(s);
    auto *e2 = (PixelEvent *) _e2.get();

    ASSERT_EQ(e1.header.getEventNo(), e2->header.getEventNo());
    ASSERT_EQ(e1.header.getType(), e2->header.getType());
    ASSERT_EQ(e1.data.getPlayerNumber(), e2->data.getPlayerNumber());
    ASSERT_EQ(e1.data.getX(), e2->data.getX());
    ASSERT_EQ(e1.data.getY(), e2->data.getY());
}

TEST(GameEventTest, PlayerEliminatedEvent)
{
    std::stringstream s;

    PlayerEliminatedEvent e1 {123, 2};
    e1.writeTo(s);

    auto _e2 = GameEvent::readFrom(s);
    auto *e2 = (PlayerEliminatedEvent *) _e2.get();

    ASSERT_EQ(e1.header.getEventNo(), e2->header.getEventNo());
    ASSERT_EQ(e1.header.getType(), e2->header.getType());
    ASSERT_EQ(e1.playerNumber, e2->playerNumber);
}

TEST(GameEventTest, GameOverEvent)
{
    std::stringstream s;

    GameOverEvent e1 {123};
    e1.writeTo(s);

    auto _e2 = GameEvent::readFrom(s);
    auto *e2 = (GameOverEvent *) _e2.get();

    ASSERT_EQ(e1.header.getEventNo(), e2->header.getEventNo());
    ASSERT_EQ(e1.header.getType(), e2->header.getType());
}
