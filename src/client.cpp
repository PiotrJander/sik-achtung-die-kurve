//
// Created by Piotr Jander on 27/08/2017.
//

#include <Socket.h>
#include <ClientMessage.h>
#include <netdb.h>
#include <thread>
#include <chrono>

int main(int argc, const char *argv[]) {
    Socket socket1, socket2, socket3;
    socket1.bindSocket(NULL, "8071");

    addrinfo *servinfoOther = Socket::getAddrInfo("127.0.0.1", "8070");
    const sockaddr *addr = servinfoOther->ai_addr;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (uint32_t i = 0; ; ++i) {
        ClientMessage cm(789, 1, i, "Jed");
        cm.sendto(socket1, addr);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
#pragma clang diagnostic pop

//    ClientMessage cm3(789, 1, 0, "Jed");
//
//    socket2.bindSocket(NULL, "8072");
//    socket3.bindSocket(NULL, "8073");
//
//    addrinfo *servinfoOther = Socket::getAddrInfo("127.0.0.1", "8070");
//    const sockaddr *addr = servinfoOther->ai_addr;
//
//    ClientMessage cm1(123, 0, 0, "");
//    ClientMessage cm2(456, 0, 0, "Piotr");
//    ClientMessage cm3(789, 0, 0, "Jed");
//
//    cm1.sendto(socket1, addr);
//    cm2.sendto(socket2, addr);
//    cm3.sendto(socket3, addr);
//
////    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
//
//    ClientMessage cm4(456, 1, 0, "Piotr");
//    ClientMessage cm5(789, -1, 0, "Jed");
//
//    cm4.sendto(socket2, addr);
//    cm5.sendto(socket3, addr);
}
