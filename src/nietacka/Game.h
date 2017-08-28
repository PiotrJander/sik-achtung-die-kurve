//
// Created by Piotr Jander on 24/08/2017.
//

#ifndef PROJECT_GAMESTATE_H
#define PROJECT_GAMESTATE_H


#include <cstdint>
#include <string>
#include <vector>
#include <map>
//#include "../../test/lib/googletest/include/gtest/gtest_prod.h"
#include "../../test/lib/googletest/include/gtest/gtest_prod.h"
#include "PlayerConnection.h"
#include "event/GameEvent.h"
#include "Random.h"

typedef std::pair<long , long> CoordinateLong;
typedef std::pair<unsigned long , unsigned long> CoordinateUnsignedLong;
typedef std::pair<double , double> CoordinateDouble;
typedef std::vector<std::unique_ptr<GameEvent>> EventHistory;

class Game {
public:
    FRIEND_TEST(GameTest, AddPlayers);
    FRIEND_TEST(GameTest, TickGeneratesPixel);
    FRIEND_TEST(GameTest, TickEliminatePlayerGameOver);
    FRIEND_TEST(GameManagerTest, EnqueueNewDatagramBatches);

    class Player {
    public:
        Player(std::size_t hash, uint8_t number, uint64_t sessionId, PlayerConnectionMap &connections);

        std::size_t hash;
        uint8_t number;
        uint64_t sessionId;
        PlayerConnectionMap &connections;
        double x, y, heading;
        bool eliminated;

        std::string const & getName() const;

        int8_t getTurnDirection() const;

        Player &setCoordinates(CoordinateDouble c)
        {
            x = c.first;
            y = c.second;
            return *this;
        }

        CoordinateLong getCoordinates() const
        {
            return CoordinateLong(x, y);
        }

    private:
        const PlayerConnectionMap::iterator getPlayerConnection() const;
    };

    Game(Random &random, int turningSpeed, uint32_t maxx, uint32_t maxy);

    void addPlayers(PlayerConnectionMap &connections);

    void start();

    void tick();

    const EventHistory &getEventHistory() const
    {
        return events;
    }

    bool isInProgress() const
    {
        return inProgress;
    }

    uint32_t getFirstNewEventNumber() const;

    uint32_t getId() const
    {
        return id;
    }

private:
    Random random;

    std::vector<Player> players;

    int turningSpeed;

    uint32_t id, maxx, maxy, eventNoCounter = 0, firstNewEventNumber = 0;

    bool inProgress = true;

    /**
     * Vector of columns
     */
    std::vector<std::vector<bool>> matrix;

    EventHistory events;

    void setPixel(CoordinateUnsignedLong c)
    {
        matrix.at(c.first).at(c.second) = true;
    }

    bool isSetPixel(CoordinateUnsignedLong c) const
    {
        return matrix.at(c.first).at(c.second);
    }

    bool shouldPlayerGetEliminated(const Player &p) const
    {
        const CoordinateLong c = p.getCoordinates();
        return c.first < 0 || c.first >= maxx || c.second < 0 || c.second >= maxy || isSetPixel(c);
    };

    uint32_t nextEventNo() {
        return eventNoCounter++;
    }

    int numberOfPlayers() const;
};


#endif //PROJECT_GAMESTATE_H
