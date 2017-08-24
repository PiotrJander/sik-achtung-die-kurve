//
// Created by Piotr Jander on 24/08/2017.
//

#include "Game.h"
#include "event/NewGameEvent.h"
#include "event/PlayerEliminatedEvent.h"
#include "event/PixelEvent.h"


Game::Game(Random &random, int turningSpeed, uint32_t maxx, uint32_t maxy)
        : random(random), players(), id(random.rand()), turningSpeed(turningSpeed), maxx(maxx), maxy(maxy),
          matrix(maxx, std::vector<bool>(maxy, false)), events()
{}

void Game::addPlayers(std::vector<PlayerConnection> &playerConnections)
{
//    std::vector<std::reference_wrapper<PlayerConnection>> activePlayerConnections(playerConnections.begin(),
//                                                                                  playerConnections.end());
//
//    // could also be done with copy_if
//    activePlayerConnections.erase(
//            std::remove_if(activePlayerConnections.begin(), activePlayerConnections.end(),
//                           [](const PlayerConnection &p){ return p.getName().empty(); })
//    );
//
//    std::sort(activePlayerConnections.begin(), activePlayerConnections.end());
//
//    for (uint8_t i = 0; i < activePlayerConnections.size(); ++i) {
//        Player p(i, activePlayerConnections[i].get().getName());  // TODO inline
//        playerConnections.emplace_back(std::move(p));
//    }
}

void Game::start()
{
    // make vector of player names
    std::vector<std::string> playerNames;
    std::transform(players.begin(), players.end(), playerNames.begin(), [](const Player &p) { return p.name; });

    // make new game event
    events.emplace_back(NewGameEvent(nextEventNo(), maxx, maxy, std::move(playerNames)));

    for (auto &&player : players) {
        // generate coordinates and heading
        CoordinateInt coors((random.rand() % maxx) + 0.5, (random.rand() % maxy) + 0.5);
        player.setCoordinates(coors);
        player.heading = random.rand() % 360;

        if (shouldPlayerGetEliminated(player)) {
            events.emplace_back(PlayerEliminatedEvent(nextEventNo(), player.number));
            // TODO set player to be eliminated
        } else {
            setPixel(player.getCoordinates());
            events.emplace_back(PixelEvent(nextEventNo(), player.number, coors.first, coors.second));
        }
    }
}

Game::Player::Player(uint8_t number, const std::string &name) : number(number),
                                                                name(name), x(x), y(y),
                                                                heading(heading)
{}
