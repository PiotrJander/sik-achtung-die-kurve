//
// Created by Piotr Jander on 26/08/2017.
//

#include "chrono"
#include "Game.h"
#include "EventBatch.h"
#include "GameManager.h"

using namespace std::chrono;

void GameManager::processDatagram(const ClientMessage::SelfPacked *buffer, const sockaddr *socket,
                                  const Game *game = nullptr)
{
    ClientMessage message(*buffer);

    // add or update connected player
    size_t hash = PlayerConnection::getHashFor(socket);
    const auto &entry = connectedPlayers.find(hash);
    if (entry == connectedPlayers.end()) {
        // new socket
        addPlayerConnection(hash, socket, message);
    } else {
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

    // send response if game in progress
    if (game) {
        enqueueNewDatagramBatches(*game, message.getNextExpectedEventNo());
    }
}

void GameManager::addPlayerConnection(std::size_t hash, const sockaddr *socket, const ClientMessage &message)
{
    if (connectedPlayers.size() < 42 && !isPlayerNameTaken(message.getPlayerName())) {
        connectedPlayers[hash] =
                PlayerConnection(socket, message.getSessionId(), message.getTurnDirection(), message.getPlayerName());
    }
}

void GameManager::gameLoop()
{
    do {
        auto res = udpWorker.getDatagram();
        processDatagram(res.first, res.second);
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
        udpWorker.workUntil(startOfFrame);
    }
    resetPlayers();
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

void GameManager::enqueueNewDatagramBatches(const Game &game, uint32_t startEventNumber = game.getFirstNewEventNumber())
{
    const MAX_DATAGRAM_SIZE = 512;
    const SIZEOF_HEADER = sizeof(uint32_t);
    int length = SIZEOF_HEADER;
    for (uint32_t eventNumber = startEventNumber; eventNumber < game.getEventHistory().size(); ++eventNumber) {
        uint32_t eventSize = game.getEventHistory().at(startEventNumber)->getLength();
        if (length + eventSize > MAX_DATAGRAM_SIZE) {
            udpWorker.enqueue(EventBatch(length, game.getEventHistory(), startEventNumber, eventNumber));
            length = SIZEOF_HEADER;
            startEventNumber = eventNumber;
            eventNumber--;
        } else {
            length += eventSize;
        }
    }
    udpWorker.enqueue(EventBatch(length, game.getEventHistory(), startEventNumber,
                                 static_cast<uint32_t>(game.getEventHistory().size())));
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
