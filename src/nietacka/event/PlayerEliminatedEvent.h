//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_PLAYERELIMINATEDEVENT_H
#define PROJECT_PLAYERELIMINATEDEVENT_H


#include "GameEvent.h"

class PlayerEliminatedEvent: public GameEvent {
public:
    PlayerEliminatedEvent(const GameEvent::Header &header, uint8_t playerNumber)
            : GameEvent(header),
              playerNumber(playerNumber)
    {};

    PlayerEliminatedEvent(uint32_t eventNo, uint8_t playerNumber) : GameEvent(eventNo, Type::PLAYER_ELIMINATED),
                                                                               playerNumber(playerNumber)
    {}

    uint8_t playerNumber;

    bool operator==(const GameEvent &other) const override;

private:
    uint32_t getLength() override {
        return sizeof(header) + sizeof(playerNumber);
    }

    std::unique_ptr<char[]> getBuffer() override;
};


#endif //PROJECT_PLAYERELIMINATEDEVENT_H
