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
    auto *header = (Header *) buffer;
    switch (header->getType()) {
        case Type::NEW_GAME: {
            auto *data = (NewGameEvent::Data *) (buffer + sizeof(*header));
            auto playerNames = NewGameEvent::parsePlayerNames(buffer + sizeof(*header) + sizeof(*data), buffer + length);
            return std::make_unique<NewGameEvent>(*header, *data, playerNames);
        }
        case Type::PIXEL: {
            auto *data = (PixelEvent::Data *) (buffer + sizeof(*header));
            return std::make_unique<PixelEvent>(*header, *data);
        }
        case Type::PLAYER_ELIMINATED: {
            auto *playerNumber = (uint8_t *) (buffer + sizeof(*header));
            return std::make_unique<PlayerEliminatedEvent>(*header, *playerNumber);
        }
        case Type::GAME_OVER: {
            return std::make_unique<GameOverEvent>(*header);
        }
    }
}

void GameEvent::writeTo(std::ostream &s)
{
    uint32_t length = getLength();
    std::unique_ptr<char[]> buffer = getBuffer();
    uint32_t checksum = crc32c(0, (unsigned char *) buffer.get(), length);
    StreamUtils::write_int<uint32_t>(s, length);
    s.write(buffer.get(), length);
    StreamUtils::write_int<uint32_t>(s, checksum);
}

bool GameEvent::operator==(const GameEvent &other) const
{
    return other.header.getEventNo() == header.getEventNo()
           && other.header.getType() == header.getType();
}
