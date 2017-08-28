//
// Created by Piotr Jander on 27/08/2017.
//

#include <Socket.h>
#include <ClientMessage.h>
#include <netdb.h>

int main(int argc, const char *argv[]) {
    Socket socket1, socket2, socket3;
    socket1.bindSocket(NULL, "12346");
    socket2.bindSocket(NULL, "12347");
    socket3.bindSocket(NULL, "12348");

    addrinfo *servinfoOther = Socket::getAddrInfo("127.0.0.1", "12345");
    const sockaddr *addr = servinfoOther->ai_addr;
    
    ClientMessage cm1(123, 0, 0, "");
    ClientMessage cm2(456, 0, 0, "Piotr");
    ClientMessage cm3(789, 0, 0, "Jed");

    cm1.sendto(socket1, addr);
    cm2.sendto(socket2, addr);
    cm3.sendto(socket3, addr);

    ClientMessage cm4(456, 1, 0, "Piotr");
    ClientMessage cm5(789, -1, 0, "Jed");

    cm4.sendto(socket2, addr);
    cm5.sendto(socket3, addr);
}
