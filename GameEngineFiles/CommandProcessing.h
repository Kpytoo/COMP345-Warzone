#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include <map>
#include <vector>

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

    // Name of command
    std::string name;

    // Argument of command
    std::string arg;

    // A description of what the command does
    std::string description;

    // The effect of the command
    std::string effect;

    Command();

    Command(const Command& copy);

    Command(std::string name, GameState nextState, const std::string &description);

    std::string ToString();

    /**
     * Once a command gets executed, we can save its effect by using
     * saveEffect() and entering a string that relfects its effect.
     *
     * @param effect The effect of the command as a string.
     */
    void saveEffect(std::string effect);
};

/**
 * Statically define the maps for representing the commands and the valid commands per state
 */
// Map of commands with their descriptions
const std::map<std::string, Command> MapCommand = {
        {"loadmap", {"loadmap", GameState::Map_Loaded, "Loading a Map"}},
        {"validatemap", {"validatemap", GameState::Map_Validated, "Validate the Loaded Map"}},
        {"addplayer", {"addplayer", GameState::Players_Added, "Add Players To The Game"}},
        {"assigncountries", {"assigncountries", GameState::Assign_Reinforcement, "Assign Countries To Players"}},
        {"issueorder", {"issueorder", GameState::Issue_Orders, "Issue Orders"}},
        {"endissueorders", {"endissueorders", GameState::Execute_Orders, "Execute Orders"}},
        {"execorder", {"execorder", GameState::Execute_Orders, "Execute Orders"}},
        {"endexecorders", {"endexecorders", GameState::Assign_Reinforcement, "Assign Countries To Players"}},
        {"win", {"win", GameState::Win, "You Won!"}},
        {"end", {"end", GameState::End, "Game Finished"}},
        {"play", {"play", GameState::Start, "Restart The Game"}}
};

// Valid commands for each game state
const std::map<GameState, std::vector<std::string>> MapValidCommands = {
        {GameState::Start, {"loadmap"}},
        {GameState::Map_Loaded, {"loadmap", "validatemap"}},
        {GameState::Map_Validated, {"addplayer"}},
        {GameState::Players_Added, {"addplayer", "assigncountries"}},
        {GameState::Assign_Reinforcement, {"issueorder"}},
        {GameState::Issue_Orders, {"issueorder", "endissueorders"}},
        {GameState::Execute_Orders, {"execorder", "endexecorders", "win"}},
        {GameState::Win, {"play", "end"}}
};

/**
 * The CommandProcessor class is used to get
 * commands from the console as a string using its
 * readCommand() method which store the command internally in
 * a collection of Command objects using the saveCommand() method.
 * It also provides a public getCommand() method to other objects
 * such as the GameEngine or the Player.
 */
class CommandProcessor
{
public:
    /**
     * This function calls readCommand() and use its
     * output as an argument for saveCommand().
     */
    Command &getCommand();

    /**
     * This function checks whether the given Command is available
     * inside the Command Collection list, if it is not,
     * we output and error message stating that the Command is not in
     * the Command Collection.
     * Whenever checking for validity, if it is not
     * valid, we save an error message in the "effect" of the Command.
     *
     * @param command String that holds a command.
     */
    bool validate(Command& command, GameState currentGameState);

    /**
     * Function to check if a command is valid in the current game state.
     *
     * @param command The command to check.
     * @return True if the command is valid, false otherwise.
     */
    static bool isCommandValidForGameState(const std::string& command, GameState currentGameState);

    /**
     * Default CommandProcessor constructor.
     */
    CommandProcessor();

    /**
     * Copy CommandProcessor constructor.
     *
     * @param cmdprc A command processor instance that is copied from.
     */
    CommandProcessor(CommandProcessor &cmdprc);

    /**
     * CommandProcessor destructor that deletes
     * every command pointer in its command collection list.
     */
    virtual ~CommandProcessor();

    /**
     * Overloaded assignment operator for the Command Processor class.
     */
    void operator=(CommandProcessor &cmdprc);

    /**
     * Overloaded Stream insertion operator which outputs the Command Collection
     * of the CommandProcessor object.
     *
     * @param COUT The output stream object.
     * @param CMDPRC The command processor instance being outputted.
     */
    friend std::ostream &operator<<(std::ostream &COUT, CommandProcessor &CMDPRC);

    /**
     * A vector that contains a collection of Command pointers.
     */
    std::list<Command *> commandCollection;

private:
    /**
     * This function prompts the user to enter a command
     * from the console and stores it in a string.
     * This string is then returned to the caller.
     * Note: This function is used in accordance with the saveCommand() function.
     */
    virtual std::string readCommand();

    /**
     * This function takes in a constant string, and parses it
     * to determine the specific command. If the command entered
     * is not valid, an error message is displayed to the user.
     * If it is a valid command, the command is stored into the
     * Command Collection list of the Command Processor object.
     *
     * Possible types of command: {loadmap <mapfile>, validatemap, addplayer <playername>, gamestart, replay, quit}.
     *
     * @param stringCommand Constant string that contains the specified command.
     */
    Command &saveCommand(const std::string stringCommand);
};

/**
 * The FileLineReader class is the
 * "Adaptee" class that provides features to
 * be able to read from a previously saved
 * text file. It is used by the FileCommandProcessorAdapter class.
 */
class FileLineReader
{
private:
    std::string fileName;
    std::fstream file;

public:
    FileLineReader(std::string fileName);
    std::string readLineFromFile();

    virtual ~FileLineReader();
};

/**
 * The FileCommandProcessorAdapter class inherits from and provides
 * the same functionality as the CommandProcessor class,
 * except that it reads the commands sequentially from
 * a previously saved text file. This class is known as
 * the "Adapter" class, implementing the Adapter design patter.
 */
class FileCommandProcessorAdapter : public CommandProcessor
{
private:
    /**
     * Adaptee instance of FileLineReader.
     */
    FileLineReader *fileReader;

public:
    FileCommandProcessorAdapter(std::string fileName);
    ~FileCommandProcessorAdapter();
    std::string readCommand();
};

#endif