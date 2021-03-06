//
// Created by Piotr Jander on 24/08/2017.
//

#define _USE_MATH_DEFINES

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
{
    LOG(INFO) << "Creating game with board size " << maxx << " x " << maxy;
}

void Game::addPlayers(PlayerConnectionMap &connections)
{
    // get sessionIds for players with non-empty names
    std::vector<uint64_t> hashes;
    for (auto const &player : connections) {
        if (!player.second.getName().empty()) {
            hashes.emplace_back(player.second.hash());
        }
    }

    // sort by name
    std::sort(hashes.begin(), hashes.end(), [connections](uint64_t h1, uint64_t h2) {
        return connections.at(h1).getName() < connections.at(h2).getName();
    });

    for (uint8_t j = 0; j < hashes.size(); ++j) {
        players.emplace_back(Player(hashes.at(j), j, connections.at(hashes.at(j)).getSessionId(), connections));
    }
}

void Game::start()
{
    // make vector of player names
    std::vector<std::string> playerNames;
    for (auto const &player : players) {
        playerNames.emplace_back(player.getName());
    }

    // make new game event
    events.emplace_back(std::make_shared<NewGameEvent>(nextEventNo(), maxx, maxy, std::move(playerNames)));

    for (auto &&player : players) {
        // generate coordinates and heading
        CoordinateLong coors((random.rand() % maxx) + 0.5, (random.rand() % maxy) + 0.5);
        player.setCoordinates(coors);
        player.heading = random.rand() % 360;
        LOG(INFO) << "Initial heading " << player.heading;

        if (shouldPlayerGetEliminated(player)) {
            player.eliminated = true;
            events.emplace_back(std::make_shared<PlayerEliminatedEvent>(nextEventNo(), player.number));
            if (numberOfPlayers() == 1) {
                events.emplace_back(std::make_shared<GameOverEvent>(nextEventNo()));
                inProgress = false;
                return;
            }
        } else {
            setPixel(player.getCoordinates());
            events.emplace_back(std::make_shared<PixelEvent>(nextEventNo(), player.number, coors.first, coors.second));
        }
    }
}

void Game::tick()
{
    firstNewEventNumber = eventNoCounter;

    for (auto &&player : players) {
        // update heading
        player.heading += player.getTurnDirection() * turningSpeed;
        LOG(INFO) << "Updating heading to " << player.heading << " turn direction " << (int) player.getTurnDirection()
                     << " turning speed " << turningSpeed;


        // save old position and update position
        CoordinateLong previousPosition = player.getCoordinates();
        double heading_radians = player.heading * M_PI / 180;
        LOG(INFO) << "Heading: degrees " << player.heading << " radians " << heading_radians;
        double delta_x = cos(heading_radians);
        double delta_y = sin(player.heading * M_PI / 180);
        LOG(INFO) << "Position change: " << delta_x << ":" << delta_y;
        player.x += delta_x;
        player.y += delta_y;
        CoordinateLong newPosition = player.getCoordinates();

        LOG(INFO) << "Previous: " << previousPosition.first << ":" << previousPosition.second
                  << "; new: " << newPosition.first << ":" << newPosition.second;

        if (previousPosition.first == newPosition.first && previousPosition.second == newPosition.second) {
            // do nothing
        } else if (shouldPlayerGetEliminated(player)) {
            player.eliminated = true;
            events.emplace_back(std::make_shared<PlayerEliminatedEvent>(nextEventNo(), player.number));
            if (numberOfPlayers() == 1) {
                events.emplace_back(std::make_shared<GameOverEvent>(nextEventNo()));
                inProgress = false;
                return;
            }
        } else {
            setPixel(newPosition);
            events.emplace_back(std::make_shared<PixelEvent>(nextEventNo(), player.number,
                                                             newPosition.first, newPosition.second));
        }
    }
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

uint32_t Game::getFirstNewEventNumber() const
{
    return firstNewEventNumber;
}

Game::Player::Player(std::size_t hash, uint8_t number, uint64_t sessionId, PlayerConnectionMap &connections)
        : hash(hash), number(number), sessionId(sessionId), connections(connections)
{}

const PlayerConnectionMap::iterator Game::Player::getPlayerConnection() const
{
    const auto &conn = connections.find(hash);
    if (conn == connections.end()) {
        return connections.end();
    } else {
        return sessionId == conn->second.getSessionId() ? conn : connections.end();
    }
}

std::string const &Game::Player::getName() const
{
    static const std::string emptyString = "";
    const auto &conn = getPlayerConnection();
    if (conn == connections.end()) {
        return emptyString;
    } else {
        return conn->second.getName();
    }
}

int8_t Game::Player::getTurnDirection() const
{
    const auto &conn = getPlayerConnection();
    if (conn == connections.end()) {
        return 0;
    } else {
        return conn->second.getTurnDirection();
    }
}
