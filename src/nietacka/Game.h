//
// Created by Piotr Jander on 24/08/2017.
//

#ifndef PROJECT_GAMESTATE_H
#define PROJECT_GAMESTATE_H


#include <cstdint>
#include <string>
#include <vector>
#include "PlayerConnection.h"
#include "event/GameEvent.h"
#include "Random.h"

class Game {
public:

    class Player {
    public:
        Player(uint8_t number, const std::string &name, double x, double y, double heading);

        uint8_t number;
        std::string name;
        double x, y, heading;
    };

    Game(Random &random, int turningSpeed, uint32_t maxx, uint32_t maxy);

    void addPlayers(std::vector<PlayerConnection> &playerConnections);

    void tick();

private:

    Random random;

    std::vector<Player> players;

    int turningSpeed, nextEventNo = 0;

    uint32_t id, maxx, maxy;

    /**
     * Vector of columns
     */
    std::vector<std::vector<bool>> matrix;

    std::vector<GameEvent> events;
};


#endif //PROJECT_GAMESTATE_H
