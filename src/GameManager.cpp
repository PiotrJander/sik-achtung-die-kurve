//
// Created by Piotr Jander on 26/08/2017.
//

#include <chrono>
#include <Game.h>  // TODO why <>
#include "GameManager.h"

using namespace std::chrono;

void GameManager::processDatagram(const char *buffer)
{

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
        for (auto &&datagram : getDatagramBatches()) {
            udpWorker.enqueue(std::move(datagram));
        }
        udpWorker.workUntil(startOfFrame);
    }

    // reset keyPressed and last event sent
}

bool GameManager::canGameStart()
{
    return false;
}
