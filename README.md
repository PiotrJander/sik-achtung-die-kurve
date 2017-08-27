# Achtung Die Kurve clone: network sockets project 

LOG

reading about C++
start 11:40
end 12:50
total 1:10

impl udp worker
start 12:50
end 14:10
total 1:20

TODO
now test what we have
start 15:00

now socket also needs to be able to sendto

make a client which will send a few messages to the server
view logs to see they came through
assert that sockaddr is set okay for client













BACKLOG
add logging

ICEBOX
when player inactive for 2 secs, disconnect; can check it periodically; need to log last access time but snake keep moving
pusta nazwa gracza jako ""
test conversion hton
check field order in structs
validate messages
handle all kinds of exceptions!
client: remember about hton!
use vectors (contiguoes storage) rather than arrays
let PlayerConnection impl two interfaces; one for Game and other for sockets
what if datagram bigger than size of buffer?
no better way to have polymorphic class members and containers than unique_ptr?

Qs:
64 bit random numbers but 32 bits game id
what if we don't send all send Q in the time frame?

UNDERSTANDING C++:
* what to put in header files, what in cpp files?
* pass functors rather than function pointers
* copy and swap as exception safety
* vectors as buffers: &myvec[0] is the way to go, also ::resize and ::assign
* we use structs as buffers, except for EventBatch
* we can define one ctor in terms of another
* buffer: unsigned char?

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

CLIENT read events
const char *bufferLocation = buffer;
uint32_t length = *reinterpret_cast<const uint32_t *>(bufferLocation);
bufferLocation += sizeof(uint32_t);
uint32_t expectedChecksum = *reinterpret_cast<const uint32_t *>(bufferLocation + length);
uint32_t actualChecksum = crc32c(0, (unsigned const char *) bufferLocation, length);
if (expectedChecksum != actualChecksum) {
    // TODO
}