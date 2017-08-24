//
// Created by Piotr Jander on 24/08/2017.
//

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H


#include <string>

class PlayerConnection {
public:
    const std::string &getName() const
    {
        return name;
    }

    bool operator < (const PlayerConnection& pc) const
    {
        return name < pc.name;
    }

private:
    int socket, session_id;
    std::string name;
};


#endif //PROJECT_PLAYER_H
