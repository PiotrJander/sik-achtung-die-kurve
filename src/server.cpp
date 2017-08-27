#include <iostream>
#include <GameManager.h>
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, const char *argv[]) {
    try {
        GameManager gameManager(5, 5, std::string("12345"), 50, 6, 123);
        LOG(INFO) << "Entering game loop";
        gameManager.gameLoop();
    } catch (std::runtime_error &e) {
        std::cerr << "Runtime error: " << e.what();
    }
}
