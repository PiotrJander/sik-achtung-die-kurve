//
// Created by Piotr Jander on 17/08/2017.
//

#ifndef NIETACKA_CLIENTMESSAGE_H
#define NIETACKA_CLIENTMESSAGE_H


#include <cstdint>
#include <iostream>

using std::string;

class ClientMessage {
public:
    ClientMessage(uint64_t session_id, int8_t turn_direction, uint32_t next_expected_event_no,
                  const string &player_name) : session_id(session_id), turn_direction(turn_direction),
                                               next_expected_event_no(next_expected_event_no), player_name(player_name)
    {}

private:
    uint64_t session_id;
    int8_t turn_direction;
    uint32_t next_expected_event_no;
    string player_name;
};


#endif //NIETACKA_CLIENTMESSAGE_H
