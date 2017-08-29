////
//// Created by Piotr Jander on 26/08/2017.
////
//
//#include <nietacka/EventBatch.h>
//#include <nietacka/event/PixelEvent.h>
//#include <nietacka/event/PlayerEliminatedEvent.h>
//#include <nietacka/event/GameOverEvent.h>
//#include "gtest/gtest.h"
//
////INITIALIZE_EASYLOGGINGPP
//
//TEST(EventBatchTest, GetBuffer)
//{
//    Random rand(123);
//    Game game(rand, 6, 40, 40);
//    game.id = 456;
//    game.events.emplace_back(std::make_shared<PixelEvent>(0, 3, 45, 67));
//    game.events.emplace_back(std::make_shared<PlayerEliminatedEvent>(1, 3));
//    game.events.emplace_back(std::make_shared<GameOverEvent>(2));
//
//    EventBatch eventBatch(game, 0, 3);
//    DynamicBuffer buffer = eventBatch.getBuffer();
//    const char *bufferLocation = buffer.getStartPointer();
//
//    uint32_t gameId = *reinterpret_cast<const uint32_t *>(bufferLocation);
//    gameId = ntohl(gameId);
//    EXPECT_EQ(gameId, 456);
//    bufferLocation += sizeof(uint32_t);
//
//    uint32_t length1 = *reinterpret_cast<const uint32_t *>(bufferLocation);
//    length1 = ntohl(length1);
//    bufferLocation += sizeof(uint32_t);
//    std::unique_ptr<GameEvent> e1 = GameEvent::read(<#initializer#>, 0);
//    bufferLocation += length1;
//    uint32_t crc1 = *reinterpret_cast<const uint32_t *>(bufferLocation);
//    bufferLocation += sizeof(uint32_t);
//
//    uint32_t length2 = *reinterpret_cast<const uint32_t *>(bufferLocation);
//    length2 = ntohl(length2);
//    bufferLocation += sizeof(uint32_t);
//    std::unique_ptr<GameEvent> e2 = GameEvent::read(<#initializer#>, 0);
//    bufferLocation += length2;
//    uint32_t crc2 = *reinterpret_cast<const uint32_t *>(bufferLocation);
//    bufferLocation += sizeof(uint32_t);
//
//    uint32_t length3 = *reinterpret_cast<const uint32_t *>(bufferLocation);
//    length3 = ntohl(length3);
//    bufferLocation += sizeof(uint32_t);
//    std::unique_ptr<GameEvent> e3 = GameEvent::read(<#initializer#>, 0);
//    bufferLocation += length3;
//    uint32_t crc3 = *reinterpret_cast<const uint32_t *>(bufferLocation);
//    bufferLocation += sizeof(uint32_t);
//
//    PixelEvent *pixelEvent = dynamic_cast<PixelEvent *>(e1.get());
//    PlayerEliminatedEvent *eliminatedEvent = dynamic_cast<PlayerEliminatedEvent *>(e2.get());
//    GameOverEvent *gameOverEvent = dynamic_cast<GameOverEvent *>(e3.get());
//
//    EXPECT_EQ(pixelEvent->getX(), 45);
//    EXPECT_EQ(eliminatedEvent->getPlayerNumber(), 3);
//    EXPECT_EQ(gameOverEvent->getEventNo(), 2);
//}