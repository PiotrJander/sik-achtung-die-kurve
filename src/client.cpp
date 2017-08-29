//
// Created by Piotr Jander on 27/08/2017.
//

#include <Socket.h>
#include <ClientMessage.h>
#include <netdb.h>
#include <thread>
#include <chrono>

int main(int argc, const char *argv[]) {
    Socket socket;
    socket.bindSocket(NULL, "8071");

    addrinfo *servinfoOther = Socket::getAddrInfo("127.0.0.1", "8070");
    const sockaddr *addr = servinfoOther->ai_addr;

    ClientMessage cm(789, 1, 0, "Jed");

    while (true) {

        socket.socketPoll(20, true);
    }

    // now we could block on send every 20 ms
    // then we use poll to listen only
    // otherwise we can have a one elem queue
    // optional?

}
