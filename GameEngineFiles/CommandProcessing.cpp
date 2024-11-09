#include "CommandProcessing.h"
#include <algorithm>

/**
 * Once a command gets executed, we can save its effect by using
 * saveEffect() and entering a string that reflects its effect.
 *
 * @param effect The effect of the command as a string.
 */
void Command::saveEffect(std::string effect)
{
    this->effect = effect;
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
bool CommandProcessor::validate(std::string command, GameState currentGameState)
{
    // check if the Command Collection is empty
    if (this->commandCollection.empty())
    {
        std::cout << "Can't find " << command << " inside an empty Command Collection" << std::endl;
        return false;
    }

    if (!isCommandValidForGameState(command, currentGameState)) {
        std::cout << "Command: " << command << " invalid for current game state" << std::endl;
        return false;
    }

    if (command.substr(0, 7) == "loadmap") ///> loadmap command
    {

        if (command.size() > 7) ///> loadmap xxxxx...
        {
            std::string fileName = command.substr(8);
            for (std::list<Command *>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++) ///> iterate through the command collection
            {
                if ((*(*i)).description == fileName) ///> if the command description fits the file name
                {
                    std::cout << command << " was found in the Command Collection " << std::endl;
                    return true;
                }
            }
            std::cout << command << " was not found in the Command Collection" << std::endl;
            return false;
        }
        else
        {
            std::cout << "No file name was given" << std::endl;
            return false;
        }
    }
    else if (command == "validatemap") ///> validatemap command
    {
        for (std::list<Command *>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++) ///> iterate through the command collection
        {
            if ((*(*i)).description == command) ///> if the command description fits the command
            {
                std::cout << command << " was found in the Command Collection " << std::endl;
                return true;
            }
        }
        std::cout << command << " was not found in the Command Collection" << std::endl;
        return false;
    }
    else if (command.substr(0, 9) == "addplayer") ///> addplayer command
    {
        if (command.size() > 9)
        {
            std::string playerName = command.substr(10);
            for (std::list<Command *>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++) ///> iterate through the command collection
            {
                if ((*(*i)).description == playerName) ///> if the command description fits the player name
                {
                    std::cout << command << " was found in the Command Collection " << std::endl;
                    return true;
                }
            }
            std::cout << command << " was not found in the Command Collection" << std::endl;
            return false;
        }
        else
        {
            std::cout << "No player name was given" << std::endl;
            return false;
        }
    }
    else if (command == "gamestart") ///> gamestart command
    {
        for (std::list<Command *>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++) ///> iterate through the command collection
        {
            if ((*(*i)).description == command) ///> if the command description fits the command
            {
                std::cout << command << " was found in the Command Collection " << std::endl;
                return true;
            }
        }
        std::cout << command << " was not found in the Command Collection" << std::endl;
        return false;
    }
    else if (command == "replay") ///> replay command
    {
        for (std::list<Command *>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++) ///> iterate through the command collection
        {
            if ((*(*i)).description == command) ///> if the command description fits the command
            {
                std::cout << command << " was found in the Command Collection " << std::endl;
                return true;
            }
        }
        std::cout << command << " was not found in the Command Collection" << std::endl;
        return false;
    }
    else if (command == "quit") ///> quit command
    {
        for (std::list<Command *>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++) ///> iterate through the command collection
        {
            if ((*(*i)).description == command) ///> if the command description fits the command
            {
                std::cout << command << " was found in the Command Collection " << std::endl;
                return true;
            }
        }
        std::cout << command << " was not found in the Command Collection" << std::endl;
        return false;
    }
    else ///> Invalid command has been entered
    {
        std::cout << "Can't validate an invalid command!\n"
                  << std::endl;
        return false;
    }
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
    for (std::list<Command *>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++)
    {
        delete *i; ///> free the memory held by each command pointer
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
    Command *command = new Command;

    if (stringCommand.substr(0, 7) == "loadmap") ///> loadmap command
    {

        if (stringCommand.size() > 7)
        {
            if (stringCommand.at(7) != ' ') ///> checks so there is a space between "loadmap" and the file name
            {
                std::cout << "Please leave one space between \"loadmap\" and the file name." << std::endl;
            }
            else
            {
                std::string fileName = stringCommand.substr(8);
                command->nextState = GameState::Map_Loaded; ///> state transitions to maploaded
                command->description = fileName;            ///> File name is added to the description of the command
                command->effect = "To load a map";          ///> update the effect
                this->commandCollection.push_back(command); ///> add the command to the collection
                std::cout << "*** loadmap command added to command collection (file name: " << fileName << ") ***" << std::endl;
                return *command;
            }
        }
        else
        {
            std::cout << "Please enter a file name after using the \"loadmap\" command." << std::endl; ///> if no file name was provided
            return *command;
        }
    }
    else if (stringCommand == "validatemap") ///> validatemap command
    {
        command->nextState = GameState::Map_Validated; ///> state transitions to mapvalidated
        command->description = "validatemap";          ///> description holds "validatemap"
        command->effect = "To validate a map";         ///> update the effect
        this->commandCollection.push_back(command);    ///> add the command to the collection
        std::cout << "*** validatemap command added to command collection ***" << std::endl;
        return *command;
    }
    else if (stringCommand.substr(0, 9) == "addplayer") ///> addplayer command
    {
        if (stringCommand.size() > 9) ///> addplayer xxxxx....
        {
            if (stringCommand.at(9) != ' ') ///> checks so there is a space between "addplayer" and the player name
            {
                std::cout << "Please leave one space between \"addplayer\" and the player name." << std::endl;
            }
            else
            {
                std::string playerName = stringCommand.substr(10);
                command->nextState = GameState::Players_Added; ///> state transitions to maploaded
                command->description = playerName;             ///> Player name is added to the description of the command
                command->effect = "To add a player";           ///> update the effect
                this->commandCollection.push_back(command);    ///> add the command to the collection
                std::cout << "*** playerName command added to command collection (player name: " << playerName << ") ***" << std::endl;
                return *command;
            }
        }
        else
        {
            std::cout << "Please enter a player name after using the \"addplayer\" command." << std::endl; ///> if no name was provided
            return *command;
        }
    }
    else if (stringCommand == "gamestart") ///> gamestart command
    {
        command->nextState = GameState::Assign_Reinforcement; ///> state transitions to assignreinforcement
        command->description = "gamestart";                   ///> description holds "gamestart"
        command->effect = "To start a game";                  ///> update the effect
        this->commandCollection.push_back(command);           ///> add the command to the collection
        std::cout << "*** gamestart command added to command collection ***" << std::endl;
        return *command;
    }
    else if (stringCommand == "replay") ///> replay command
    {
        command->nextState = GameState::Start;      ///> state transitions to start
        command->description = "replay";            ///> description holds "replay"
        command->effect = "To replay a game";       ///> update the effect
        this->commandCollection.push_back(command); ///> add the command to the collection
        std::cout << "*** replay command added to command collection ***" << std::endl;
        return *command;
    }
    else if (stringCommand == "quit") ///> quit command
    {
        command->nextState = GameState::End;        ///> state transitions to exit program
        command->description = "quit";              ///> description holds "quit"
        command->effect = "To quit a game";         ///> update the effect
        this->commandCollection.push_back(command); ///> add the command to the collection
        std::cout << "*** quit command added to command collection ***" << std::endl;
        return *command;
    }
    else ///> Invalid command has been entered
    {
        std::cout << "Invalid command has been entered.\n"
                  << std::endl;
        return *command;
    }
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
}

/**
 * readLineFromFile() method that reads a line from a
 * file and returns the command.
 *
 * Notice: Given the restrictions in Assignment 2, part 1,
 * especially the graph "Sum-up of the proposed design for Part 1",
 * it is impossible to read more than one command from a file since
 * the saveCommand() method has to be inside the getCommand().
 */
std::string FileLineReader::readLineFromFile()
{
    std::fstream file;
    std::string command;

    file.open(this->fileName, std::ios::in); ///> read mode

    if (file.is_open()) ///> if the file can be opened
    {
        if (getline(file, command)) ///> get the line
        {
            file.close();
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
    }
    file.close();
    return "Nothing"; ///> if the file couldn't be opened
}