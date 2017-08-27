# Achtung Die Kurve clone: network sockets project 

LOG

start 10:55
end 11:00
impl interfaces

ConnectedPlayer would store struct sockaddr and its length
refactor from session_id to socket hash; get socket from struct sockaddr
start 11:02
end 13:36
total 2:30

game manager phase 1
start 15:50
end 17:50
2h

game manager phase 2
start 18:33
end 19:40
total 1:07

stream utils and fix bugs
start 20:30
end 21:10
total 40 min

refactor event read and write
start 21:12
end 22:16
total 1h

TODO now
tests
start 1:40
end

TODO
when writing game id, length, crc, itp, to buffer, need to hton or ntoh!!!

























BACKLOG
add logging

ICEBOX
when player inactive for 2 secs, disconnect; can check it periodically; need to log last access time but snake keep moving
pusta nazwa gracza jako ""
test conversion hton
check field order in structs
use poll in UDPWorker
validate messages
handle all kinds of exceptions!
strange game test failures with hashing

Qs:
64 bit random numbers but 32 bits game id
what if we don't send all send Q in the time frame?

UNDERSTANDING C++:
* what to put in header files, what in cpp files?
* pass functors rather than function pointers

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