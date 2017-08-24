//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_NEWGAMEEVENT_H
#define PROJECT_NEWGAMEEVENT_H


#include <vector>
#include "GameEvent.h"

class NewGameEvent: public GameEvent {
public:

    #pragma pack(push, 1)
    class Data {
    public:
        Data(uint32_t maxx, uint32_t maxy) : maxx(htonl(maxx)), maxy(htonl(maxy))
        {}

        uint32_t getMaxx() const
        {
            return ntohl(maxx);
        }

        uint32_t getMaxy() const
        {
            return ntohl(maxy);
        }

    private:
        uint32_t maxx, maxy;
    };
    #pragma pack(pop)

    Data data;

    std::unique_ptr<std::vector<std::string>> playerNames;

    NewGameEvent(uint32_t eventNo, uint32_t maxx, uint32_t maxy);

    NewGameEvent(const GameEvent::Header &header, const NewGameEvent::Data &data,
                 std::vector<std::string> playerNames);

    static std::vector<std::string> parsePlayerNames(char *buffer, const char *endOfBuffer);

private:
    uint32_t getSizeofPlayerNames();

    uint32_t getLength() override;

    std::unique_ptr<char[]> getBuffer() override;
};


#endif //PROJECT_NEWGAMEEVENT_H
