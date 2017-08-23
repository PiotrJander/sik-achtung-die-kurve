//
// Created by Piotr Jander on 22/08/2017.
//

#include <sstream>
#include "GameEvent.h"
#include "StreamUtils.h"
#include "crc32c.h"
#include "PixelEventData.h"
#include "PixelEvent.h"

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
    GameEventHeader header;
    std::memcpy(&header, buffer, sizeof(header));
//    std::unique_ptr<GameEvent> event;
    switch (header.getType()) {
        case EventType::PIXEL:
            PixelEventData data;
            std::memcpy(&data, buffer + sizeof(header), sizeof(data));
            return std::make_unique<PixelEvent>(header, data);
        default:
            return nullptr;
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
