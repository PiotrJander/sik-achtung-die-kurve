//
// Created by Piotr Jander on 19/08/2017.
//

#include <sstream>
//#include <string>
#include "gtest/gtest.h"
#include <nietacka/ClientMessage.h>

//using namespace std::string_literals;

TEST(ClientMessage__Test, First)
{
    uint64_t session_id = 123;
    int8_t turn_direction = 1;
    uint32_t next_expected_event_no = 456;
    char player_name[64] = "PiotrJander";

    ClientMessage writeCM(session_id, turn_direction, next_expected_event_no, player_name);
    ClientMessage readCM;

    std::stringstream stream;
    stream.write((char *) &writeCM, sizeof(writeCM));
    stream.read((char *) &readCM, sizeof(readCM));

    ASSERT_EQ(session_id, readCM.getSession_id());
    ASSERT_EQ(turn_direction, readCM.getTurn_direction());
    ASSERT_EQ(next_expected_event_no, readCM.getNext_expected_event_no());
    ASSERT_EQ(0, strcmp(player_name, readCM.getPlayer_name()));
}