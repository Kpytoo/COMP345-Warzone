#include <iostream>

#include "MapFiles/MapDriver.h"
#include "CardsFiles/CardsDriver.h"
#include "PlayerFiles/PlayerDriver.h"
#include "GameEngineFiles/GameEngineDriver.h"
#include "OrdersFiles/OrdersDriver.h"
#include "LogFiles/LoggingObserverDriver.h"
#include "GameEngineFiles/CommandProcessingDriver.h"
#include "PlayerFiles/PlayerStrategiesDriver.h"
#include "GameEngineFiles/TournamentDriver.h"

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
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        std::cout << "No specific tests selected. Running no tests.";
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];

            // Assignment #1 Drivers
            if (arg == "testLoadMaps")
            {
                std::cout << "Running test: testLoadMaps...\n";
                testLoadMaps();
            }
            else if (arg == "testCards")
            {
                std::cout << "Running test: testCards...\n";
                testCards();
            }
            else if (arg == "testPlayers")
            {
                std::cout << "Running test: testPlayers...\n";
                testPlayers();
            }
            else if (arg == "testGameStates")
            {
                std::cout << "Running test: testGameStates...\n";
                testGameStates();
            }
            else if (arg == "testOrdersLists")
            {
                std::cout << "Running test: testOrdersLists...\n";
                testOrdersLists();
            }

            // Assignment #2 drivers
            else if (arg == "testCommandProcessor")
            {
                testCommandProcessor(argc - 1, argv + 1); // Provide args after first argument to driver
                break;                                    // Exit parent loop
            }
            else if (arg == "testStartupPhase")
            {
                std::cout << "Running test: testStartupPhase...\n";
                testStartupPhase();
            }
            else if (arg == "testMainGameLoop")
            {
                std::cout << "Running test: testMainGameLoop...\n";
                testMainGameLoop();
            }
            else if (arg == "testOrderExecution")
            {
                std::cout << "Running test: testOrderExecution...\n";
                testOrderExecution();
            }
            else if (arg == "testLoggingObserver")
            {
                std::cout << "Running test: testLoggingObserver...\n";
                testLoggingObserver();
            }

            // Assignment 3 drivers
            else if (arg == "testPlayerStrategies")
            {
                std::cout << "Running test: testPlayerStrategies...\n";
                testPlayerStrategies();
            }
            else if (arg == "testTournament")
            {
                std::cout << "Running test: testTournament...\n";
                testTournament(argc - 1, argv + 1); // Provide args after first argument to driver
            }
            else
            {
                std::cerr << "Unknown test: " << arg << "\n";
            }
        }
    }

    return 0;
}
