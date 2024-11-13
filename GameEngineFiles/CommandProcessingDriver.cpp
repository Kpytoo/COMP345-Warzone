#include "CommandProcessingDriver.h"
#include "CommandProcessing.h"
#include "GameEngine.h"
#include <cstring>

/**
 * (1) commands can be read from the console using the CommandProcessor class
 * (2) commands can be read from a saved text file using the FileCommandProcessorAdapter
 * (3) commands that are invalid in the current game state
 *     are rejected, and valid commands result in the correct effect and state change.
 */
void testCommandProcessor(int argc, char *argv[])
{
    GameEngine *engine = new GameEngine();
    CommandProcessor *comProc;

    if (argc == 2)
    {
        if (strcmp(argv[1], "-console") == 0) ///> if the command line is "-console"
        {
            comProc = new CommandProcessor(); ///> create a new command processor instance on the heap
            // Get 4 commands
            comProc->getCommand();
            comProc->getCommand();
            comProc->getCommand();
            comProc->getCommand();
        }
        else
        {
            std::cout << "Please specify \"-console\" or \"-file <filename>\"" << std::endl;
            // Free the memory held by comProc
            delete comProc;

            // Free the memory held by engine
            delete engine;
            return;
        }
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], "-file") == 0) ///> if the command line is "-file <filename>"
        {

            std::string fileName = argv[2];
            comProc = new FileCommandProcessorAdapter(fileName); ///> create a new file command processor adapter on the heap
            std::cout << "Reading commands from " << fileName << " file\n"
                      << std::endl;

            Command* cmd;
            do {
                cmd = &comProc->getCommand();
            } while(cmd->name != "Nothing");
        }
        else
        {
            std::cout << "Please specify \"-console\" or \"-file <filename>\"" << std::endl;
            // Free the memory held by comProc
            delete comProc;

            // Free the memory held by engine
            delete engine;
            return;
        }
    }
    else // if arg count is not 0, continue to rest of function and just show some examples
    {
        std::cout << "Please specify \"-console\" or \"-file <filename>\"" << std::endl;
        // Free the memory held by comProc
        delete comProc;

        // Free the memory held by engine
        delete engine;
        return;
    }

    // Display the commands saved in the collection
    std::cout << *comProc << std::endl;

    // Validate all possible commands

    std::cout << std::endl;

    for (Command *cmd : comProc->commandCollection) {
        std::cout << "<<< validating \"" << cmd->name << "\" command >>>" << std::endl;
        if (comProc->validate(*cmd, engine->getCurrentGameState())) ///> if loadmap is valid
        {
            engine->manageCommand(*cmd);
        }
        std::cout << std::endl;
    }

    std::cout << "\nCommands Processed: \n" << *comProc;

    // Free the memory held by comProc
    delete comProc;

    // Free the memory held by engine
    delete engine;
}