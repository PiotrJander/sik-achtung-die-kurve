//
// Created by Piotr Jander on 24/08/2017.
//

#include "Game.h"


Game::Game(Random &random, int turningSpeed, uint32_t maxx, uint32_t maxy)
        : random(random), players(), id(random.rand()), turningSpeed(turningSpeed), maxx(maxx), maxy(maxy),
          matrix(maxx, std::vector<bool>(maxy, false)), events()
{}

void Game::addPlayers(std::vector<PlayerConnection> &playerConnections)
{
    std::vector<std::reference_wrapper<PlayerConnection>> activePlayerConnections(playerConnections.begin(),
                                                                                  playerConnections.end());

    // could also be done with copy_if
    activePlayerConnections.erase(
            std::remove_if(activePlayerConnections.begin(), activePlayerConnections.end(),
                           [](const PlayerConnection &p){ return p.getName().empty(); })
    );

    std::sort(activePlayerConnections.begin(), activePlayerConnections.end());

    for (uint8_t i = 0; i < activePlayerConnections.size(); ++i) {
        Player p(
                i,
                activePlayerConnections[i].get().getName(),
                (random.rand() % maxx) + 0.5,
                (random.rand() % maxy) + 0.5,
                random.rand() % 360
        );
        playerConnections.emplace_back(std::move(p));
    }
}

Game::Player::Player(uint8_t number, const std::string &name, double x, double y, double heading) : number(number),
                                                                                                    name(name), x(x), y(y),
                                                                                                    heading(heading)
{}
