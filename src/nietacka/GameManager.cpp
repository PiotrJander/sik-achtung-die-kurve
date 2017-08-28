//
// Created by Piotr Jander on 26/08/2017.
//

#include "chrono"
#include "Game.h"
#include "EventBatch.h"
#include "GameManager.h"
#include "UdpWorker.h"
#include "easylogging++.h"
#include "Exceptions.h"
#include "Datagram.h"

using namespace std::chrono;

void GameManager::gameLoop()
{
    gamePtr = nullptr;
    do {
        udpWorker->getDatagram(*this);
    } while (!canGameStart());

    Game game(random, turningSpeed, maxx, maxy);
    gamePtr = &game;
    game.addPlayers(connectedPlayers);
    game.start();

    // in case game is over already
    broadcastNewDatagrams(game);

    while (game.isInProgress()) {
        milliseconds endOfFrame =
                duration_cast<milliseconds>(system_clock::now().time_since_epoch()) + milliseconds(200);  // TODO don't hardcode
        game.tick();
        broadcastNewDatagrams(game);
        udpWorker->workUntil(endOfFrame, *this);
    }
    resetPlayers();
    gamePtr = nullptr;
}

void GameManager::processDatagram(const ClientMessage::SelfPacked *buffer, const sockaddr *socketAddr)
{
    ClientMessage message(*buffer);
    updateConnectedPlayers(message, socketAddr);

    if (gamePtr) {
        auto datagramBatches = getEventBatches(*gamePtr, message.getNextExpectedEventNo());
        sockaddr_storage sockaddrStorage = Socket::copySockAddrToStorage(socketAddr);
        for (auto &&batch : datagramBatches) {
            udpWorker->enqueue(std::make_unique<Datagram>(batch, sockaddrStorage));
        }
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
    return std::all_of(connectedPlayers.begin(), connectedPlayers.end(),
                       [](const auto &entry) { return entry.second.isReadyForGame(); });
}

void GameManager::resetPlayers()
{
    for (auto &&p : connectedPlayers) {
        p.second.resetAfterGame();
    }
}

std::vector<std::shared_ptr<EventBatch>> GameManager::getEventBatches(const Game &game, uint32_t startEventNumber)
{
    const int MAX_DATAGRAM_SIZE = 512;
    const int SIZEOF_HEADER = sizeof(uint32_t);
    int length = SIZEOF_HEADER;
    std::vector<std::shared_ptr<EventBatch>> vector;
    for (uint32_t eventNumber = startEventNumber; eventNumber < game.getEventHistory().size(); ++eventNumber) {
        uint32_t eventSize = game.getEventHistory().at(startEventNumber)->getLength();
        if (length + eventSize > MAX_DATAGRAM_SIZE) {
            vector.emplace_back(std::make_shared<EventBatch>(length, game.getEventHistory(),
                                                             startEventNumber, eventNumber, game.getId()));
            length = SIZEOF_HEADER + eventSize;
            startEventNumber = eventNumber;
        } else {
            length += eventSize;
        }
    }
    uint32_t endEventNo = static_cast<uint32_t>(game.getEventHistory().size());
    vector.emplace_back(std::make_shared<EventBatch>(length, game.getEventHistory(),
                                                     startEventNumber, endEventNo, game.getId()));
    return vector;
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

GameManager::GameManager(uint32_t maxx, uint32_t maxy, const string &port, int roundsPerSecond, int turningSpeed,
                         int seed)
        : connectedPlayers(),
          maxx(maxx), maxy(maxy),
          port(port),
          roundsPerSecond(roundsPerSecond),
          turningSpeed(turningSpeed),
          random(seed),
          udpWorker(std::make_unique<UdpWorker>(port))
{}

void GameManager::broadcastDatagrams(std::vector<std::shared_ptr<EventBatch>> eventBatches)
{
    for (auto &&eventBatch : eventBatches) {
        for (auto &&player : connectedPlayers) {
            udpWorker->enqueue(std::make_unique<Datagram>(eventBatch, player.second.getSocketStorage()));
        }
    }
}

void GameManager::broadcastNewDatagrams(const Game &game)
{
    auto eventBatches = getEventBatches(game, game.getFirstNewEventNumber());
    broadcastDatagrams(eventBatches);
}
