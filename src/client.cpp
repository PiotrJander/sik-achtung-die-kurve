//
// Created by Piotr Jander on 27/08/2017.
//

#include <Socket.h>
#include <ClientMessage.h>
#include <netdb.h>

int main(int argc, const char *argv[]) {
    Socket socket1, socket2, socket3;
    // maybe bind here

    addrinfo *servinfoSelf = Socket::getAddrInfo("127.0.0.1", "0");
    addrinfo *servinfoOther = Socket::getAddrInfo("127.0.0.1", "12345");
    
    ClientMessage cm1(123, 0, 0, "");
    ClientMessage cm2(456, 0, 0, "Piotr");
    ClientMessage cm3(789, 0, 0, "Jed");

    cm1.sendto(socket1, servinfoOther->ai_addr);
    cm2.sendto(socket2, servinfoOther->ai_addr);
    cm3.sendto(socket3, servinfoOther->ai_addr);
}
