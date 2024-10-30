#include "CommandProcessing.h"


// <<<< CommandProcessor Class Definitions >>>> 

/**
 * This function calls readCommand() and use its
 * output as an argument for saveCommand().
 */
void CommandProcessor::getCommand()
{   
    std::string stringCommand;
    stringCommand = readCommand();
    saveCommand(stringCommand);
}

/**
 * This function takes in a string that stores a command and a GameEngine object
 * and firstly checks whether the given Command is available
 * inside the Command Collection list, if it is, we check whether 
 * the command is valid in the current game state, if it is not, 
 * we output and error message stating that the Command is not in
 * the Command Collection. 
 * Whenever checking for validity, if it is not
 * valid, we save an error message in the "effect" of the Command.
 * 
 * @param command String that holds a command.
 * @param currentGame GameEngine instance passed by reference.
 */
void CommandProcessor::validate(std::string command, GameEngine& currentGame)
{
    //check if the Command Collection is empty
    if(this->commandCollection.empty() == true)
    {
        std::cout << "Can't find " << command << " inside an empty Command Collection" << std::endl;
        return;
    }
    
    if(command.substr(0,7) == "loadmap") ///> loadmap command
    {

        if(command.size()>7) ///> loadmap xxxxx...
        {   
            std::string fileName = command.substr(8);
            for(std::list<Command*>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++) ///> iterate through the command collection
            {
                if( (*(*i)).description == fileName ) ///> if the command description fits the file name 
                {
                    std::cout << command << " was found in the Command Collection " << std::endl;
                    if(currentGame.getCurrentState() == "Start" || currentGame.getCurrentState() == "Load Map")///> Check game state if valid
                    {
                        std::cout << command << " is currently valid! | Gamestate: " << currentGame.getCurrentState() << std::endl;
                        return;
                    } 
                    else ///< if command is not valid in current game state
                    {
                        std::cout << command << " is not currently valide! | Gamestate: " << currentGame.getCurrentState() << std::endl;
                        (*(*i)).saveEffect("Error: loadmap is not valid in current game state: " + currentGame.getCurrentState());
                        std::cout << "Effect saved: " << (*(*i)).effect << "\n" << std::endl;
                        return;
                    }
                }
            }  
            std::cout << command << " was not found in the Command Collection" << std::endl;   
            return;    
        }
        else
        {
            std::cout << "No file name was given" << std::endl;
            return;
        }
    }
    else if (command == "validatemap") ///> validatemap command
    {
        for(std::list<Command*>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++) ///> iterate through the command collection
        {
            if( (*(*i)).description == command ) ///> if the command description fits the command
            {
                std::cout << command << " was found in the Command Collection " << std::endl;
                if(currentGame.getCurrentState() == "Load Map")///> Check game state if valid
                {
                    std::cout << command << " is currently valid! | Gamestate: " << currentGame.getCurrentState() << std::endl;
                    return;
                } 
                else ///< if command is not valid in current game state
                {
                    std::cout << command << " is not currently valide! | Gamestate: " << currentGame.getCurrentState() << std::endl;
                    (*(*i)).saveEffect("Error: validatemap is not valid in current game state: " + currentGame.getCurrentState());
                    std::cout << "Effect saved: " << (*(*i)).effect << "\n" << std::endl;
                    return;
                }
            }
        }
        std::cout << command << " was not found in the Command Collection" << std::endl;   
        return;  
    }
    else if (command.substr(0,9) == "addplayer") ///> addplayer command
    {
        if(command.size()>9)
        {
            std::string playerName = command.substr(10);
            for(std::list<Command*>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++) ///> iterate through the command collection
            {
                if( (*(*i)).description == playerName ) ///> if the command description fits the player name 
                {
                    std::cout << command << " was found in the Command Collection " << std::endl;
                    if(currentGame.getCurrentState() == "Validate Map" || currentGame.getCurrentState() == "Add Player")///> Check game state if valid
                    {
                        std::cout << command << " is currently valid! | Gamestate: " << currentGame.getCurrentState() << std::endl;
                        return;
                    } 
                    else ///< if command is not valid in current game state
                    {
                        std::cout << command << " is not currently valide! | Gamestate: " << currentGame.getCurrentState() << std::endl;
                        (*(*i)).saveEffect("Error: addplayer is not valid in current game state: " + currentGame.getCurrentState());
                        std::cout << "Effect saved: " << (*(*i)).effect << "\n" << std::endl;
                        return;
                    }
                }
            }  
            std::cout << command << " was not found in the Command Collection" << std::endl;   
            return;    
        }
        else
        {
            std::cout << "No player name was given" << std::endl;
            return;
        } 
    }
    else if (command == "gamestart") ///> gamestart command
    {
        for(std::list<Command*>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++) ///> iterate through the command collection
        {
            if( (*(*i)).description == command ) ///> if the command description fits the command
            {
                std::cout << command << " was found in the Command Collection " << std::endl;
                if(currentGame.getCurrentState() == "Add Player")///> Check game state if valid
                {
                    std::cout << command << " is currently valid! | Gamestate: " << currentGame.getCurrentState() << std::endl;
                    return;
                } 
                else ///< if command is not valid in current game state
                {
                    std::cout << command << " is not currently valide! | Gamestate: " << currentGame.getCurrentState() << std::endl;
                    (*(*i)).saveEffect("Error: gamestart is not valid in current game state: " + currentGame.getCurrentState());
                    std::cout << "Effect saved: " << (*(*i)).effect << "\n" << std::endl;
                    return;
                }
            }
        }
        std::cout << command << " was not found in the Command Collection" << std::endl;   
        return;
    }
    else if(command == "replay") ///> replay command
    {
        for(std::list<Command*>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++) ///> iterate through the command collection
        {
            if( (*(*i)).description == command ) ///> if the command description fits the command
            {
                std::cout << command << " was found in the Command Collection " << std::endl;
                if(currentGame.getCurrentState() == "Win")///> Check game state if valid
                {
                    std::cout << command << " is currently valid! | Gamestate: " << currentGame.getCurrentState() << std::endl;
                    return;
                } 
                else ///< if command is not valid in current game state
                {
                    std::cout << command << " is not currently valide! | Gamestate: " << currentGame.getCurrentState() << std::endl;
                    (*(*i)).saveEffect("Error: replay is not valid in current game state: " + currentGame.getCurrentState());
                    std::cout << "Effect saved: " << (*(*i)).effect << "\n" << std::endl;
                    return;
                }
            }
        }
        std::cout << command << " was not found in the Command Collection" << std::endl;   
        return;
    }
    else if (command == "quit") ///> quit command
    {
        for(std::list<Command*>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++) ///> iterate through the command collection
        {
            if( (*(*i)).description == command ) ///> if the command description fits the command
            {
                std::cout << command << " was found in the Command Collection " << std::endl;
                if(currentGame.getCurrentState() == "Win")///> Check game state if valid
                {
                    std::cout << command << " is currently valid! | Gamestate: " << currentGame.getCurrentState() << std::endl;
                    return;
                } 
                else ///< if command is not valid in current game state
                {
                    std::cout << command << " is not currently valide! | Gamestate: " << currentGame.getCurrentState() << std::endl;
                    (*(*i)).saveEffect("Error: quit is not valid in current game state: " + currentGame.getCurrentState());
                    std::cout << "Effect saved: " << (*(*i)).effect << "\n" << std::endl;
                    return;
                }
            }
        }
        std::cout << command << " was not found in the Command Collection" << std::endl;   
        return;
    }
    else ///> Invalid command has been entered
    {
        std::cout << "Can't validate an invalid command!\n" << std::endl;
    }     
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
CommandProcessor::CommandProcessor(CommandProcessor& cmdprc)
{
    for(std::list<Command*>::iterator i = cmdprc.commandCollection.begin(); i != cmdprc.commandCollection.end(); i++)
    {
        Command* tempCommand = new Command();
        tempCommand->description = (*(*i)).description;
        tempCommand->effect = (*(*i)).effect;
        tempCommand->nextState = (*(*i)).nextState;
        this->commandCollection.push_back(tempCommand);
    }
}

/**
 * CommandProcessor destructor that deletes
 * every command pointer in its command collection list.
 */
CommandProcessor::~CommandProcessor()
{
        for(std::list<Command*>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++)
    {
        delete *i;
    }
}

/**
 * Overloaded assignment operator for the Command Processor class.
 */
void CommandProcessor::operator=(CommandProcessor& cmdprc)
{
    for(std::list<Command*>::iterator i = this->commandCollection.begin(); i != this->commandCollection.end(); i++)
    {
        delete *i;
    }
        for(std::list<Command*>::iterator i = cmdprc.commandCollection.begin(); i != cmdprc.commandCollection.end(); i++)
    {
        Command* tempCommand = new Command();
        tempCommand->description = (*(*i)).description;
        tempCommand->effect = (*(*i)).effect;
        tempCommand->nextState = (*(*i)).nextState;
        this->commandCollection.push_back(tempCommand);
    }
}

/**
 * Overloaded Stream insertion operator which outputs the Command Collection
 * of the CommandProcessor object.
 * 
 * @param COUT The output stream object.
 * @param CMDPRC The command processor instance being outputted.
 */
std::ostream& operator<<(std::ostream& COUT, CommandProcessor& CMDPRC)
{
    if(CMDPRC.commandCollection.empty()) ///> if the command collection is empty
    {
        COUT << "Command collection is empty.";
    }
    else ///> if the command collection has at least one command
    {
        COUT << "<< Command Collection >>" << std::endl;
        for(std::list<Command*>::iterator i = CMDPRC.commandCollection.begin(); i != CMDPRC.commandCollection.end(); i++)
        {
           COUT << (*(*i)).description << " - Effect: " << (*(*i)).effect << std::endl;
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

    std::cout << "List of commands: {loadmap <mapfile>, validatemap, addplayer <playername>, "<<
                                    "gamestart, replay, quit}" << std::endl;
    std::cout << "Enter a command: ";

    std::getline(std::cin, stringCommand);
    std::cout << std::endl;

    return stringCommand;
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
void CommandProcessor::saveCommand(const std::string stringCommand)
{
    
    if(stringCommand.substr(0,7) == "loadmap") ///> loadmap command
    {
        
        if(stringCommand.size()>7)
        {
            if(stringCommand.at(7) != ' ') ///> checks so there is a space between "loadmap" and the file name
            {
                std::cout << "Please leave one space between \"loadmap\" and the file name." << std::endl;
            }
            else
            {
                std::string fileName = stringCommand.substr(8);
                Command* command = new Command;
                command->nextState = GameState::Map_Loaded; ///> state transitions to maploaded
                command->description = fileName; ///> File name is added to the description of the command
                command->effect = "To load a map";
                this->commandCollection.push_back(command);
                std::cout << "*** loadmap command added to command collection (file name: " << fileName << ") ***" << std::endl;
            }
            
        }
        else
        {
           std::cout << "Please enter a file name after using the \"loadmap\" command." << std::endl; 
        }  
    }
    else if (stringCommand == "validatemap") ///> validatemap command
    {
        Command* command = new Command;
        command->nextState = GameState::Map_Validated; ///> state transitions to mapvalidated
        command->description = "validatemap";
        command->effect = "To validate a map";
        this->commandCollection.push_back(command);
        std::cout << "*** validatemap command added to command collection ***" << std::endl;
    }
    else if (stringCommand.substr(0,9) == "addplayer") ///> addplayer command
    {
        if(stringCommand.size()>9)
        {
            if(stringCommand.at(9) != ' ') ///> checks so there is a space between "addplayer" and the player name
            {
                std::cout << "Please leave one space between \"addplayer\" and the player name." << std::endl;
            }
            else
            {
                std::string playerName = stringCommand.substr(10);
                Command* command = new Command;
                command->nextState = GameState::Players_Added; ///> state transitions to maploaded
                command->description = playerName; ///> Player name is added to the description of the command
                command->effect = "To add a player";
                this->commandCollection.push_back(command);
                std::cout << "*** playerName command added to command collection (player name: " << playerName << ") ***" << std::endl;   
            }

        }
        else
        {
           std::cout << "Please enter a player name after using the \"addplayer\" command." << std::endl; 
        }  
    }
    else if (stringCommand == "gamestart") ///> gamestart command
    {
        Command* command = new Command;
        command->nextState = GameState::Assign_Reinforcement; ///> state transitions to assignreinforcement
        command->description = "gamestart";
        command->effect = "To start a game";
        this->commandCollection.push_back(command);
        std::cout << "*** gamestart command added to command collection ***" << std::endl;
    }
    else if(stringCommand == "replay") ///> replay command
    {
        Command* command = new Command;
        command->nextState = GameState::Start; ///> state transitions to start
        command->description = "replay";
        command->effect = "To replay a game";
        this->commandCollection.push_back(command);
        std::cout << "*** replay command added to command collection ***" << std::endl;
    }
    else if (stringCommand == "quit") ///> quit command
    {
        Command* command = new Command;
        command->nextState = GameState::End; ///> state transitions to exit program
        command->description = "quit";
        command->effect = "To quit a game";
        this->commandCollection.push_back(command);
        std::cout << "*** quit command added to command collection ***" << std::endl;
    }
    else ///> Invalid command has been entered
    {
        std::cout << "Invalid command has been entered.\n" << std::endl;
    } 
}


// <<<< FileCommandProcessorAdapter Class Definitions >>>> 