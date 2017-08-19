//
// Created by Piotr Jander on 17/08/2017.
//

#ifndef NIETACKA_MOCKSOCKET_H
#define NIETACKA_MOCKSOCKET_H

#include <sstream>
//#include <memory>

using std::stringstream;
using std::shared_ptr;

class MockSocket {
public:
    MockSocket();

    shared_ptr<stringstream> serverRead();

    shared_ptr<stringstream> serverWrite();

    shared_ptr<stringstream> clientRead();

    shared_ptr<stringstream> clientWrite();

private:
    shared_ptr<stringstream> serverToClient;
    shared_ptr<stringstream> clientToServer;
};


#endif //NIETACKA_MOCKSOCKET_H
