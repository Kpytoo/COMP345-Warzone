#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <iostream>
#include <vector>
#include <string>
#include "GameEngine.h"

class CommandProcessor
{
    public:
        void getCommand();
        void validate();
        CommandProcessor();
        CommandProcessor(const CommandProcessor& cmdprc);
        ~CommandProcessor();
        void operator=(const CommandProcessor& cmdprc);
        friend std::ostream& operator<<(std::ostream& COUT, const CommandProcessor& CMDPRC);
    private:
        std::vector<Command*> commandCollection;
        void readCommand();
        void saveCommand();
        void saveEffect();
};

class FileCommandProcessorAdapter
{

};

#endif