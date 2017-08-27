//
// Created by Piotr Jander on 17/08/2017.
//

#include "ClientMessage.h"

ClientMessage::ClientMessage(uint64_t session_id, int8_t turn_direction, uint32_t next_expected_event_no,
                             const string &player_name)
        : sessionId(session_id),
          turnDirection(turn_direction),
          nextExpectedEventNo(next_expected_event_no),
          playerName(player_name)
{}

uint64_t ClientMessage::getSessionId() const
{
    return sessionId;
}

int8_t ClientMessage::getTurnDirection() const
{
    return turnDirection;
}

uint32_t ClientMessage::getNextExpectedEventNo() const
{
    return nextExpectedEventNo;
}

const std::string &ClientMessage::getPlayerName() const
{
    return playerName;
}

void ClientMessage::sendto(Socket &socket, const sockaddr *addr)
{
    SelfPacked packed(*this);
    socket.sendTo(&packed, sizeof(SelfPacked), addr);
}
