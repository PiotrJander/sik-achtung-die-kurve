//
// Created by Piotr Jander on 29/08/2017.
//

#include <Socket.h>
#include <ClientMessage.h>
#include <netdb.h>
#include <thread>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, const char *argv[])
{
    Socket socket;
    socket.bindSocket(NULL, "8017");

    addrinfo *servinfoOther = Socket::getAddrInfo("127.0.0.1", "12345");
    const sockaddr *addr = servinfoOther->ai_addr;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (uint32_t i = 0;; ++i) {
        ClientMessage cm(789, 1, i, "Jed");
        cm.sendto(socket, addr);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
#pragma clang diagnostic pop
}