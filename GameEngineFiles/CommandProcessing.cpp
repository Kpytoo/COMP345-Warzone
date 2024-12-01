#include "CommandProcessing.h"
#include "GameEngine.h"
#include <algorithm>
#include <cstring>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

/**
 * Once a command gets executed, we can save its effect by using
 * saveEffect() and entering a string that reflects its effect.
 *
 * @param effect The effect of the command as a string.
 */
void Command::saveEffect(std::string effect)
{
    this->effect = effect;
    notify(this);
}

Command::Command(const Command& copy) : name(copy.name), arg(copy.arg), nextState(copy.nextState), description(copy.description) {}

Command::Command(std::string name, GameState nextState, const std::string &description) : name(name), nextState(nextState),
                                                                        description(description) {}

Command::Command() : name("Nothing"), description("Nothing") {
}

std::string Command::ToString() {
    return name + " " + arg;
}

std::string Command::stringToLog() const {
    std::stringstream SS;
    SS << "Command's effect: " << effect;
    return SS.str();
}

std::ostream &operator<<(std::ostream &os, const Command &command) {
    os << " name: "<< command.name << " arg: " << command.arg;
    return os;
}


// <<<< CommandProcessor Class Definitions >>>>

/**
 * This function calls readCommand() and use its
 * output as an argument for saveCommand().
 */
Command &CommandProcessor::getCommand()
{
    std::string stringCommand;
    stringCommand = readCommand();     ///> returns a command as a string
    return saveCommand(stringCommand); ///> saves the command in the command collection, if the command is valid
}

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
bool CommandProcessor::validate(Command& command, GameState currentGameState)
{
    if (!isCommandValidForGameState(command.name, currentGameState)) {
        std::cout << "Command: " << command.name << " invalid for current game state" << std::endl;
        return false;
    }

    return true;
}

/**
 * Check if the command input is valid for the current game state.
 *
 * This function verifies whether a given command is valid
 * based on the current state of the game.
 *
 * @param command The command string to validate.
 * @return True if the command is valid for the current game state; otherwise, false.
 */
bool CommandProcessor::isCommandValidForGameState(const std::string &command, GameState currentGameState)
{
    // Get the valid commands for the current game state
    auto validC = MapValidCommands.find(currentGameState);

    // Check if the current game state has any valid commands
    if (validC != MapValidCommands.end())
    {
        // Retrieve the list of valid commands for the current game state
        const std::vector<std::string> &validCmd = validC->second;

        // Check if the given command exists in the list of valid commands
        return std::find(validCmd.begin(), validCmd.end(), command) != validCmd.end();
    }

    // The command wasn't found in the list of valid commands for the current game state
    return false;
}

/**
 * Default CommandProcessor constructor.
 */
CommandProcessor::CommandProcessor()
{
    ///> does nothing
}

/**
 * Copy CommandProcessor constructor.
 *
 * @param cmdprc A command processor instance that is copied from.
 */
CommandProcessor::CommandProcessor(CommandProcessor &cmdprc)
{
    for (std::list<Command *>::iterator i = cmdprc.commandCollection.begin(); i != cmdprc.commandCollection.end(); i++)
    {
        Command *tempCommand = new Command();           ///> create a new command on the heap
        tempCommand->description = (*(*i)).description; ///> assign the description
        tempCommand->effect = (*(*i)).effect;           ///> assign the effect
        tempCommand->nextState = (*(*i)).nextState;     ///> assign the next state
        this->commandCollection.push_back(tempCommand); ///> add the command pointer to the command collection
    }
}

/**
 * CommandProcessor destructor that deletes
 * every command pointer in its command collection list.
 */
CommandProcessor::~CommandProcessor()
{
    for (Command *cmd : commandCollection)
    {
        delete cmd; ///> free the memory held by each command pointer
    }
}

/**
 * Overloaded assignment operator for the Command Processor class.
 */
