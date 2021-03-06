//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_PLAYERELIMINATEDEVENT_H
#define PROJECT_PLAYERELIMINATEDEVENT_H


#include "../DynamicBuffer.h"
#include "GameEvent.h"
#include "../easylogging++.h"

class PlayerEliminatedEvent: public GameEvent {
public:

    /*
     * packed structs ~~~~~~~~~~~~~~~~~~~~~
     */
#pragma pack(push, 1)
    struct SelfPacked {
        SelfPacked(const PlayerEliminatedEvent &eliminatedEvent)
                : header(eliminatedEvent), playerNumber(eliminatedEvent.getPlayerNumber())
        {}

        HeaderPacked header;
        uint8_t playerNumber;
    };
#pragma pack(pop)
    /*
     * END packed structs ~~~~~~~~~~~~~~~~~~~~~~
     */

    PlayerEliminatedEvent(const SelfPacked &packed)
            : GameEvent(packed.header),
              playerNumber(packed.playerNumber)
    {};

    PlayerEliminatedEvent(uint32_t eventNo, uint8_t playerNumber)
            : GameEvent(eventNo, Type::PLAYER_ELIMINATED), playerNumber(playerNumber)
    {
        LOG(INFO) << "PlayerEliminatedEvent";
    }

    bool operator==(const GameEvent &other) const override;

    uint8_t getPlayerNumber() const
    {
        return playerNumber;
    }

    void writeSelf(DynamicBuffer &buffer) override;

private:
    uint8_t playerNumber;

    uint32_t selfLength() const override
    {
        return sizeof(SelfPacked);
    }
};

#endif //PROJECT_PLAYERELIMINATEDEVENT_H
