#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include "GameEngine.h"

class FileLineReader;

/**
 * The CommandProcessor class is used to get
 * commands from the console as a string using its 
 * readCommand() method which store the command internally in
 * a collection of Command objects using the saveCommand() method.
 * It also provides a public getCommand() method to other objects
 * such as the GameEngine or the Player.
 */
class CommandProcessor
{
    public:
        
        /**
        * This function calls readCommand() and use its
        * output as an argument for saveCommand().
        */
        Command& getCommand();

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
        bool validate(std::string command, GameEngine& currentGame);

        /**
        * Default CommandProcessor constructor.
        */
        CommandProcessor();

        /**
         * Copy CommandProcessor constructor.
         * 
         * @param cmdprc A command processor instance that is copied from.
         */
        CommandProcessor(CommandProcessor& cmdprc);

        /**
         * CommandProcessor destructor that deletes
         * every command pointer in its command collection list.
         */
        virtual ~CommandProcessor();

        /**
         * Overloaded assignment operator for the Command Processor class.
         */
        void operator=(CommandProcessor& cmdprc);

        /**
        * Overloaded Stream insertion operator which outputs the Command Collection
        * of the CommandProcessor object.
        * 
        * @param COUT The output stream object.
        * @param CMDPRC The command processor instance being outputted.
        */
        friend std::ostream& operator<<(std::ostream& COUT, CommandProcessor& CMDPRC);

    private:

        /**
         * A list that contains a collection of Command pointers.
         */
        std::list<Command*> commandCollection;

        /**
        * This function prompts the user to enter a command 
        * from the console and stores it in a string. 
        * This string is then returned to the caller. 
        * Note: This function is used in accordance with the saveCommand() function.
        */
        virtual std::string readCommand();

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
        Command& saveCommand(const std::string stringCommand);
};

/**
 * The FileCommandProcessorAdapter class inherits from and provides 
 * the same functionality as the CommandProcessor class, 
 * except that it reads the commands sequentially from
 * a previously saved text file. This class is known as 
 * the "Adapter" class, implementing the Adapter design patter.
 */
class FileCommandProcessorAdapter : public CommandProcessor
{
    private:

        /**
         * Adaptee instance of FileLineReader.
         */
        FileLineReader* fileReader;

    public:
    
        FileCommandProcessorAdapter(std::string fileName);
        ~FileCommandProcessorAdapter();
        std::string readCommand();

};

/**
 * The FileLineReader class is the
 * "Adaptee" class that provides features to 
 * be able to read from a previously saved
 * text file. It is used by the FileCommandProcessorAdapter class.
 */
class FileLineReader
{
    private:
        std::string fileName;
    public:
        FileLineReader(std::string fileName);
        std::string readLineFromFile();
};

#endif