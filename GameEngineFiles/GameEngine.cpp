#include "GameEngine.h"

// Default constructor
GameEngine::GameEngine()
{
    // Allocating memory for the game engine and initializing to the game state Start
    currentGameState = new GameState(GameState::Start);

    // Allocating memory for the command map
    mapCommand = new std::map<std::string, Command>();

    // Allocating memory for valid commands for each state map
    mapValidCommands = new std::map<GameState, std::vector<std::string>>();
    
    // Pairing the string command to their respective game state
    (*mapCommand)["loadmap"] = {GameState::Map_Loaded, "Loading a Map"};
    (*mapCommand)["validatemap"] = {GameState::Map_Validated, "Validate the Loaded Map"};
    (*mapCommand)["addplayer"] = {GameState::Players_Added, "Add Players To The Game"};
    (*mapCommand)["assigncountries"] = {GameState::Assign_Reinforcement, "Assign Countries To Players"};
    (*mapCommand)["issueorder"] = {GameState::Issue_Orders, "Issue Orders"};
    (*mapCommand)["endissueorders"] = {GameState::Execute_Orders, "Execute Orders"};
    (*mapCommand)["execorder"] = {GameState::Execute_Orders, "Execute Orders"};
    (*mapCommand)["endexecorders"] = {GameState::Assign_Reinforcement, "Assign Countries To Players"};
    (*mapCommand)["win"] = {GameState::Win, "You Won!"};
    (*mapCommand)["end"] = {GameState::End, "Game Finished"};
    (*mapCommand)["play"] = {GameState::Start, "Restart The Game"};

    // Define valid commands for each game state
    (*mapValidCommands)[GameState::Start] = {"loadmap"};
    (*mapValidCommands)[GameState::Map_Loaded] = {"loadmap", "validatemap"};
    (*mapValidCommands)[GameState::Map_Validated] = {"addplayer"};
    (*mapValidCommands)[GameState::Players_Added] = {"addplayer", "assigncountries"};
    (*mapValidCommands)[GameState::Assign_Reinforcement] = {"issueorder"};
    (*mapValidCommands)[GameState::Issue_Orders] = {"issueorder", "endissueorders"};
    (*mapValidCommands)[GameState::Execute_Orders] = {"execorder", "endexecorders", "win"};
    (*mapValidCommands)[GameState::Win] = {"play", "end"};
}

// Deep Copy Constructor
GameEngine::GameEngine(const GameEngine& copy)
{
    // Allocating new memory for the current game state and copy its value
    currentGameState = new GameState(*copy.currentGameState);

    // Allocating new memory for command map and copying it
    mapCommand = new std::map<std::string, Command>(*(copy.mapCommand));

    // Allocating new memory for the valid commands map and copying it
    mapValidCommands = new std::map<GameState, std::vector<std::string>>(*(copy.mapValidCommands));
}

// Destructor
GameEngine::~GameEngine()
{
    // Delete the pointer and free memory allocated for the current game state
    delete currentGameState;
    // Delete the pointer and free memory allocated for the command map
    delete mapCommand;
    // Delete the pointer and free memory allocated for the valid commands map
    delete mapValidCommands;
}

// Function to convert a string to lowercase
std::string GameEngine::toLowerCase(const std::string& str) const
{
    std::string s = str;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// Assign the Game Engine to another (deep copy)
GameEngine& GameEngine::operator=(const GameEngine& copy)
{
    if(this != &copy)
    {
        // Free Existing memory allocated
        delete currentGameState;
        delete mapCommand;
        delete mapValidCommands;

        // Do a deep copy of the Game Engine
        currentGameState = new GameState(*copy.currentGameState);
        mapCommand = new std::map<std::string, Command>(*(copy.mapCommand));
        mapValidCommands = new std::map<GameState, std::vector<std::string>>(*(copy.mapValidCommands));
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const GameEngine& gameEngine)
{
    // Print the current game state
    os << "Current Game State: " << gameEngine.getCurrentState() << "\n\n";
    
    // Print the valid commands for the current game state
    os << "Valid Commands: \n";
    GameState currentGameState = *(gameEngine.currentGameState);

    // Check if there are any valid commands for the current game state
    if(gameEngine.mapValidCommands->count(currentGameState) > 0)
    {
        // List of valid commands for the current game state
        const std::vector<std::string>& commands = (*gameEngine.mapValidCommands)[currentGameState];
        for(const std::string& command: commands)
        {
            // Description from the command map and print it
            const Command& commandDesc = (*gameEngine.mapCommand).at(command);
            os << " - " << command << ": " << commandDesc.description << "\n";
        }
    }

    return os;
}

// Function to process user input and make the respective transition
void GameEngine::manageCommand(const std::string& command)
{
    // Convert the user input string to lowercase to ignore case sensitivity
    std::string lowerStringCommand = toLowerCase(command);

    // Check if the command is valid for the current game state
    if(isCommandValid(lowerStringCommand))
    {
        // Find the command in the command map
        auto c = mapCommand->find(lowerStringCommand);
        if(c != mapCommand->end())
        {
            // If it's a valid command then transition to the next game state
            *currentGameState = c->second.nextState;
            std::cout << "\nTransitioned to game state: " << getCurrentState() << "\n\n";

            // If we reach the game state End
            if(*currentGameState == GameState::End)
            {
                std::cout << "Game Finished.\n";
                // Call the destructor
                delete this;
                // Exit application
                exit(0);
            }
        }
    }
    else
    {
        // Invalid command for the current game state
        std::cout << "\nInvalid command for the current game state.\n";
    }
}

// Get the current game state as string
std::string GameEngine::getCurrentState() const
{
    switch(*currentGameState)
    {
        case GameState::Start:
            return "Start";
        case GameState::Map_Loaded:
            return "Load Map";
        case GameState::Map_Validated:
            return "Validate Map";
        case GameState::Players_Added:
            return "Add Player";
        case GameState::Assign_Reinforcement:
            return "Assign Reinforcement";
        case GameState::Issue_Orders:
            return "Issue Orders";
        case GameState::Execute_Orders:
            return "Execute Orders";
        case GameState::End:
            return "End";
        default:
            return "Invalid State";
    }
}

// Display valid commands
void GameEngine::displayCommands() const
{
    std::cout << "Valid commands: \n";
    for(const auto& cmd : *mapCommand)
    {
        std::cout << " - " << cmd.first << ": " << cmd.second.description << "\n";
    }
}

// Check if the command input is a valid for the current game state
bool GameEngine::isCommandValid(const std::string& command) const
{
    // Get the valid commands for the current game state
    auto validC = mapValidCommands->find(*currentGameState);
    // Check if the command exist in the list of valid commands
    if(validC != mapValidCommands->end())
    {
        const std::vector<std::string>& validCmd = validC->second;
        return std::find(validCmd.begin(), validCmd.end(), command) != validCmd.end();
    }

    // The command wasn't found in the list of valid commands for the current game state
    return false;
}