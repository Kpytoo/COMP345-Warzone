
#include "CommandProcessing.h"

// <<<< CommandProcessor Class Definitions >>>> 

void CommandProcessor::getCommand()
{   
    readCommand();
    saveCommand();
}

void CommandProcessor::validate()
{

}

CommandProcessor::CommandProcessor()
{

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

std::ostream& operator<<(std::ostream& COUT, const CommandProcessor& CMDPRC)
{
    return COUT;
}

void CommandProcessor::readCommand()
{
    std::string stringCommand;
    std::cout << "List of commands: {loadmap <mapfile>, validatemap, addplayer <playername>, "<<
                                    "gamestart, replay, quit}" << std::endl;
    std::cout << "Enter a command: ";
    std::cin >> stringCommand;
    std::cout << "Command entered: " << stringCommand << std::endl;
}

void CommandProcessor::saveCommand()
{

}

void CommandProcessor::saveEffect()
{

}