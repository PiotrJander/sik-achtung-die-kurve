//
// Created by Piotr Jander on 17/08/2017.
//

#ifndef NIETACKA_CLIENTMESSAGE_H
#define NIETACKA_CLIENTMESSAGE_H


#include <cstdint>
#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include "Socket.h"
#include "Exceptions.h"

#ifdef __linux__
#include <endian.h>
#define htonll htobe64
#define ntohll be64toh
#endif

using std::string;

class ClientMessage {
public:

    /*
     * packed structs ~~~~~~~~~~~~~~~~~~~~~
     */
#pragma pack(push, 1)
    struct SelfPacked {
        SelfPacked(const ClientMessage &clientMessage)
                : sessionId(htonll(clientMessage.getSessionId())),
                  turnDirection(clientMessage.getTurnDirection()),
                  nextExpectedEventNo(htonl(clientMessage.getNextExpectedEventNo()))
        {
            strncpy(playerName, clientMessage.getPlayerName().c_str(), 64);
        }

        SelfPacked() = default;

        uint64_t sessionId;
        int8_t turnDirection;
        uint32_t nextExpectedEventNo;
        char playerName[64];
    };
#pragma pack(pop)
    /*
     * END packed structs ~~~~~~~~~~~~~~~~~~~~~~
     */

    ClientMessage(uint64_t session_id, int8_t turn_direction, uint32_t next_expected_event_no,
                  const string &player_name);

    ClientMessage(const SelfPacked &packed)
            : sessionId(ntohll(packed.sessionId)),
              turnDirection(packed.turnDirection),
              nextExpectedEventNo(ntohl(packed.nextExpectedEventNo)),
              playerName(packed.playerName, 0, 64)
    {
        if (!(turnDirection == -1 || turnDirection == 0 || turnDirection == 1)) {
            throw ProtocolException("Invalid turn direction");
        }

        for (char &c : playerName) {
            if (c < 33 || c > 126) {
                throw ProtocolException("Invalid name");
            }
        }
    }

    uint64_t getSessionId() const;

    int8_t getTurnDirection() const;

    uint32_t getNextExpectedEventNo() const;

    const std::string &getPlayerName() const;

    void sendto(Socket &socket, const sockaddr *addr);

private:
    uint64_t sessionId;
    int8_t turnDirection;
    uint32_t nextExpectedEventNo;
    std::string playerName;
};

#endif //NIETACKA_CLIENTMESSAGE_H
