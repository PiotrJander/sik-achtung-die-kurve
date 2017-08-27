//
// Created by Piotr Jander on 26/08/2017.
//

#ifndef PROJECT_GAMEMANAGER_H
#define PROJECT_GAMEMANAGER_H


#include "gtest/gtest_prod.h"
#include "IDatagramObserver.h"
#include "PlayerConnection.h"
#include "IUdpWorker.h"
#include "Game.h"

class GameManager: public IDatagramObserver {
public:
    FRIEND_TEST(GameManagerTest, UpdateConnectedPlayers);
    FRIEND_TEST(GameManagerTest, IsPlayerNameTaken);
    FRIEND_TEST(GameManagerTest, CanGameStart);
    FRIEND_TEST(GameManagerTest, EnqueueNewDatagramBatches);

    GameManager(uint32_t maxx, uint32_t maxy, string &port, int roundsPerSecond, int turningSpeed, int seed);

    void processDatagram(const ClientMessage::SelfPacked *buffer, const sockaddr *socket, const Game *game) override;

    void gameLoop();

private:
    PlayerConnectionMap connectedPlayers;

    std::unique_ptr<IUdpWorker> udpWorker;

    uint32_t maxx, maxy;
    std::string port;
    int roundsPerSecond;
    int turningSpeed;
    Random random;

    bool canGameStart();

    void resetPlayers();

    void enqueueNewDatagramBatches(const Game &game, uint32_t startEventNumber);

    void addPlayerConnection(std::size_t hash, const sockaddr *socket, const ClientMessage &message);

    bool isPlayerNameTaken(const std::string &name) const;

    void updateConnectedPlayers(const ClientMessage &message, const sockaddr *socket);
};


#endif //PROJECT_GAMEMANAGER_H
