//
// Created by Piotr Jander on 17/08/2017.
//

#ifndef NIETACKA_CLIENTMESSAGE_H
#define NIETACKA_CLIENTMESSAGE_H


#include <cstdint>
#include <iostream>

using std::string;

#pragma pack(push, 1)
class ClientMessage {
public:
    ClientMessage() = default;

    ClientMessage(uint64_t session_id, int8_t turn_direction, uint32_t next_expected_event_no, char *player_name);

    uint64_t getSession_id() const;

    int8_t getTurn_direction() const;

    uint32_t getNext_expected_event_no() const;

    const char *getPlayer_name() const;

private:
    uint64_t session_id;
    int8_t turn_direction;
    uint32_t next_expected_event_no;
    char player_name[64];
};
#pragma pack(pop)


#endif //NIETACKA_CLIENTMESSAGE_H
