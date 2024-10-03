#include <iostream>
#include "GameEngine.h"
#include <string>

using namespace std;

void testGameStates()
{
    GameEngine* game = new GameEngine();
    string command;

    cout << "--- Test Game States ---" << endl << endl;

    while(true)
    {
        // Display the current game state and available commands
        // Access the GameEngine object by dereferencing the pointer
        cout << *game;

        cout << "\nEnter a command (or enter 'quit' to exit): ";
        // Read the command input from the user
        getline(cin, command);

        if(command == "quit")
        {
            cout << "Exiting the game..." << endl;
            break;
        }

        // Manage the commands which will handle the transition to the next game state
        game->manageCommand(command);

        cout << endl << endl;
    }

    // Free memory allocated
    delete game;
}

// Main function to run the test game states
int main()
{
    // Call test function
    testGameStates();

    return 0;
}