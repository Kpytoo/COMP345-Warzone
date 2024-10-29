#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <iostream>
#include <list>
#include <string>
#include "GameEngine.h"

class CommandProcessor
{
    public:
        
        void getCommand();
        void validate(Command& cmd, GameEngine& currentGame);
        CommandProcessor();
        CommandProcessor(const CommandProcessor& cmdprc);
        ~CommandProcessor();
        void operator=(const CommandProcessor& cmdprc);
        friend std::ostream& operator<<(std::ostream& COUT, CommandProcessor& CMDPRC);
    private:
        std::list<Command*> commandCollection;
        std::string readCommand();
        void saveCommand(const std::string stringCommand);
};

class FileCommandProcessorAdapter
{

};

#endif