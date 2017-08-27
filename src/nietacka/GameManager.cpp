//
// Created by Piotr Jander on 26/08/2017.
//

#include "chrono"
#include "Game.h"
#include "EventBatch.h"
#include "GameManager.h"
#include "UdpWorker.h"

using namespace std::chrono;

void GameManager::gameLoop()
{
    do {
        auto res = udpWorker->getDatagram();
        processDatagram(res.first, res.second, nullptr);
    } while (!canGameStart());

    Game game(random, turningSpeed, maxx, maxy);
    game.addPlayers(connectedPlayers);
    game.start();

    // in case game is over already
    enqueueNewDatagramBatches(game, 0);

    while (game.isInProgress()) {
        milliseconds startOfFrame = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        game.tick();
        enqueueNewDatagramBatches(game, 0);
//        udpWorker->workUntil(startOfFrame, <#initializer#>);
    }
    resetPlayers();
}

void GameManager::processDatagram(const ClientMessage::SelfPacked *buffer, const sockaddr *socket,
                                  const Game *game = nullptr)
{
    ClientMessage message(*buffer);
    updateConnectedPlayers(message, socket);

    if (game) {
        enqueueNewDatagramBatches(*game, message.getNextExpectedEventNo());
    }
}

void GameManager::updateConnectedPlayers(const ClientMessage &message, const sockaddr *socket)
{
    size_t hash = PlayerConnection::getHashFor(socket);
    const auto &entry = connectedPlayers.find(hash);
    if (entry == connectedPlayers.end()) {
        // new socket
        addPlayerConnection(hash, socket, message);
    } else {
        // existing socket
        auto &conn = entry->second;
        if (conn.getSessionId() == message.getSessionId()) {
            // same player, update fields
            conn.setTurnDirection(message.getTurnDirection());
            conn.setNextExpectedEvent(message.getNextExpectedEventNo());
        } else if (conn.getSessionId() < message.getSessionId()) {
            // same socket but different session; disconnect old and connect new
            connectedPlayers.erase(hash);
            addPlayerConnection(hash, socket, message);
        } else {
            // ignore
        }
    }
}

void GameManager::addPlayerConnection(std::size_t hash, const sockaddr *socket, const ClientMessage &message)
{
    if (connectedPlayers.size() < 42 && !isPlayerNameTaken(message.getPlayerName())) {
        PlayerConnection pc(socket, message.getSessionId(), message.getTurnDirection(), message.getPlayerName());
        connectedPlayers.insert(std::make_pair(hash, pc));
    }
}

bool GameManager::canGameStart()
{
//    return std::all_of(connectedPlayers.begin(), connectedPlayers.end(),
//                       [](const auto &entry) { return entry.second.isReadyForGame(); });
    return false;  // TODO revert
}

void GameManager::resetPlayers()
{
    for (auto &&p : connectedPlayers) {
        p.second.resetAfterGame();
    }
}

void GameManager::enqueueNewDatagramBatches(const Game &game, uint32_t startEventNumber)
{
    const int MAX_DATAGRAM_SIZE = 512;
    const int SIZEOF_HEADER = sizeof(uint32_t);
    int length = SIZEOF_HEADER;
    for (uint32_t eventNumber = startEventNumber; eventNumber < game.getEventHistory().size(); ++eventNumber) {
        uint32_t eventSize = game.getEventHistory().at(startEventNumber)->getLength();
        if (length + eventSize > MAX_DATAGRAM_SIZE) {
            udpWorker->enqueue(
                    std::make_unique<EventBatch>(length, game.getEventHistory(),
                                                 startEventNumber, eventNumber, game.getId())
            );
            length = SIZEOF_HEADER + eventSize;
            startEventNumber = eventNumber;
        } else {
            length += eventSize;
        }
    }
    uint32_t endEventNo = static_cast<uint32_t>(game.getEventHistory().size());
    udpWorker->enqueue(std::make_unique<EventBatch>(length, game.getEventHistory(),
                                                    startEventNumber, endEventNo, game.getId()));
}

bool GameManager::isPlayerNameTaken(const std::string &name) const
{
    // there can be many observing players
    if (name.empty()) {
        return false;
    }

    for (auto &&entry : connectedPlayers) {
        if (entry.second.getName() == name) {
            return true;
        }
    }

    // else
    return false;
}

GameManager::GameManager(uint32_t maxx, uint32_t maxy, uint16_t port, int roundsPerSecond, int turningSpeed, int seed)
        : connectedPlayers(),
          maxx(maxx), maxy(maxy),
          port(port),
          roundsPerSecond(roundsPerSecond),
          turningSpeed(turningSpeed),
          random(seed),
          udpWorker(std::make_unique<UdpWorker>(port))
{}
