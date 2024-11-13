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
#include "LogFiles/LoggingObserver.h"
#include "CommandProcessing.h"

/**
 * GameEngine class responsible for managing the game states and commands.
 */
class GameEngine : public Subject, public ILoggable
{
private:
    // Pointer to the current game state
    GameState *currentGameState;

    // The map used for the game
    Map* currentMap;

    // Add a players vector to GameEngine
    std::vector<Player *> players;

    // The deck used for the game
    Deck* mainDeck;

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
    void manageCommand(Command& command);

    /**
     * Function to get the current game state as a string.
     *
     * @return The current game state as a string.
     */
    std::string getCurrentState() const;

    void setCurrentState(GameState newState);

    GameState getCurrentGameState() const;

    // Returns a const reference to the vector of players in the game
    std::vector<Player *> &getPlayers();
    // Sets the list of players in the game
    void setPlayers(const std::vector<Player *> &newPlayers);

    /**
     * Function to display the valid commands for the current state.
     */
    void displayCommands() const;

    // can delete these methods if not needed, created for GameEngineDriver
    void setCurrentMap(Map* map);
    void setGameDeck(Deck* deck);

    /**
     * Main game loop that runs the core gameplay sequence.
     */
    void mainGameLoop();

    /**
     * Executes the Reinforcement Phase for a given player.
     *
     * @param player The player who is undergoing the reinforcement phase.
     */
    void reinforcementPhase(Player* player);

    /**
     * Executes the Issuing Orders Phase for a given player.
     *
     * @param player The player who is issuing orders.
     */
    void issueOrdersPhase(Player* player);

    /**
     * Executes all orders in each player's orders list during the Orders Execution Phase of the game.
     */
    void executeOrdersPhase();

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

    std::string stringToLog() const override;

    /**
     * Helper function to convert a string to lowercase.
     *
     * @param str The input string to convert to lower case.
     * @return The lowercase version of the input string.
     */
    static std::string toLowerCase(const std::string &str);
};

#endif