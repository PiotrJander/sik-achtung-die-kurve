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

std::unique_ptr<GameEvent> GameEvent::readFrom(std::istream &s)
{
    auto length = StreamUtils::read_int<uint32_t>(s);
    char buffer[length];
    s.read(buffer, length);
    auto expectedChecksum = StreamUtils::read_int<uint32_t>(s);
    uint32_t actualChecksum = crc32c(0, (unsigned char *) buffer, length);
    if (expectedChecksum != actualChecksum) {
        // TODO
    }
    HeaderPacked *header = reinterpret_cast<HeaderPacked *>(buffer);
    switch (header->type) {
        case Type::NEW_GAME: {
            auto packedNoPlayerNames = reinterpret_cast<NewGameEvent::SelfPackedNoPlayerNames *>(buffer);
            auto playerNames = NewGameEvent::parsePlayerNames(
                    buffer + sizeof(NewGameEvent::SelfPackedNoPlayerNames),
                    buffer + length);
            return std::make_unique<NewGameEvent>(*packedNoPlayerNames, playerNames);
        }
        case Type::PIXEL: {
            return std::make_unique<PixelEvent>(*reinterpret_cast<PixelEvent::SelfPacked *>(buffer));
        }
        case Type::PLAYER_ELIMINATED: {
            auto eliminatedPacked = reinterpret_cast<PlayerEliminatedEvent::SelfPacked *>(buffer);
            return std::make_unique<PlayerEliminatedEvent>(*eliminatedPacked);
        }
        case Type::GAME_OVER: {
            return std::make_unique<GameOverEvent>(*header);
        }
    }
}

void GameEvent::writeTo(std::ostream &s)
{
    uint32_t length = getLength();
    unsigned char buffer[length];
    writeToBuffer(buffer);
    uint32_t checksum = crc32c(0, reinterpret_cast<unsigned char *>(buffer), length);
    StreamUtils::write_int<uint32_t>(s, length);
    s.write(reinterpret_cast<const char *>(buffer), length);
    StreamUtils::write_int<uint32_t>(s, checksum);
}

bool GameEvent::operator==(const GameEvent &other) const
{
    return other.getEventNo() == getEventNo()
           && other.getType() == getType();
}
