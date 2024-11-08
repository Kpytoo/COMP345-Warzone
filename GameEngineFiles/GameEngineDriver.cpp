#include "GameEngineDriver.h"
#include <iostream>
#include "GameEngine.h"
#include <string>

/**
 * Function to test game states by simulating user input.
 *
 * This function creates a GameEngine object, displays the current game state,
 * and manages user commands to transition between different game states.
 */
void testGameStates()
{
    // Create a new GameEngine object using dynamic memory allocation
    GameEngine *game = new GameEngine();

    // String to hold user command input
    std::string command;

    // Output a header for the test of game states to the console
    std::cout << "--- Test Game States ---\n\n";

    while (true)
    {
        // Display the current game state and available commands
        // Dereference the GameEngine pointer and use the overloaded operator<< to display the current game state and valid commands
        std::cout << *game;

        // Prompt the user to enter a command or type 'quit' to exit the game
        std::cout << "\nEnter a command (or enter 'quit' to exit): ";

        // Read the command input from the user
        std::getline(std::cin, command);

        // Check if the user wants to quit the game
        if (command == "quit")
        {
            // Inform the user that the game is exiting
            std::cout << "Exiting the game...\n\n";

            // Exit the loop
            break;
        }

        // Manage the commands which will handle the transition to the next game state
        game->manageCommand(command);

        // Print a newline for better readability
        std::cout << "\n\n";
    }

    // Free memory allocated for the GameEngine object
    delete game;
}

void testStartupPhase()
{
    // Initialize objects for testing
    GameEngine gameEngine;
    Map gameMap;
    Deck gameDeck;

    std::string commandFilePath = "commands.txt";
    FileCommandProcessorAdapter commandProcessor(commandFilePath);

    // Call the startupPhase method
    gameEngine.startupPhase(commandProcessor, gameMap, gameDeck);

    // Display the final game state to verify setup success
    std::cout << "Final Game State: " << gameEngine.getCurrentState() << "\n";
    std::cout << "Players and Territories:\n";
    for (Player *player : gameEngine.getPlayers())
    {
        std::cout << "Player: " << player->getPlayerName() << "\nOwned Territories:\n";
        for (Territory *territory : player->getOwnedTerritories())
        {
            std::cout << " - " << territory->name << "\n";
        }
        std::cout << "Hand:\n";
        for (Card *card : player->getPlayerHand()->handVector)
        {
            std::cout << " - Card: " << card->getCardType() << "\n";
        }
        std::cout << "\n";
    }
}

int main()
{
    // Run the startup phase test
    testStartupPhase();
    return 0;
}
