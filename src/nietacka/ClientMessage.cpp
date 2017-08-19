//
// Created by Piotr Jander on 17/08/2017.
//

#include "ClientMessage.h"

ClientMessage::ClientMessage(uint64_t session_id, int8_t turn_direction, uint32_t next_expected_event_no,
                             char *player_name) : session_id(htonll(session_id)), turn_direction(turn_direction),
                                                  next_expected_event_no(htonl(next_expected_event_no))
{
    strcpy(this->player_name, player_name);
}

uint64_t ClientMessage::getSession_id() const
{
    return ntohll(session_id);
}

int8_t ClientMessage::getTurn_direction() const
{
    return turn_direction;
}

uint32_t ClientMessage::getNext_expected_event_no() const
{
    return ntohl(next_expected_event_no);
}

const char *ClientMessage::getPlayer_name() const
{
    return player_name;
}
