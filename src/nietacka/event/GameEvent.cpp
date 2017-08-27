//
// Created by Piotr Jander on 22/08/2017.
//

#include <sstream>
#include "GameEvent.h"
#include "../crc32c.h"
#include "PixelEvent.h"
#include "PlayerEliminatedEvent.h"
#include "GameOverEvent.h"
#include "NewGameEvent.h"

// for debugging unique pointers to char[]
template class std::unique_ptr<char[]>;
template class std::unique_ptr<std::vector<std::string>>;

std::unique_ptr<GameEvent> GameEvent::readFrom(const char *buffer, uint32_t length)
{
    const HeaderPacked *header = reinterpret_cast<const HeaderPacked *>(buffer);
    switch (header->type) {
        case Type::NEW_GAME: {
            auto packedNoPlayerNames = reinterpret_cast<const NewGameEvent::SelfPackedNoPlayerNames *>(buffer);
            auto playerNames = NewGameEvent::parsePlayerNames(
                    buffer + sizeof(NewGameEvent::SelfPackedNoPlayerNames),
                    buffer + length);
            return std::make_unique<NewGameEvent>(*packedNoPlayerNames, playerNames);
        }
        case Type::PIXEL: {
            return std::make_unique<PixelEvent>(*reinterpret_cast<const PixelEvent::SelfPacked *>(buffer));
        }
        case Type::PLAYER_ELIMINATED: {
            auto eliminatedPacked = reinterpret_cast<const PlayerEliminatedEvent::SelfPacked *>(buffer);
            return std::make_unique<PlayerEliminatedEvent>(*eliminatedPacked);
        }
        case Type::GAME_OVER: {
            return std::make_unique<GameOverEvent>(*header);
        }
    }
}

uint32_t GameEvent::writeTo(char *buffer)
{
    uint32_t length = getLength();
    char *bufferlocation = buffer;
    *reinterpret_cast<uint32_t *>(bufferlocation) = htonl(length);
    bufferlocation += sizeof(uint32_t);
    writeToBuffer(bufferlocation);
    uint32_t checksum = crc32c(0, reinterpret_cast<unsigned char *>(bufferlocation), length);
    bufferlocation += length;
    *reinterpret_cast<uint32_t *>(bufferlocation) = htonl(checksum);
    return length + 2 * sizeof(uint32_t);
}

bool GameEvent::operator==(const GameEvent &other) const
{
    return other.getEventNo() == getEventNo()
           && other.getType() == getType();
}
