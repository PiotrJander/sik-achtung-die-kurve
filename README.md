# Achtung Die Kurve clone: network sockets project 

TODO now























BACKLOG
pusta nazwa gracza jako ""
streamUtils may not be used with sockets!
refactor from session_id to (socket, session_id); get socket from struct sockaddr
ConnectedPlayer would store struct sockaddr and its length
test conversion hton
check field order in structs
add logging
use poll in UDPWorker
refactor game, add isInProgress(), don't return bool
validate messages

INTERFACES

client messages fixed length, but can be any number of them: dynamically allocate

interface IDatagram<size>, impl by EventBatch
    stores ref to event history on game, start and end position
    stores ref to player connections
    stores length
    stores player id (hash, session)
    unique ptr to buffer (pair?) makeBuffer()
    getLength()
    getStructSockaddr (through player connection)
    and its length?

interface IUDPWorker
    enqueue(const Datagram &)
    unique pointer to buffer dequeue()
    workUntil(time of next frame)

PROGRAM SKELETON

init
loop {
    // wait for game
    do
        every 20 ms
            while (UDPWorker.dequeueRecv) process client messages
    until ready for game

    // play game
    init game
    while game.isInProgress()
        start_of_frame = time now
        while (UDPWorker.dequeueRecv) process client messages
        game.tick()
        for datagram in client_events
            UDPWorker.enqueueSend(IDatagram)
        UDPWorker.workUntil(start_of_frame + frame)
}
maybe graceful exit on signal

Qs:
64 bit random numbers but 32 bits game id
what if we don't send all send Q in the time frame?

UNDERSTANDING C++:
* what to put in header files, what in cpp files?

TCP CAVEATS
some messages can be variable length; we might need to encapsulate to know where a message starts and ends
    sort of sendall
    same goes for receiving: need readall; if variable length, need a header with length
    but we might get some of next packet! circular buffer for this!
        https://stackoverflow.com/questions/3621956/how-do-i-code-a-simple-integer-circular-buffer-in-c-c

POLL CAVEAT
caveat: even if we use select, we might still need to make the socket non-blocking

UDP
partial reads of UPD datagrams: https://stackoverflow.com/questions/3069204/reading-partially-from-sockets
