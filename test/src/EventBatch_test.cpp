//
// Created by Piotr Jander on 26/08/2017.
//

#include <nietacka/EventBatch.h>
#include <nietacka/event/PixelEvent.h>
#include <nietacka/event/PlayerEliminatedEvent.h>
#include <nietacka/event/GameOverEvent.h>
#include <nietacka/event/NewGameEvent.h>
#include "gtest/gtest.h"

//INITIALIZE_EASYLOGGINGPP

TEST(EventBatchTest, GetBuffer)
{
    Random rand(123);
    Game game(rand, 6, 40, 40);
    game.id = 456;
    std::vector<std::string> names = {"Jed", "Piotr"};
    auto newGame = std::make_shared<NewGameEvent>(0, 800, 600, names);
    game.events.emplace_back(newGame);
    game.events.emplace_back(std::make_shared<PixelEvent>(1, 3, 45, 67));
    game.events.emplace_back(std::make_shared<PlayerEliminatedEvent>(2, 3));
    game.events.emplace_back(std::make_shared<GameOverEvent>(3));

    EventBatch eventBatch(game, 0, 4);
    DynamicBuffer buffer = eventBatch.getBuffer();

    ReadBuffer readBuffer(buffer);

    uint32_t gameId;

    readBuffer >> gameId;
    gameId = ntohl(gameId);

    const std::unique_ptr<GameEvent> &e1 = GameEvent::read(readBuffer);
    const std::unique_ptr<GameEvent> &e2 = GameEvent::read(readBuffer);
    const std::unique_ptr<GameEvent> &e3 = GameEvent::read(readBuffer);
    const std::unique_ptr<GameEvent> &e4 = GameEvent::read(readBuffer);

    NewGameEvent *newGameEvent = dynamic_cast<NewGameEvent *>(e1.get());
    PixelEvent *pixelEvent = dynamic_cast<PixelEvent *>(e1.get());
    PlayerEliminatedEvent *eliminatedEvent = dynamic_cast<PlayerEliminatedEvent *>(e1.get());
    GameOverEvent *gameOverEvent = dynamic_cast<GameOverEvent *>(e1.get());

    EXPECT_EQ(game.id, gameId);
    EXPECT_EQ(newGameEvent->getMaxx(), 800);
    EXPECT_EQ(newGameEvent->getPlayerNames().at(0), "Jed");
}