void CommandProcessor::operator=(CommandProcessor &cmdprc)
{
    for (std::list<Command *>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++)
    {
        delete *i; ///> free the memory held by each command pointer
    }
    for (std::list<Command *>::iterator i = cmdprc.commandCollection.begin(); i != cmdprc.commandCollection.end(); i++)
    {
        Command *tempCommand = new Command();           ///> create a new command on the heap
        tempCommand->description = (*(*i)).description; ///> assign the new description
        tempCommand->effect = (*(*i)).effect;           ///> assign the new effect
        tempCommand->nextState = (*(*i)).nextState;     ///> assign the new next state
        this->commandCollection.push_back(tempCommand); ///> add the command pointer to the command collection
    }
}

/**
 * Overloaded Stream insertion operator which outputs the Command Collection
 * of the CommandProcessor object.
 *
 * @param COUT The output stream object.
 * @param CMDPRC The command processor instance being outputted.
 */
std::ostream &operator<<(std::ostream &COUT, CommandProcessor &CMDPRC)
{
    if (CMDPRC.commandCollection.empty()) ///> if the command collection is empty
    {
        COUT << "Command collection is empty.";
    }
    else ///> if the command collection has at least one command
    {
        COUT << "<< Command Collection >>" << std::endl;
        for (std::list<Command *>::iterator i = CMDPRC.commandCollection.begin(); i != CMDPRC.commandCollection.end(); i++) ///> iterate through each command
        {
            COUT << (*(*i)).description << " - Effect: " << (*(*i)).effect << std::endl; ///> prints out the effect of each command
        }
    }
    return COUT;
}

/**
 * This function prompts the user to enter a command
 * from the console and stores it in a string.
 * This string is then returned to the caller.
 * Note: This function is used in accordance with the saveCommand() function.
 */
std::string CommandProcessor::readCommand()
{
    std::string stringCommand;

    std::cout << "List of commands: {loadmap <mapfile>, validatemap, addplayer <playername>, " << "gamestart, replay, quit}" << std::endl;
    std::cout << "Enter a command: ";

    std::getline(std::cin, stringCommand); ///> get the input line from the console
    std::cout << std::endl;

    return stringCommand; ///> return the input line
}

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
Command &CommandProcessor::saveCommand(const std::string stringCommand)
{
    std::istringstream iss(stringCommand);
    std::string token;
    std::getline(iss, token, ' '); // Split command into name and arg by white space
    std::string cmd = token;

    std::string arg = "";
    if (std::getline(iss, token)) {
        arg = token;
    }

    if ((cmd == "addplayer" || cmd == "loadmap") && arg.length() == 0) {
        std::cout<<"No argument was provided for the " << cmd << " command." << std::endl;
        Command* c = new Command();
        return *c;
    }

    if (MapCommand.find(cmd) != MapCommand.end()) {
        Command* command = new Command(MapCommand.at(cmd));
        command->arg = arg;
        commandCollection.push_back(command); ///> add the command to the collection
        std::cout << "*** " << cmd << " command added to command collection (argument: " << arg << ") ***" << std::endl;
        notify(this);
        return *command;
    } else {
        Command* c = new Command();
        return *c;
    }
}

std::string CommandProcessor::stringToLog() const {
    std::stringstream SS;
    SS << "Command: " << *commandCollection.back();
    return SS.str();
}

/**
 * Processes the tournament command and parses the arguments for tournament configuration.
 * 
 * This function extracts relevant tournament parameters (maps, strategies, number of games, and maximum turns)
 * from the command-line arguments and validates them. If the parameters are valid, it calls the GameEngine to start the tournament with the parsed settings.
 * 
 * @param args A vector of strings representing the command-line arguments passed to the program.
 * @param gameEngine A reference to the GameEngine instance that will manage and start the tournament.
 * 
 * @return Returns true if the tournament parameters are valid and the tournament was successfully started, otherwise false.
 */
