
#include "CommandProcessing.h"

/**
 * (1) commands can be read from the console using the CommandProcessor class 
 * (2) commands can be read from a saved text file using the FileCommandProcessorAdapter
 * (3) commands that are invalid in the current game state
 *     are rejected, and valid commands result in the correct effect and state change.
 */
void testCommandProcessor()
{
    ///> (1) commands can be read from the console using the CommandProcessor class <///
    GameEngine* engine = new GameEngine();
    CommandProcessor* comProc = new CommandProcessor();
}

int main()
{
    // testCommandProcessor();
    GameEngine engine;
    CommandProcessor comProc;
    comProc.getCommand();
    comProc.getCommand();
    comProc.getCommand();
    // comProc.getCommand();
    // comProc.getCommand();
    // comProc.getCommand();
    std::cout << comProc << std::endl;
    CommandProcessor comProc2(comProc);
    comProc2.getCommand();
    std::cout << comProc2 << std::endl;
    // engine.manageCommand("loadmap");
    // engine.manageCommand("validatemap");
    // engine.manageCommand("addplayer");
    // engine.manageCommand("assigncountries");
    // engine.manageCommand("issueorder");
    // engine.manageCommand("endissueorders");
    // engine.manageCommand("win");
    // comProc.validate("loadmap l",engine);
    // std::cout << comProc << std::endl;


    return 0;
}