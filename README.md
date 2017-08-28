# Achtung Die Kurve clone: network sockets project 

LOG



TODO
use valgrind














BACKLOG
why does pixel test fail on linux?
could still test udp worker; but would need to have abstract interface for socket: dependency inversion
shared_ptr and weak_ptr
maybe cache buffer
test IP v 4 6
learn to use valgrind
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
* copy and swap as exception safety
* vectors as buffers: &myvec[0] is the way to go, also ::resize and ::assign
* buffer: unsigned char
* empty unique pointer: std::unique_ptr<int> foo; compare with optional

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