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



TODO now
game manager phase 2
start










empty player name also gets events; but can't join a game
immediately after getting client message, enqueue events from game
when new player join during game; they get all events
when player inactive for 2 secs, disconnect; can check it periodically; need to log last access time
    but snake keep moving
when new player joins, check if client name taken yet
if greater session_id, disconnect and connect
if smaller, ignore
method readyForGame: all players have pressed an arrow (non-empty name), and more than one
register time at start of frame so that frame takes 2 secs












DEFAULTS
  -W n – szerokość planszy w pikselach (domyślnie 800)
  -H n – wysokość planszy w pikselach (domyślnie 600)
  -p n – numer portu (domyślnie 12345)
  -s n – liczba całkowita wyznaczająca szybkość gry (parametr
          ROUNDS_PER_SEC w opisie protokołu, domyślnie 50)
  -t n – liczba całkowita wyznaczająca szybkość skrętu (parametr
          TURNING_SPEED, domyślnie 6)
  -r n – ziarno generatora liczb losowych (opisanego poniżej)



BACKLOG
add logging
event length and crc32; maybe refactor writeTo and readFrom

ICEBOX
pusta nazwa gracza jako ""
streamUtils may not be used with sockets!
test conversion hton
check field order in structs
use poll in UDPWorker
validate messages

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
