//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_PLAYERELIMINATEDEVENT_H
#define PROJECT_PLAYERELIMINATEDEVENT_H


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

//    PlayerEliminatedEvent(const PlayerEliminatedEvent& event)
//            : GameEvent(event), playerNumber(event.playerNumber)
//    {}

    bool operator==(const GameEvent &other) const override;

    uint8_t getPlayerNumber() const
    {
        return playerNumber;
    }

private:
    uint8_t playerNumber;

    uint32_t getLength() override {
        return sizeof(SelfPacked);
    }

    void writeToBuffer(char *buffer) override;
};

#endif //PROJECT_PLAYERELIMINATEDEVENT_H
