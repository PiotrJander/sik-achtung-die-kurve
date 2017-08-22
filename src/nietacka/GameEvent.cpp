//
// Created by Piotr Jander on 22/08/2017.
//

#include "GameEvent.h"
#include "StreamUtils.h"
#include "crc32c.h"
#include "Event_Pixel.h"

void GameEvent::writeTo(std::ostream &s)
{
    StreamUtils::write_int<uint32_t>(s, htonl(this->getLength()));
    s.write(this->getData(), this->getLength());
    StreamUtils::write_int<uint32_t>(s, htonl(this->getChecksum()));
}

uint32_t GameEvent::getLength()
{
    return sizeof(*this);
}

char *GameEvent::getData()
{
    return (char *) this;
}

uint32_t GameEvent::getChecksum()
{
    return crc32c(0, (unsigned char *) this->getData(), this->getLength());
}

GameEvent::GameEvent() : eventNumber(0), type(EventType::NEW_GAME)
{}

uint32_t GameEvent::getEventNumber() const
{
    return ntohl(eventNumber);
}

const EventType GameEvent::getType() const
{
    return type;
}

std::unique_ptr<GameEvent> GameEvent::readFrom(std::istream &s)
{
    auto length = StreamUtils::read_int<uint32_t>(s);
    length = ntohl(length);
    char buffer[length];
    s.read(buffer, length);
    auto expectedChecksum = StreamUtils::read_int<uint32_t>(s);
    uint32_t actualChecksum = crc32c(0, (unsigned char *) buffer, length);
    if (expectedChecksum != actualChecksum) {
        // TODO raise error here
    }
    GameEvent baseEvent;
    std::memcpy(&baseEvent, buffer, sizeof(baseEvent));
    switch (baseEvent.getType()) {
        case EventType::PIXEL:
            return std::move(std::make_unique<Event_Pixel>((char *) buffer));
        default:
            return nullptr;
    }
}

GameEvent::GameEvent(uint32_t number, const EventType type) : eventNumber(htonl(number)), type(type)
{}
