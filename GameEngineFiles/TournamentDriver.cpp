#include "TournamentDriver.h"
#include "GameEngine.h"
#include "CommandProcessing.h"
#include <iostream>
#include <vector>
#include <string>

/**
 * Processes and handles the tournament command passed through the command line.
 * 
 * This function validates the command-line arguments, ensuring that the correct command format is passed. 
 * It then parses the arguments and uses the CommandProcessor to process the tournament command, which 
 * initializes and starts the tournament using the GameEngine.
 * 
 * @param argc The number of arguments passed to the program via the command line.
 * @param argv An array of C-style strings representing the command-line arguments.
 */
void testTournament(int argc, char *argv[])
{
    // Check if the correct command format is passed by ensuring there are enough arguments
    if (argc < 2)
    {
        // If not, print an error message showing the expected command format
        std::cerr << "Usage: tournament -M <maps> -P <strategies> -G <numGames> -D <maxTurns>\n";
        return;
    }

    // Ensure the first argument is "tournament" to match the command signature
    if (std::string(argv[1]) != "tournament")
    {
        // If the first argument is not "tournament", print an error message
        std::cerr << "Invalid command. The first argument must be 'tournament'.\n";
        return;
    }

    // Convert the arguments into a vector for easier processing
    std::vector<std::string> args(argv, argv + argc);

    // Create an instance of the GameEngine to manage the tournament's logic
    GameEngine gameEngine;

    // Process the tournament command using the CommandProcessor
    // This function will parse the arguments and set up the tournament
    if (CommandProcessor::processTournamentCommand(args, gameEngine))
    {
        // If the tournament was successfully started, output a success message
        std::cout << "Tournament successfully started!\n";
    }
    else
    {
        // If the tournament setup failed, output an error message
        std::cerr << "Failed to start the tournament.\n";
    }
}
