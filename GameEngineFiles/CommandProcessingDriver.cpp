#include "CommandProcessing.h"

/**
 * (1) commands can be read from the console using the CommandProcessor class 
 * (2) commands can be read from a saved text file using the FileCommandProcessorAdapter
 * (3) commands that are invalid in the current game state
 *     are rejected, and valid commands result in the correct effect and state change.
 */
void testCommandProcessor(int argc, char* argv[])
{
    GameEngine* engine = new GameEngine();
    CommandProcessor* comProc;

    if(argc == 2)
    {
        if(strcmp(argv[1],"-console") == 0) ///> if the command line is "-console"
        {
            comProc = new CommandProcessor(); ///> create a new command processor instance on the heap
            //Get 4 commands
            comProc->getCommand();
            comProc->getCommand();
            comProc->getCommand();
            comProc->getCommand();
        }
        else
        {
            std::cout << "Please specify \"-console\" or \"-file <filename>\"" << std::endl;
            exit(1);
        }
        
    }
    else if(argc == 3)
    {
        if(strcmp(argv[1],"-file") == 0) ///> if the command line is "-file <filename>"
        {
            
            std::string fileName = argv[2];
            comProc = new FileCommandProcessorAdapter(fileName); ///> create a new file command processor adapter on the heap
            std::cout << "Reading commands from " << fileName << " file\n" << std::endl;
            comProc->getCommand(); ///> get 1 command from the file
        }
        else
        {
            std::cout << "Please specify \"-console\" or \"-file <filename>\"" << std::endl;
            exit(1);
        }
    }
    else
    {
        std::cout << "Please specify \"-console\" or \"-file <filename>\"" << std::endl;
        exit(1);
    }

    //Display the commands saved in the collection
    std::cout << *comProc << std::endl;

    //Validate all possible commands

    std::cout << std::endl;
    std::cout << "<<< validating \"loadmap\" command >>>" << std::endl;
    if(comProc->validate("loadmap Map.txt", *engine) == true) ///> if loadmap is valid
    {
        engine->manageCommand("loadmap");
    }
    std::cout << std::endl;

    std::cout << "<<< validating \"validatemap\" command >>>" << std::endl; ///> if validatemap is valid
    if(comProc->validate("validatemap", *engine) == true)
    {
        engine->manageCommand("validatemap");
    }
    std::cout << std::endl;

    std::cout << "<<< validating \"addplayer\" command >>>" << std::endl; ///> if addplayer is valid
    if(comProc->validate("addplayer Matt", *engine) == true)
    {
        engine->manageCommand("addplayer");
    }
    std::cout << std::endl;

    std::cout << "<<< validating \"gamestart\" command >>>" << std::endl; ///> if gamestart is valid
    if(comProc->validate("gamestart", *engine) == true)
    {
        engine->manageCommand("assigncountries");
    }
    std::cout << std::endl;

    std::cout << "<<< validating \"replay\" command >>>" << std::endl; ///> if replay is valid
    if(comProc->validate("replay", *engine) == true)
    {
        engine->manageCommand("loadmap");
    }
    std::cout << std::endl;

    std::cout << "<<< validating \"quit\" command >>>" << std::endl; ///> if quit is valid
    if(comProc->validate("quit", *engine) == true)
    {
        engine->manageCommand("end");
    }
    std::cout << std::endl;

    //Free the memory held by comProc
    delete comProc;

    //Free the memory held by engine
    delete engine;
}

int main(int argc, char* argv[])
{    

    testCommandProcessor(argc, argv); ///> calling the testCommandProcessor() free function
    
    return 0;
}