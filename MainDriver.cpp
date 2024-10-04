#include <iostream>

#include "MapFiles/MapDriver.h"
#include "CardsFiles/CardsDriver.h"
#include "PlayerFiles/PlayerDriver.h"

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
        std::cout << "No specific tests selected. Running all tests...\n";
        testLoadMaps();
        testCards();
    } else {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "testLoadMaps") {
                std::cout << "Running test: testLoadMaps...\n";
                testLoadMaps();
            }
            if (arg == "testCards") {
                std::cout << "Running test: testLoadMaps...\n";
                testCards();
            }
            if (arg == "testPlayers") {
                std::cout << "Running test: testLoadMaps...\n";
                testPlayers();
            }
            else {
                std::cerr << "Unknown test: " << arg << "\n";
            }
        }
    }

    return 0;
}