bool CommandProcessor::processTournamentCommand(const std::vector<std::string>& args, GameEngine &gameEngine)
{
    // Vectors to hold the parsed maps
    std::vector<std::string> maps;
    // Vectors to hold the parsed strategies
    std::vector<std::string> strategies;
    // Number of games
    int numGames = 0;
    // Max turns for each game
    int maxTurns = 0;

    // Loop through the arguments to extract values for maps, strategies, numGames, and maxTurns
    for (size_t i = 1; i < args.size(); ++i)
    {
        // If we encounter the argument -M, it indicates maps are being specified
        if (args[i] == "-M")
        {
            // Read the maps from the next argument and split them by commas
            std::stringstream ss(args[++i]);
            std::string map;
            // Split the maps by commas and add them to the 'maps' vector
            while (std::getline(ss, map, ','))
            {
                maps.push_back(map);
            }
        }
        // If we encounter the argument -P, it indicates strategies are being specified
        else if (args[i] == "-P")
        {
            // Read the strategies from the next argument and split them by commas
            std::stringstream ss(args[++i]);
            std::string strategy;
            // Split the strategies by commas and add them to the 'strategies' vector
            while (std::getline(ss, strategy, ','))
            {
                strategies.push_back(strategy);
            }
        }
        // If we encounter the argument -G, it indicates the number of games is being specified
        else if (args[i] == "-G")
        {
            // Convert the number of games from string to integer
            numGames = std::stoi(args[++i]);
        }
        // If we encounter the argument -D, it indicates the maximum number of turns per game is being specified
        else if (args[i] == "-D")
        {
            // Convert the max turns from string to integer
            maxTurns = std::stoi(args[++i]);
        }
    }

    // Validate the parsed parameters to make sure they fall within acceptable ranges
    if (maps.size() < 1 || maps.size() > 5 || strategies.size() < 2 || strategies.size() > 4 || numGames < 1 || numGames > 5 || maxTurns < 10 || maxTurns > 50) 
    {
        // If the validation fails, print an error message and return false
        std::cout << "Invalid tournament parameters!\n";
        return false;
    }

    // Call the GameEngine to start the tournament with the parsed and validated parameters
    gameEngine.startTournament(maps, strategies, numGames, maxTurns);

    // Return true indicating the tournament was successfully started and the results are written in the log file
    return true;
}

// <<<< FileCommandProcessorAdapter Class Definitions >>>>

/**
 * Parametrized FileCommandProcessorAdapter constructor
 * that in a file name string and assigns it to
 * an instance of FileLineReader.
 *
 * @param fileName A file name string.
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(std::string fileName)
{
    this->fileReader = new FileLineReader(fileName);
}

/**
 * FileCommandProcessorAdapter destructor that
 * frees the memory of its FileLineReader instance.
 */
FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{
    delete fileReader; ///> free memory held by fileReader
}

/**
 * Overriden readCommand() method from the CommandProcessor base class
 * to be able to read a line from a file using the readLineFromFile() method
 * of the FileLineReader instance: fileReader.
 */
std::string FileCommandProcessorAdapter::readCommand()
{
    return this->fileReader->readLineFromFile(); ///> returns the command read from the file
}

// <<<< FileLineReader Class Definitions >>>>

/**
 * Parametrized FileLineReader constructor that
 * takes a string as input and assigns it to its
 * fileName variable.
 *
 * @param fileName A file name string.
 */
FileLineReader::FileLineReader(std::string fileName)
{
    this->fileName = fileName;
    file.open(fileName, std::ios::in);
}

/**
 * readLineFromFile() method that reads the next line from the opened
 * file and returns the command.
 */
std::string FileLineReader::readLineFromFile()
{
    std::string command;

    if (file.is_open()) ///> if the file is opened
    {
        if (getline(file, command)) ///> get the line
        {
            command.erase(std::remove(command.begin(), command.end(), '\n'),
                   command.end());
            return command;
        }
        else
        {
            std::cout << this->fileName << " is empty" << std::endl;
            file.close();
            return "Nothing"; ///> if the file is empty
        }
    }
    else
    {
        std::cout << this->fileName << " was not found!" << std::endl; ///> if the file wasn't found
        file.close();
    }
    return "Nothing"; ///> if the file couldn't be opened
}

FileLineReader::~FileLineReader() {
    file.close();
}
