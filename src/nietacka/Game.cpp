//
// Created by Piotr Jander on 24/08/2017.
//

#include <map>
#include <cmath>
#include "Game.h"
#include "event/NewGameEvent.h"
#include "event/PlayerEliminatedEvent.h"
#include "event/PixelEvent.h"
#include "event/GameOverEvent.h"


Game::Game(Random &random, int turningSpeed, uint32_t maxx, uint32_t maxy)
        : random(random), players(), id(random.rand()), turningSpeed(turningSpeed), maxx(maxx), maxy(maxy),
          matrix(maxx, std::vector<bool>(maxy, false)), events()
{}

void Game::addPlayers(std::map<uint64_t, PlayerConnection> &connections)
{
    // get sessionIds for players with non-empty names
    std::vector<uint64_t> sessionIds;
    for (auto const &player : connections) {
        if (!player.second.getName().empty()) {
            sessionIds.emplace_back(player.second.getSessionId());
        }
    }

    // sort by name
    std::sort(sessionIds.begin(), sessionIds.end(), [connections](uint64_t s1, uint64_t s2) {
        return connections.at(s1).getName() < connections.at(s2).getName();
    });

    for (uint8_t j = 0; j < sessionIds.size(); ++j) {
        players.emplace_back(Player(j, sessionIds.at(j), connections));
    }
}

bool Game::start()
{
    // make vector of player names
    std::vector<std::string> playerNames;
    for (auto const &player : players) {
        playerNames.emplace_back(player.getName());
    }

    // make new game event
    events.emplace_back(std::make_unique<NewGameEvent>(nextEventNo(), maxx, maxy, std::move(playerNames)));

    for (auto &&player : players) {
        // generate coordinates and heading
        CoordinateLong coors((random.rand() % maxx) + 0.5, (random.rand() % maxy) + 0.5);
        player.setCoordinates(coors);
        player.heading = random.rand() % 360;

        if (shouldPlayerGetEliminated(player)) {
            player.eliminated = true;
            events.emplace_back(std::make_unique<PlayerEliminatedEvent>(nextEventNo(), player.number));
            if (numberOfPlayers() == 1) {
                events.emplace_back(std::make_unique<GameOverEvent>(nextEventNo()));
                return false;
            }
        } else {
            setPixel(player.getCoordinates());
            events.emplace_back(std::make_unique<PixelEvent>(nextEventNo(), player.number, coors.first, coors.second));
        }
    }
    return true;
}

bool Game::tick()
{
    for (auto &&player : players) {
        // update heading
        player.heading += player.getTurnDirection() * turningSpeed;

        // save old position and update position
        CoordinateLong previousPosition = player.getCoordinates();
        player.x += cos(player.heading);
        player.y += sin(player.heading);
        CoordinateLong newPosition = player.getCoordinates();

        if (previousPosition == newPosition) {
            // do nothing
        } else if (shouldPlayerGetEliminated(player)) {
            player.eliminated = true;
            events.emplace_back(std::make_unique<PlayerEliminatedEvent>(nextEventNo(), player.number));
            if (numberOfPlayers() == 1) {
                events.emplace_back(std::make_unique<GameOverEvent>(nextEventNo()));
                return false;
            }
        } else {
            setPixel(newPosition);
            events.emplace_back(std::make_unique<PixelEvent>(nextEventNo(), player.number, newPosition.first, newPosition.second));
        }
    }

    // still multiple players; so game goes on
    return true;
}

int Game::numberOfPlayers() const
{
    int n = 0;
    for (auto &&player : players) {
        if (!player.eliminated) {
            n++;
        }
    }
    return n;
}

Game::Player::Player(uint8_t number, uint64_t sessionId, std::map<uint64_t, PlayerConnection> &connections)
        : number(number), sessionId(sessionId), connections(connections)
{}

std::string const &Game::Player::getName() const
{
    static const std::string emptyString = "";
    const auto &conn = connections.find(sessionId);
    if (conn == connections.end()) {
        return emptyString;
    } else {
        return conn->second.getName();
    }
}

int8_t Game::Player::getTurnDirection() const
{
    const auto &conn = connections.find(sessionId);
    if (conn == connections.end()) {
        return 0;
    } else {
        return conn->second.getTurnDirection();
    }
}
