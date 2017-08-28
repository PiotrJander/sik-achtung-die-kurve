//
// Created by Piotr Jander on 26/08/2017.
//

#include "chrono"
#include "Game.h"
#include "EventBatch.h"
#include "GameManager.h"
#include "UdpWorker.h"
#include "easylogging++.h"
#include "Datagram.h"

using namespace std::chrono;

void GameManager::gameLoop()
{
    do {
        udpWorker->work(*this);
    } while (!canGameStart());

    game = Game(random, turningSpeed, maxx, maxy);
    game.addPlayers(connectedPlayers);
    game.start();

    // in case game is over already
    LOG(INFO) << "Enqueuing new game datagrams";
    broadcastNewDatagrams(game);

    while (game.isInProgress()) {
        milliseconds endOfFrame =
                duration_cast<milliseconds>(system_clock::now().time_since_epoch()) + milliseconds(200);  // TODO don't hardcode
        game.tick();
        LOG(INFO) << "Enqueuing tick datagrams";
        broadcastNewDatagrams(game);
        udpWorker->workUntil(endOfFrame, *this);
    }
    resetPlayers();
}

void GameManager::processDatagram(const ClientMessage::SelfPacked *buffer, const sockaddr *socketAddr)
{
    ClientMessage message(*buffer);
    updateConnectedPlayers(message, socketAddr);

    auto datagramBatches = getEventBatches(game, message.getNextExpectedEventNo());
    sockaddr_storage sockaddrStorage = Socket::copySockAddrToStorage(socketAddr);
    for (auto &&batch : datagramBatches) {
        udpWorker->enqueue(std::make_unique<Datagram>(batch, sockaddrStorage));
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
            LOG(INFO) << "Player connection update; "
                       << " new turn direction " << static_cast<int>(message.getTurnDirection())
                       << " new next expected " << message.getNextExpectedEventNo();
            conn.setTurnDirection(message.getTurnDirection());
            conn.setNextExpectedEvent(message.getNextExpectedEventNo());
        } else if (conn.getSessionId() < message.getSessionId()) {
            // same socket but different session; disconnect old and connect new
            LOG(INFO) << "Player connection replace: greater session id";
            connectedPlayers.erase(hash);
            addPlayerConnection(hash, socket, message);
        } else {
            LOG(INFO) << "Player connection ignore; smaller session id";
        }
    }
}

void GameManager::addPlayerConnection(std::size_t hash, const sockaddr *socket, const ClientMessage &message)
{
    if (connectedPlayers.size() < 42 && !isPlayerNameTaken(message.getPlayerName())) {
        PlayerConnection pc(socket, message.getSessionId(), message.getTurnDirection(), message.getPlayerName());
        connectedPlayers.insert(std::make_pair(hash, pc));
        LOG(INFO) << "Player connection added";
    }
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

bool GameManager::canGameStart()
{
    bool allReady = std::all_of(connectedPlayers.begin(), connectedPlayers.end(),
                                [](const auto &entry) { return entry.second.isReadyForGame(); });
    long participatingPlayers = std::count_if(connectedPlayers.begin(), connectedPlayers.end(),
                                     [](const auto &entry)  { return !entry.second.getName().empty(); });
    LOG(INFO) << "canGameStart: Players ready: " << allReady << "; participating players: " << participatingPlayers;
    return allReady && participatingPlayers > 1;
}

void GameManager::resetPlayers()
{
    for (auto &&p : connectedPlayers) {
        p.second.resetAfterGame();
    }
}

std::vector<EventBatch> GameManager::getEventBatches(const Game &game, uint32_t startEventNumber)
{
    const int MAX_DATAGRAM_SIZE = 512;
    const int SIZEOF_HEADER = sizeof(uint32_t);
    int length = SIZEOF_HEADER;
    std::vector<EventBatch> vector;
    for (uint32_t eventNumber = startEventNumber; eventNumber < game.getEventHistory().size(); ++eventNumber) {
        uint32_t eventSize = game.getEventHistory().at(startEventNumber)->getLength();
        if (length + eventSize > MAX_DATAGRAM_SIZE) {
            LOG(INFO) << "getEventBatches: producing a batch of length " << length;
            vector.emplace_back(EventBatch(game, startEventNumber, eventNumber));
            length = SIZEOF_HEADER + eventSize;
            startEventNumber = eventNumber;
        } else {
            length += eventSize;
        }
    }

    // only make an event batch if there any new events
    if (length > SIZEOF_HEADER) {
        LOG(INFO) << "getEventBatches: producing a batch of length " << length;
        uint32_t endEventNo = static_cast<uint32_t>(game.getEventHistory().size());
        vector.emplace_back(EventBatch(game, startEventNumber, endEventNo));
    } else {
        LOG(INFO) << "getEventBatches: no new events to enqueue";
    }

    return vector;
}

void GameManager::broadcastDatagrams(std::vector<EventBatch> eventBatches)
{
    for (auto &&eventBatch : eventBatches) {
        for (auto &&player : connectedPlayers) {
            udpWorker->enqueue(std::make_unique<Datagram>(eventBatch, player.second.getSocketStorage()));
        }
    }
}

void GameManager::broadcastNewDatagrams(const Game &game)
{
    LOG(INFO) << "broadcastNewDatagrams: calling getEventBatches";
    auto eventBatches = getEventBatches(game, game.getFirstNewEventNumber());
    broadcastDatagrams(eventBatches);
}

GameManager::GameManager(uint32_t maxx, uint32_t maxy, int roundsPerSecond, int turningSpeed,
                         int seed, std::unique_ptr<IUdpWorker> udpWorker)
        : connectedPlayers(),
          maxx(maxx), maxy(maxy),
          roundsPerSecond(roundsPerSecond),
          turningSpeed(turningSpeed),
          random(seed),
          udpWorker(std::move(udpWorker)),
          game(random, turningSpeed, maxx, maxy)
{}
