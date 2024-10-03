#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>


// Game state/phase
enum class GameState
{
    Start,
    Map_Loaded,
    Map_Validated,
    Players_Added,
    Assign_Reinforcement,
    Issue_Orders,
    Execute_Orders,
    Win,
    End
};

// Command object which holds a game state with its description
struct Command
{
    GameState nextState;
    std::string description;
};

class GameEngine
{
private:
    // Pointer to the current game state
    GameState* currentGameState;
    // Pointer to the map of commands where each user input string corresponds to a game state with its description
    std::map<std::string, Command>* mapCommand;
    // Pointer to the map of valid commands which for each game state, there is a list of valid command(s)
    std::map<GameState, std::vector<std::string>>* mapValidCommands;

    // Helper function to convert all user string inputs to lowercase
    std::string toLowerCase(const std::string& str) const;

public:
    // Default Constructor
    GameEngine();
    // Deep copy constructor to create independent copies of the resources for each object
    GameEngine(const GameEngine& gameCopy);
    // Destructor
    ~GameEngine();

    // Assignment operator overload to copy values from one object to another after that have been created
    GameEngine& operator = (const GameEngine& gameCopy);
    // Stream insertion operation overload, how a custom object (Game Engine) would be printed
    friend std::ostream& operator<<(std::ostream& os, const GameEngine& gameCopy);

    // Function to check user input and trigger the appropriate state transitions
    void manageCommand(const std::string &command);
    // Function to get the current state as a string
    std::string getCurrentState() const;
    // Function to display the valid commands
    void displayCommands() const;
    // Function to check if the command is valid in the current game state
    bool isCommandValid(const std::string& command) const;
};

#endif