//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_GAMEEVENTHEADER_H
#define PROJECT_GAMEEVENTHEADER_H


#include <cstdint>
#include <i386/endian.h>


enum class EventType : uint8_t {
    NEW_GAME = 0,
    PIXEL = 1,
    PLAYER_ELIMINATED = 2,
    GAME_OVER = 3
};


#pragma pack(push, 1)
class GameEventHeader {
public:
    GameEventHeader() = default;

    GameEventHeader(uint32_t eventNo, EventType type) : eventNo(htonl(eventNo)), type(type)
    {}

    uint32_t getEventNo() const
    {
        return ntohl(eventNo);
    }

    EventType getType() const
    {
        return type;
    }

private:
    uint32_t eventNo;
    EventType type;
};
#pragma pack(pop)


#endif //PROJECT_GAMEEVENTHEADER_H
