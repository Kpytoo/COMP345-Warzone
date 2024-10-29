#include "CommandProcessing.h"


// <<<< CommandProcessor Class Definitions >>>> 

void CommandProcessor::getCommand()
{   
    std::string stringCommand;
    stringCommand = readCommand();
    saveCommand(stringCommand);
}

void CommandProcessor::validate(Command& cmd, GameEngine& currentGame)
{
    //check if command is in commandprocessor
    switch(cmd.nextState)
    {
        case GameState::Map_Loaded: ///> "loadmap" command has its nextstate as "Map_Loaded"
        {
            if(currentGame.getCurrentState() == "Start" || currentGame.getCurrentState() == "Load Map") ///> We can load a map if the game has started or if a map has already been added
            {
                std::cout << "\"loadmap\" command is in its valide state" << std::endl;
            }
            else
            {
                std::cout << "\"loadmap\" command is not in its valide state, error message has been saved" << std::endl;
                cmd.saveEffect("\"loadmap\" command is invalid");
            }
            break;
        }
        case GameState::Map_Validated: ///> "validatemap" command has its nextstate as "Map_Validated"
        {
            if(currentGame.getCurrentState() == "Load Map") ///> We can validate the map if the map has been loaded
            {
                std::cout << "\"validate\" command is in its valide state" << std::endl;
            }
            else
            {
                std::cout << "\"validate\" is not in its valide state, error message has been saved" << std::endl;
                cmd.saveEffect("\"validate\" is invalid");
            }
            break;
        }
        case GameState::Players_Added: ///> "addplayer" command has its nextstate as "Players_Added"
        {
            if(currentGame.getCurrentState() == "Validate Map" || currentGame.getCurrentState() == "Add Player") ///> We can add players if the map has been validated or one player has already been added
            {
                std::cout << "\"addplayer\" command is in its valide state" << std::endl;
            }
            else
            {
                std::cout << "\"addplayer\" is not in its valide state, error message has been saved" << std::endl;
                cmd.saveEffect("\"addplayer\" is invalid");
            }
            break;
        }
        case GameState::Assign_Reinforcement: ///> "gamestart" command has its nextstate as "Assign_Reinforcement"
        {
            if(currentGame.getCurrentState() == "Add Player") ///> We can start the game if players have been added
            {
                std::cout << "\"gamestart\" command is in its valide state" << std::endl;
            }
            else
            {
                std::cout << "\"gamestart\" is not in its valide state, error message has been saved" << std::endl;
                cmd.saveEffect("\"gamestart\" is invalid");
            }
            break;
        }
        case GameState::Start: ///> "replay" command has its nextstate as "Start"
        {
            if(currentGame.getCurrentState() == "End") ///> We can replay the game if it has ended
            {
                std::cout << "\"replay\" command is in its valide state" << std::endl;
            }
            else
            {
                std::cout << "\"replay\" is not in its valide state, error message has been saved" << std::endl;
                cmd.saveEffect("\"replay\" is invalid");
            }
            break;
        }
        case GameState::End: ///> "quit" command has its nextstate as "End"
        {
            if(currentGame.getCurrentState() == "End") ///> We can quit the game if it has ended
            {
                std::cout << "\"quit\" command is in its valide state" << std::endl;
            }
            else
            {
                std::cout << "\"quit\" is not in its valide state, error message has been saved" << std::endl;
                cmd.saveEffect("\"quit\" is invalid");
            }
            break;
        }
        default:
        {
            std::cout << "Command is not in the current Command collection" << std::endl;
            break;
        }
    }
    
}

CommandProcessor::CommandProcessor()
{
  ///> does nothing
}

CommandProcessor::CommandProcessor(const CommandProcessor& cmdprc)
{
    
}

CommandProcessor::~CommandProcessor()
{

}

void CommandProcessor::operator=(const CommandProcessor& cmdprc)
{

}

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
           COUT << (*(*i)).description << std::endl;
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
        command->description = "Validatemap";
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
        command->description = "Gamestart";
        this->commandCollection.push_back(command);
        std::cout << "*** gamestart command added to command collection ***" << std::endl;
    }
    else if(stringCommand == "replay") ///> replay command
    {
        Command* command = new Command;
        command->nextState = GameState::Start; ///> state transitions to start
        command->description = "Replay";
        this->commandCollection.push_back(command);
        std::cout << "*** replay command added to command collection ***" << std::endl;
    }
    else if (stringCommand == "quit") ///> quit command
    {
        Command* command = new Command;
        command->nextState = GameState::End; ///> state transitions to exit program
        command->description = "Quit";
        this->commandCollection.push_back(command);
        std::cout << "*** quit command added to command collection ***" << std::endl;
    }
    else ///> Invalid command has been entered
    {
        std::cout << "Invalid command has been entered." << std::endl;
    } 
}


// <<<< FileCommandProcessorAdapter Class Definitions >>>> 