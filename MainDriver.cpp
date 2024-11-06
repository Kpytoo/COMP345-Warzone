#include <iostream>

#include "MapFiles/MapDriver.h"
#include "CardsFiles/CardsDriver.h"
#include "PlayerFiles/PlayerDriver.h"
#include "GameEngineFiles/GameEngineDriver.h"
#include "OrdersFiles/OrdersDriver.h"

/**
 * @brief Main function that runs specified test cases based on command-line arguments.
 *
 * If no arguments are provided, it runs all available tests (currently just testLoadMaps).
 * If specific test names are provided as arguments, it runs only those tests.
 * Unknown test names result in an error message.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int Returns 0 if successful, otherwise returns an error code.
 */
int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "No specific tests selected. Running all tests... \n(except for game engine, testGameStates arg is required)\n";
        testLoadMaps();
        testCards();
        testPlayers();
        testOrdersLists();
    } else {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "testLoadMaps") {
                std::cout << "Running test: testLoadMaps...\n";
                testLoadMaps();
            }
            else if (arg == "testCards") {
                std::cout << "Running test: testCards...\n";
                testCards();
            }
            else if (arg == "testPlayers") {
                std::cout << "Running test: testPlayers...\n";
                testPlayers();
            }
            else if (arg == "testGameStates") {
                std::cout << "Running test: testGameStates...\n";
                testGameStates();
            }
            else if (arg == "testOrdersLists") {
                std::cout << "Running test: testOrdersLists...\n";
                testOrdersLists();
            }
            else {
                std::cerr << "Unknown test: " << arg << "\n";
            }
        }
    }

    return 0;
}
