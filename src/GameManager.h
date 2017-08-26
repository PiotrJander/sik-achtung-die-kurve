//
// Created by Piotr Jander on 26/08/2017.
//

#ifndef PROJECT_GAMEMANAGER_H
#define PROJECT_GAMEMANAGER_H


#include <IDatagramObserver.h>
#include <PlayerConnection.h>
#include <IUdpWorker.h>

class GameManager: public IDatagramObserver {
public:
    void processDatagram(const char *buffer) override;

    void gameLoop();

private:
    PlayerConnectionMap connectedPlayers;

    IUdpWorker udpWorker;

    uint32_t maxx, maxy;
    uint16_t port;  // TODO remember about htons!
    int roundsPerSecond;
    int turningSpeed;
    Random random;

    bool canGameStart();

    void resetPlayers();

    void enqueueNewDatagramBatches(const Game &game);
};


#endif //PROJECT_GAMEMANAGER_H
