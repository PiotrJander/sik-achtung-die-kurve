#include <iostream>
#include <GameManager.h>
#include <UdpWorker.h>
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, const char *argv[]) {
    std::unique_ptr<IUdpWorker> udpWorker = std::make_unique<UdpWorker>(argc == 2 ? argv[1] : "12345");
    try {
        GameManager gameManager(5, 5, 50, 6, 123, std::move(udpWorker));
        LOG(INFO) << "Entering game loop";
        while (true) {
            gameManager.gameLoop();
        }
    } catch (std::runtime_error &e) {
        LOG(ERROR) << "Runtime error: " << e.what();
    }
}
