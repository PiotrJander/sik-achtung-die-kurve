//
// Created by Piotr Jander on 22/08/2017.
//

#include <sstream>
#include "GameEvent.h"
#include "../StreamUtils.h"
#include "../crc32c.h"
#include "PixelEvent.h"
#include "PlayerEliminatedEvent.h"
#include "GameOverEvent.h"
#include "NewGameEvent.h"

// for debugging unique pointers to char[]
template class std::unique_ptr<char[]>;
template class std::unique_ptr<std::vector<std::string>>;

std::pair<std::unique_ptr<GameEvent>, int> GameEvent::readFrom(const char *buffer)
{
    auto res = StreamUtils::read_int<uint32_t>(buffer);
    uint32_t length = res.first;
    char *location = res.second;
    auto bufferBox = std::make_unique<char[]>(length);
    res = StreamUtils::read_int<uint32_t>(location + length);
    uint32_t expectedChecksum = res.first;
    uint32_t actualChecksum = crc32c(0, (unsigned char *) location, length);
    if (expectedChecksum != actualChecksum) {
        // TODO
    }
    
    HeaderPacked *header = reinterpret_cast<HeaderPacked *>(location);
    switch (header->type) {
        case Type::NEW_GAME: {
            auto packedNoPlayerNames = reinterpret_cast<NewGameEvent::SelfPackedNoPlayerNames *>(location);
            auto playerNames = NewGameEvent::parsePlayerNames(
                    location + sizeof(NewGameEvent::SelfPackedNoPlayerNames),
                    location + length);
            const auto newGameEvent = std::make_unique<NewGameEvent>(*packedNoPlayerNames, playerNames);
            return std::make_pair(newGameEvent, length);
        }
        case Type::PIXEL: {
            auto pixelEvent = std::make_unique<PixelEvent>(*reinterpret_cast<PixelEvent::SelfPacked *>(location));
            return std::make_pair(pixelEvent, length);
        }
        case Type::PLAYER_ELIMINATED: {
            auto eliminatedPacked = reinterpret_cast<PlayerEliminatedEvent::SelfPacked *>(location);
            auto eliminatedEvent = std::make_unique<PlayerEliminatedEvent>(*eliminatedPacked);
            return std::make_pair(eliminatedEvent, length);
        }
        case Type::GAME_OVER: {
            auto gameOverEvent = std::make_unique<GameOverEvent>(*header);
            return std::make_pair(gameOverEvent, length);
        }
    }
}

void GameEvent::writeTo(std::ostream &s)
{
    uint32_t length = getLength();
    auto bufferBox = std::make_unique<char[]>(length);
    char *buffer = bufferBox.get();
    writeToBuffer(buffer);
    uint32_t checksum = crc32c(0, reinterpret_cast<unsigned char *>(buffer), length);
    StreamUtils::write_int<uint32_t>(nullptr, length);
    s.write(buffer, length);
    StreamUtils::write_int<uint32_t>(nullptr, checksum);
}

bool GameEvent::operator==(const GameEvent &other) const
{
    return other.getEventNo() == getEventNo()
           && other.getType() == getType();
}
