//
// Created by Piotr Jander on 22/08/2017.
//

#ifndef PROJECT_GAMEEVENT_H
#define PROJECT_GAMEEVENT_H

#include <iostream>


class GameEvent {
public:

    enum class Type : uint8_t {
        NEW_GAME = 0,
        PIXEL = 1,
        PLAYER_ELIMINATED = 2,
        GAME_OVER = 3
    };

    struct HeaderPacked;

    virtual ~GameEvent() = default;

    explicit GameEvent(HeaderPacked header) : eventNo(ntohl(header.eventNo)), type(header.type)
    {}

    GameEvent(uint32_t eventNo, Type type) : eventNo(eventNo), type(type)
    {}

    static std::unique_ptr<GameEvent>
    readFrom(std::istream &);

    void
    writeTo(std::ostream &);

    virtual bool operator==(const GameEvent &other) const;

    uint32_t getEventNo() const
    {
        return eventNo;
    }

    Type getType() const
    {
        return type;
    }

private:
    uint32_t eventNo;
    GameEvent::Type type;

    virtual uint32_t getLength() = 0;

    virtual void writeToBuffer(void *buffer) = 0;
};

#pragma pack(push, 1)
struct GameEvent::HeaderPacked {
    HeaderPacked(const GameEvent &gameEvent)
            : eventNo(htonl(gameEvent.getEventNo())), type(gameEvent.getType())
    {}

    uint32_t eventNo;
    GameEvent::Type type;
};
#pragma pack(pop)


#endif //PROJECT_GAMEEVENT_H
