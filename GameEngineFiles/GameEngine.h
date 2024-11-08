#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "PlayerFiles/Player.h"
#include "MapFiles/Map.h"
#include "CardsFiles/Cards.h"
#include "CommandProcessing.h"
/**
 * Enum representing various game states/phases.
 */
enum class GameState
{
    // Initial state of the game
    Start,

    // State when the map has been loaded
    Map_Loaded,

    // State when the map has been validated
    Map_Validated,

    // State when players have been added to the game
    Players_Added,

    // State when players are assigning reinforcements to their territories
    Assign_Reinforcement,

    // State for players to issue their orders
    Issue_Orders,

    // State for executing the issued orders
    Execute_Orders,

    // State when a player wins the game
    Win,

    // State indicating the end of the game
    End
};

/**
 * Command class that holds a game state along with its description and methods.
 */
class Command
{
public:
    // The game state triggered from the command input by the player
    GameState nextState;

    // A description of what the command does
    std::string description;

    // The effect of the command
    std::string effect;

    /**
     * Once a command gets executed, we can save its effect by using
     * saveEffect() and entering a string that relfects its effect.
     *
     * @param effect The effect of the command as a string.
     */
    void saveEffect(std::string effect);
};

/**
 * GameEngine class responsible for managing the game states and commands.
 */
class GameEngine
{
private:
    // Pointer to the current game state
    GameState *currentGameState;

    // Map of commands with their descriptions
    std::map<std::string, Command> *mapCommand;

    // Valid commands for each game state
    std::map<GameState, std::vector<std::string>> *mapValidCommands;

    // Add a players vector to GameEngine
    std::vector<Player *> players;

    /**
     * Helper function to convert a string to lowercase.
     *
     * @param str The input string to convert to lower case.
     * @return The lowercase version of the input string.
     */
    std::string toLowerCase(const std::string &str) const;

public:
    // Default Constructor
    GameEngine();

    /**
     * Deep copy constructor for creating independent copies of GameEngine objects.
     *
     * @param gameCopy The GameEngine object to copy.
     */
    GameEngine(const GameEngine &gameCopy);

    // Destructor to clean up dynamically allocated resources.
    ~GameEngine();

    /**
     * Assignment operator overload for copying values from one GameEngine object to another.
     *
     * @param gameCopy The GameEngine object to copy from.
     * @return Reference to the current object.
     */
    GameEngine &operator=(const GameEngine &gameCopy);

    /**
     * Stream insertion operator overload to define how a GameEngine object is printed.
     *
     * @param os The output stream where the game state and commands will be printed.
     * @param gameEngine The GameEngine object whose state and commands are to be printed.
     * @return The output stream after printing the game state and valid commands.
     */
    friend std::ostream &operator<<(std::ostream &os, const GameEngine &gameCopy);

    /**
     * Function to process user input and make the respective transition.
     *
     * @param command The user input command to manage.
     */
    void manageCommand(const std::string &command);

    /**
     * Function to get the current game state as a string.
     *
     * @return The current game state as a string.
     */
    std::string getCurrentState() const;

    void setCurrentState(GameState newState);

    // Returns a const reference to the vector of players in the game
    const std::vector<Player *> &getPlayers() const;
    // Sets the list of players in the game
    void setPlayers(const std::vector<Player *> &newPlayers);

    /**
     * Function to display the valid commands for the current state.
     */
    void displayCommands() const;

    /**
     * Function to check if a command is valid in the current game state.
     *
     * @param command The command to check.
     * @return True if the command is valid, false otherwise.
     */
    bool isCommandValid(const std::string &command) const;

    /**
     * Initializes the game setup phase, including map loading, validation, player addition,
     * territory distribution, and initial resource allocation. This function guides the game
     * through its setup states and transitions to the play phase upon successful setup.
     *
     * @param commandProcessor Reference to the CommandProcessor instance for handling commands.
     * @param gameMap Reference to the Map object used in the game.
     * @param gameDeck Reference to the Deck object used for drawing initial cards for players.
     */
    void startupPhase(CommandProcessor &commandProcessor, Map &gameMap, Deck &gameDeck);
};

#endif