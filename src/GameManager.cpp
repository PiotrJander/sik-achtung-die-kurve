//
// Created by Piotr Jander on 26/08/2017.
//

#include <chrono>
#include <Game.h>  // TODO why <>
#include <EventBatch.h>
#include "GameManager.h"

using namespace std::chrono;

void GameManager::processDatagram(const char *buffer)
{

    // TODO enqueue datagrams from next expected ...
}

void GameManager::gameLoop()
{
    do {
        processDatagram(udpWorker.getDatagram());
    } while (!canGameStart());

    Game game(random, turningSpeed, maxx, maxy);
    game.addPlayers(connectedPlayers);
    game.start();

    // send events even if end on start
    while (game.isInProgress()) {
        milliseconds startOfFrame = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        game.tick();
        enqueueNewDatagramBatches(game);
        udpWorker.workUntil(startOfFrame);
    }
    resetPlayers();
}

bool GameManager::canGameStart()
{
    return false;  // TODO
}

void GameManager::resetPlayers()
{
    for (auto &&p : connectedPlayers) {
        p.second.resetAfterGame();
    }
}

void GameManager::enqueueNewDatagramBatches(const Game &game)
{
    const MAX_DATAGRAM_SIZE = 512;
    const SIZEOF_HEADER = 0;  // TODO this should be non-zero
    uint32_t startEventNumber = game.getFirstNewEventNumber();
    int length = SIZEOF_HEADER;
    for (uint32_t eventNumber = startEventNumber; eventNumber < game.getEventHistory().size(); ++eventNumber) {
        uint32_t eventSize = game.getEventHistory().at(startEventNumber)->getLength();
        if (length + eventSize > MAX_DATAGRAM_SIZE) {
            udpWorker.enqueue(EventBatch(length, game.getEventHistory(), startEventNumber, eventNumber));
            length = SIZEOF_HEADER;
            startEventNumber = eventNumber;
            eventNumber--;
        } else {
            length += eventSize;
        }
    }
    udpWorker.enqueue(EventBatch(length, game.getEventHistory(), startEventNumber,
                                 static_cast<uint32_t>(game.getEventHistory().size())));
}
