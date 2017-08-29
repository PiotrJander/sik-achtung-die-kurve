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
#include "../Exceptions.h"


std::unique_ptr<GameEvent> GameEvent::read(ReadBuffer &readBuffer)
{
    try {
        uint32_t length, expectedChecksum;
        readBuffer >> length;
        length = ntohl(length);
        const char *startOfEvent = readBuffer.peek();

        auto event = readSelf(readBuffer, length);

        if (length != event->selfLength()) {
            throw ParseException("Event length mismatch");
        }

        readBuffer >> expectedChecksum;
        expectedChecksum = ntohl(expectedChecksum);
        uint32_t actualChecksum = crc32c(0, reinterpret_cast<const unsigned char *>(startOfEvent), length);

        //    if (expectedChecksum != actualChecksum) {
        //        throw ParseException("Bad checksum");
        //    }

        return event;
    } catch (ReadBufferException &e) {
        throw ParseException("Trying to read past the buffer");
    }
}

std::unique_ptr<GameEvent> GameEvent::readSelf(ReadBuffer &readBuffer, int length)
{
    const HeaderPacked *header = reinterpret_cast<const HeaderPacked *>(readBuffer.peek());
    switch (header->type) {
        case Type::NEW_GAME: {
            auto packedNoPlayerNames = readBuffer.castAndAdvance<NewGameEvent::SelfPackedNoPlayerNames>();

            std::vector<std::string> names;
            int len = length - sizeof(NewGameEvent::SelfPackedNoPlayerNames);
            
            while (len) {
                std::string name = readBuffer.readString(len);
                len -= name.size() + 1;
                names.emplace_back(move(name));
            }
            
            return std::make_unique<NewGameEvent>(*packedNoPlayerNames, names);
        }
        case Type::PIXEL: {
            return std::make_unique<PixelEvent>(*readBuffer.castAndAdvance<PixelEvent::SelfPacked>());
        }
        case Type::PLAYER_ELIMINATED: {
            return std::make_unique<PlayerEliminatedEvent>(*readBuffer.castAndAdvance<PlayerEliminatedEvent::SelfPacked>());
        }
        case Type::GAME_OVER: {
            return std::make_unique<GameOverEvent>(*readBuffer.castAndAdvance<HeaderPacked>());
        }
    }
}

bool GameEvent::operator==(const GameEvent &other) const
{
    return other.getEventNo() == getEventNo()
           && other.getType() == getType();
}

void GameEvent::write(DynamicBuffer &buffer)
{
    uint32_t length = selfLength();
    buffer << htonl(length);
    const char *start = buffer.currentLocation();
    writeSelf(buffer);
    uint32_t checksum = crc32c(0, (const unsigned char *) start, length);
    buffer << htonl(checksum);
}
