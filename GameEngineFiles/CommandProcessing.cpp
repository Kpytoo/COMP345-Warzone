#include "CommandProcessing.h"
#include <algorithm>
#include <cstring>
#include <sstream>

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

Command::Command(const Command& copy) : name(copy.name), arg(copy.arg), nextState(copy.nextState), description(copy.description) {}

Command::Command(std::string name, GameState nextState, const std::string &description) : name(name), nextState(nextState),
                                                                        description(description) {}

Command::Command() : name("Nothing"), description("Nothing") {
}

std::string Command::ToString() {
    return name + " " + arg;
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

std::vector<std::string> split(const std::string &input, char delimiter) {
    std::istringstream stream(input);
    std::vector<std::string> result;
    std::string segment;

    while (std::getline(stream, segment, delimiter)) {
        result.push_back(segment);
    }

    return result;
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
    std::vector<std::string> split_command = split(stringCommand, ' ');
    std::string cmd = split_command[0];

    std::string arg = "";
    if (split_command.size() >= 2) {
        arg = split_command[1];
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
        return *command;
    } else {
        Command* c = new Command();
        return *c;
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
