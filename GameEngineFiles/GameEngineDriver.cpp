#include "GameEngineDriver.h"
#include <iostream>
#include "GameEngine.h"
#include <string>

#include "MapFiles/Map.h"
#include "PlayerFiles/Player.h"
#include <vector>
#include <set>

/**
 * Function to test game states by simulating user input.
 *
 * This function creates a GameEngine object, displays the current game state,
 * and manages user commands to transition between different game states.
 */
void testGameStates()
{
    // Create a new GameEngine object using dynamic memory allocation
    GameEngine* game = new GameEngine();

    // String to hold user command input
    std::string command;

    // Output a header for the test of game states to the console
    std::cout << "--- Test Game States ---\n\n";

    while(true)
    {
        // Display the current game state and available commands
        // Dereference the GameEngine pointer and use the overloaded operator<< to display the current game state and valid commands
        std::cout << *game;

        // Prompt the user to enter a command or type 'quit' to exit the game
        std::cout << "\nEnter a command (or enter 'quit' to exit): ";

        // Read the command input from the user
        std::getline(std::cin, command);

        // Check if the user wants to quit the game
        if(command == "quit")
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

void testMainGameLoop()
{
    GameEngine* gameEngine = new GameEngine();
    Map* gameMap = new Map();

    gameEngine->getCurrentMap() = gameMap;

    // Create territories manually
    Territory* t1 = new Territory();
    t1->name = "Territory1";
    Territory* t2 = new Territory();
    t2->name = "Territory2";
    Territory* t3 = new Territory();
    t3->name = "Territory3";
    Territory* t4 = new Territory();
    t4->name = "Territory4";
    Territory* t5 = new Territory();
    t5->name = "Territory5";
    Territory* t6 = new Territory();
    t6->name = "Territory6";
    Territory* t7 = new Territory();
    t7->name = "Territory7";

    // Manually set adjacent territories
    t1->adjacentTerritories.insert({"Territory2", t2});
    t1->adjacentTerritories.insert({"Territory7", t7});
    t2->adjacentTerritories.insert({"Territory1", t1});
    t2->adjacentTerritories.insert({"Territory3", t3});
    t3->adjacentTerritories.insert({"Territory2", t2});
    t3->adjacentTerritories.insert({"Territory4", t4});
    t4->adjacentTerritories.insert({"Territory3", t3});
    t4->adjacentTerritories.insert({"Territory5", t5});
    t5->adjacentTerritories.insert({"Territory4", t4});
    t5->adjacentTerritories.insert({"Territory6", t6});
    t6->adjacentTerritories.insert({"Territory5", t5});
    t7->adjacentTerritories.insert({"Territory1", t1});

    // Create continents and assign territories to continents
    Continent* continent1 = new Continent();
    continent1->bonusPoints = 5; // Example bonus of 5 points
    continent1->childTerritories.insert({"Territory1", t1});
    continent1->childTerritories.insert({"Territory2", t2});
    continent1->childTerritories.insert({"Territory3", t3});

    // Add the continent to the game map
    gameMap.continents.insert({"Continent1", continent1});

    // Create players
    Player* player1 = new Player("Player1");
    Player* player2 = new Player("Player2");

    player1->getOwnedTerritories().insert(t1);
    player1->getOwnedTerritories().insert(t2);
    player1->getOwnedTerritories().insert(t3);
    player1->getOwnedTerritories().insert(t6);

    player2->getOwnedTerritories().insert(t4);
    player2->getOwnedTerritories().insert(t5);
    player2->getOwnedTerritories().insert(t7);

    gameEngine->getPlayersList().push_back(player1);
    gameEngine->getPlayersList().push_back(player2);

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
    delete t1;
    delete t2;
    delete t3;
    delete t4;
    delete t5;
    delete t6;
    delete t7;
    delete continent1;
    delete gameMap;
    delete gameEngine;
}

int main()
{
    testMainGameLoop();
    return 0;
}