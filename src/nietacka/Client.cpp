//
// Created by Piotr Jander on 29/08/2017.
//

#include <chrono>
#include <poll.h>
#include "Client.h"
#include "easylogging++.h"

using namespace std::chrono;

Client::Client(sockaddr *serverAddr)
        : socket(),
          message(123, -1, 0, "Piotr"),
          serverAddr(serverAddr)
{
    socket.setNonBlocking();
}

void Client::loop()
{
    // 20 ms slices
    milliseconds endOfFrame =
            duration_cast<milliseconds>(system_clock::now().time_since_epoch()) + milliseconds(20);

    // send message
    try {
        ClientMessage::SelfPacked packed(message);
        socket.sendTo(&packed, sizeof(ClientMessage::SelfPacked), serverAddr);
        LOG(INFO) << "Sending client message to server";
    } catch (WouldBlockException &e) {
        LOG(WARNING) << e.what();
    } catch (InterruptedException &e) {}

    // receive messages until end of time slice
    while (true) {
        milliseconds remainingTime =
                endOfFrame - duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        long long remainingTimeLong = remainingTime.count();

        if (remainingTimeLong <= 0) {
            break;
        }

        short rc=0;
        try {
            rc = socket.socketPoll(remainingTimeLong, false);
        } catch (InterruptedException &e) {
            continue;
        }

        if (rc == 0) {
            // timeout
            break;
        } else {
            if (rc & POLLIN) {
                try {
                    char buffer[512];
                    // in fact should check for the sender
                    socket.recvFrom(buffer, 512);
                    LOG(INFO) << "Receiving event batch from server";
                } catch (WouldBlockException &e) {
                    LOG(WARNING) << e.what();
                }
            }
        }
    }
}

