//
// Created by Piotr Jander on 17/08/2017.
//

#ifndef NIETACKA_MOCKSOCKET_H
#define NIETACKA_MOCKSOCKET_H

#include <sstream>

using std::stringstream;

class MockSocket {
public:
    void serverRead(char* s, std::streamsize n);
    void serverWrite(const char* s, std::streamsize n);
    void clientRead(char* s, std::streamsize n);
    void clientWrite(const char* s, std::streamsize n);

private:
    stringstream serverToClient;
    stringstream clientToServer;
};


#endif //NIETACKA_MOCKSOCKET_H
