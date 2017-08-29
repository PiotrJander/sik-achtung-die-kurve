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
#include "../ReadBuffer.h"
#include "../Exceptions.h"

// for debugging unique pointers to char[]
template class std::unique_ptr<char[]>;
template class std::unique_ptr<std::vector<std::string>>;

std::unique_ptr<GameEvent> GameEvent::read(ReadBuffer &readBuffer, int length)
{
    const HeaderPacked *header = reinterpret_cast<const HeaderPacked *>(readBuffer.peek());
    switch (header->type) {
        case Type::NEW_GAME: {
            if (length <= sizeof(NewGameEvent::SelfPackedNoPlayerNames)) {
                throw ParseException("Event length mismatch");
            }
            auto packedNoPlayerNames = readBuffer.castAndAdvance<NewGameEvent::SelfPackedNoPlayerNames>();
            
            std::vector<std::string> names;
            int len = length - sizeof(NewGameEvent::SelfPackedNoPlayerNames);
            
            while (len) {
                std::string name = readBuffer.readString(length);
                len -= name.size() - 1;
                names.emplace_back(std::move(name));
            }
            
            return std::make_unique<NewGameEvent>(*packedNoPlayerNames, names);
        }
        case Type::PIXEL: {
            if (length != sizeof(PixelEvent::SelfPacked)) {
                throw ParseException("Event length mismatch");
            }
            return std::make_unique<PixelEvent>(*readBuffer.castAndAdvance<PixelEvent::SelfPacked>());
        }
        case Type::PLAYER_ELIMINATED: {
            if (length != sizeof(PlayerEliminatedEvent::SelfPacked)) {
                throw ParseException("Event length mismatch");
            }
            return std::make_unique<PlayerEliminatedEvent>(*readBuffer.castAndAdvance<PlayerEliminatedEvent::SelfPacked>());
        }
        case Type::GAME_OVER: {
            if (length != sizeof(HeaderPacked)) {
                throw ParseException("Event length mismatch");
            }
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
