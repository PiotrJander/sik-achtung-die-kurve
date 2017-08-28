#include <iostream>
#include <GameManager.h>
#include <UdpWorker.h>
#include <getopt.h>
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

class Arguments {
public:
    Arguments(int argc, char * const *argv)
    {
        port = "12345";
        seed = time(NULL);

        char optstr[] = "W:H:p:s:t:r:";
        int opt;
        while ((opt = getopt(argc, argv, optstr)) != -1) {
            switch (opt) {
                case 'W':
                    width = std::stoi(optarg);
                case 'H':
                    height = std::stoi(optarg);
                case 'p':
                    port = optarg;
                case 's':
                    ROUNDS_PER_SEC = std::stoi(optarg);
                case 't':
                    TURNING_SPEED = std::stoi(optarg);
                case 'r':
                    seed = std::stoll(optarg);
            }
        }
    }

    uint32_t width = 800, height = 600;
    std::string port;
    int ROUNDS_PER_SEC = 50;
    int TURNING_SPEED = 6;
    int64_t seed;
};

int main(int argc, char *argv[])
{
    try {
        Arguments args(argc, argv);

        std::unique_ptr<IUdpWorker> udpWorker = std::make_unique<UdpWorker>(args.port);
        GameManager gameManager(args.width, args.height, args.ROUNDS_PER_SEC, args.TURNING_SPEED, args.seed, std::move(udpWorker));

        LOG(INFO) << "Entering game loop";
        while (true) {
            gameManager.gameLoop();
        }
    } catch (std::runtime_error &e) {
        LOG(ERROR) << "Runtime error: " << e.what();
    }
}
