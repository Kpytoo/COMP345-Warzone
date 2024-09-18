#include <iostream>

#include "MapFiles/MapDriver.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "No specific tests selected. Running all tests...\n";
        testLoadMaps();
        // add other test driver calls here
    } else {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "testLoadMaps") {
                std::cout << "Running test: testLoadMaps...\n";
                testLoadMaps();
            }
                // Add other test driver function calls here
            else {
                std::cerr << "Unknown test: " << arg << "\n";
            }
        }
    }

    return 0;
}
