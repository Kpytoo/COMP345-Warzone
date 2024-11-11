#include "GameEngineDriver.h"
#include <iostream>
#include "GameEngine.h"
#include <string>

#include "MapFiles/Map.h"
#include "PlayerFiles/Player.h"
#include "CommandProcessing.h"
#include <vector>
#include <set>

// Assignment #1 drivers
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
        Command cmd = Command();
        cmd.name = command;
        game->manageCommand(cmd);

        // Print a newline for better readability
        std::cout << "\n\n";
    }

    // Free memory allocated for the GameEngine object
    delete game;
}

// Assignment #2 drivers
void testStartupPhase()
{
    // Initialize objects for testing
    GameEngine gameEngine;
    Map gameMap;
    Deck gameDeck;

    std::string commandFilePath = "../GameEngineFiles/Commands.txt";
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

void testMainGameLoop()
{
    GameEngine* gameEngine = new GameEngine();
    Map* gameMap = new Map();
    MapLoader::LoadMap("../SomeMapsFromOnline/Montreal/Grand Montreal.map", gameMap);

    gameEngine->setCurrentMap(gameMap);

    // Create players
    Player* player1 = new Player();
    Player* player2 = new Player();

    player1->setPlayerName("Player1");
    player2->setPlayerName("Player2");

    player1->getOwnedTerritories().push_back(gameMap->territories.at("Ahuntsic"));
    player1->getOwnedTerritories().push_back(gameMap->territories.at("Cote St-Luc"));
    player1->getOwnedTerritories().push_back(gameMap->territories.at("NDG CDN"));
    player1->getOwnedTerritories().push_back(gameMap->territories.at("Outremont"));

    player2->getOwnedTerritories().push_back(gameMap->territories.at("Lachine"));
    player2->getOwnedTerritories().push_back(gameMap->territories.at("Mont Royal"));
    player2->getOwnedTerritories().push_back(gameMap->territories.at("NDG CDN"));

    gameEngine->getPlayers().push_back(player1);
    gameEngine->getPlayers().push_back(player2);

    // Test toDefend() and toAttack() for Player 1
    std::cout << "\nTesting Player 1's toDefend() and toAttack() methods:\n";
    std::vector<Territory*> toDefend1 = player1->toDefend();
    std::vector<Territory*> toAttack1 = player1->toAttack();

    std::cout << "Player 1 should defend:\n";
    for (Territory* t : toDefend1)
    {
        std::cout << t->name << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Player 1 should attack:\n";
    for (Territory* t : toAttack1)
    {
        std::cout << t->name << std::endl;
    }
    std::cout << std::endl;

    // Test toDefend() and toAttack() for Player 2
    std::cout << "\nTesting Player 2's toDefend() and toAttack() methods:\n";
    std::vector<Territory*> toDefend2 = player2->toDefend();
    std::vector<Territory*> toAttack2 = player2->toAttack();

    std::cout << "Player 2 should defend:\n";
    for (Territory* t : toDefend2)
    {
        std::cout << t->name << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Player 2 should attack:\n";
    for (Territory* t : toAttack2)
    {
        std::cout << t->name << std::endl;
    }
    std::cout << std::endl;

    // Start the game loop
    std::cout << "\nGame Starting...\n\n";

    gameEngine->mainGameLoop();

    delete player1;
    delete player2;
    delete gameMap;
    delete gameEngine;
}
