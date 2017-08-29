//
// Created by Piotr Jander on 22/08/2017.
//

#ifndef PROJECT_GAMEEVENT_H
#define PROJECT_GAMEEVENT_H

#include <iostream>
#include <memory>
#include <netinet/in.h>
#include "../DynamicBuffer.h"
#include "../ReadBuffer.h"


class GameEvent {
public:

    enum class Type : uint8_t {
        NEW_GAME = 0,
        PIXEL = 1,
        PLAYER_ELIMINATED = 2,
        GAME_OVER = 3
    };

    /*
     * packed structs ~~~~~~~~~~~~~~~~~~~~~
     */
#pragma pack(push, 1)
    struct HeaderPacked {
        HeaderPacked(const GameEvent &gameEvent)
                : eventNo(htonl(gameEvent.getEventNo())), type(gameEvent.getType())
        {}

        uint32_t eventNo;
        GameEvent::Type type;
    };
#pragma pack(pop)
    /*
     * END packed structs ~~~~~~~~~~~~~~~~~~~~~~
     */

    virtual ~GameEvent() = default;

    explicit GameEvent(HeaderPacked header) : eventNo(ntohl(header.eventNo)), type(header.type)
    {}


    GameEvent(uint32_t eventNo, Type type) : eventNo(eventNo), type(type)
    {}

    void write(DynamicBuffer &buffer);

    static std::unique_ptr<GameEvent> read(ReadBuffer &readBuffer);

    virtual bool operator==(const GameEvent &other) const;

    uint32_t getEventNo() const
    {
        return eventNo;
    }

    Type getType() const
    {
        return type;
    }

    uint32_t getLength() const
    {
        return selfLength() + 2 * sizeof(uint32_t);
    }

private:
    uint32_t eventNo;
    GameEvent::Type type;

    static std::unique_ptr<GameEvent> readSelf(ReadBuffer &readBuffer, int length);

    virtual uint32_t selfLength() const = 0;

    virtual void writeSelf(DynamicBuffer &buffer) = 0;
};

#endif //PROJECT_GAMEEVENT_H
