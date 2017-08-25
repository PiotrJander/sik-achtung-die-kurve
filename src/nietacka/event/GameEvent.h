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

    /**
     * Header ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    class Header {
    public:
        Header() = default;

        Header(uint32_t eventNo, GameEvent::Type type) : eventNo(htonl(eventNo)), type(type)
        {}

        Header(const HeaderPacked &headerPacked)
                : eventNo(ntohl(headerPacked.eventNo)), type(headerPacked.type)
        {}

        uint32_t getEventNo() const
        {
            return ntohl(eventNo);
        }

        GameEvent::Type getType() const
        {
            return type;
        }

    private:
        uint32_t eventNo;
        GameEvent::Type type;
    };
    /*
     * END Header ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */

    /**
     * HeaderPacked ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    #pragma pack(push, 1)
    class HeaderPacked {
    public:
        HeaderPacked(const GameEvent::Header &header)
                : eventNo(htonl(header.getEventNo())), type(header.getType())
        {}

        uint32_t eventNo;
        GameEvent::Type type;
    };
    #pragma pack(pop)
    /*
     * END HeaderPacked ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */

    /**
     * own methods
     */

    virtual ~GameEvent() = default;

    explicit GameEvent(Header header) : header(header)
    {}

    GameEvent(uint32_t eventNo, Type type) : header(eventNo, type)
    {}

    static std::unique_ptr<GameEvent>
    readFrom(std::istream &);

    void
    writeTo(std::ostream &);

    virtual bool operator==(const GameEvent &other) const;

    Header header;

private:
    virtual uint32_t getLength() = 0;

    virtual std::unique_ptr<char[]> getBuffer() = 0;
};


#endif //PROJECT_GAMEEVENT_H
