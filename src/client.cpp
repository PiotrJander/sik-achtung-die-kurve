//
// Created by Piotr Jander on 27/08/2017.
//

#include <Socket.h>
#include <ClientMessage.h>
#include <netdb.h>
#include <Client.h>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, const char *argv[])
{
    addrinfo *servinfoOther = Socket::getAddrInfo("127.0.0.1", "12345");

    Client client(servinfoOther->ai_addr);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    while (true) {
        client.loop();
    }

#pragma clang diagnostic pop
}

