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

typedef std::pair<uint32_t , uint32_t> CoordinateInt;
typedef std::pair<double , double> CoordinateDouble;

class Game {
public:

    class Player {
    public:
        Player(uint8_t number, uint64_t sessionId, std::map<uint64_t, PlayerConnection> &connections);

        uint8_t number;
        uint64_t sessionId;
        std::map<uint64_t, PlayerConnection> &connections;
        double x, y, heading;

        std::string const & getName() const;

        uint8_t getTurnDirection() const;

        void setCoordinates(CoordinateDouble c)
        {
            x = c.first;
            y = c.second;
        }

        CoordinateInt getCoordinates() const
        {
            return CoordinateInt(x, y);
        }
    };

    Game(Random &random, int turningSpeed, uint32_t maxx, uint32_t maxy);

    void addPlayers(std::map<uint64_t, PlayerConnection> &connections);

    void start();

    void tick();

private:
    Random random;

    std::vector<Player> players;

    int turningSpeed;

    uint32_t id, maxx, maxy, eventNoCounter = 0;

    /**
     * Vector of columns
     */
    std::vector<std::vector<bool>> matrix;

    std::vector<GameEvent> events;

    void setPixel(CoordinateInt c)
    {
        matrix.at(c.first).at(c.second) = true;
    }

    bool isSetPixel(CoordinateInt c) const
    {
        return matrix.at(c.first).at(c.second);
    }
    
    bool shouldPlayerGetEliminated(const Player &p) const
    {
        const CoordinateInt &c = p.getCoordinates();
        return isSetPixel(c) || c.first < 0 || c.first >= maxx || c.second < 0 || c.second >= maxy;
    };

    uint32_t nextEventNo() {
        return eventNoCounter++;
    }
};


#endif //PROJECT_GAMESTATE_H
