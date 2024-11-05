#include "GameEngine.h"

/**
 * Default constructor for the GameEngine class.
 * Initializes the game engine to the starting state and allocates memory for command maps.
 */
GameEngine::GameEngine()
{
    // Allocating memory for the game engine and initializing it to the game state Start
    currentGameState = new GameState(GameState::Start);

    // Allocating memory for the command map
    mapCommand = new std::map<std::string, Command>();

    // Allocating memory for the valid commands map
    mapValidCommands = new std::map<GameState, std::vector<std::string>>();
    
    // Pairing string commands to their respective game states with descriptions
    // Maps "loadmap" to Map_Loaded state
    (*mapCommand)["loadmap"] = {GameState::Map_Loaded, "Loading a Map"};

    // Maps "validatemap" to Map_Validated state
    (*mapCommand)["validatemap"] = {GameState::Map_Validated, "Validate the Loaded Map"};

    // Maps "addplayer" to Players_Added state
    (*mapCommand)["addplayer"] = {GameState::Players_Added, "Add Players To The Game"};

    // Maps "assigncountries" to Assign_Reinforcement state
    (*mapCommand)["assigncountries"] = {GameState::Assign_Reinforcement, "Assign Countries To Players"};
    
    // Maps "issueorder" to Issue_Orders state
    (*mapCommand)["issueorder"] = {GameState::Issue_Orders, "Issue Orders"};
    
    // Maps "endissueorders" to Execute_Orders state
    (*mapCommand)["endissueorders"] = {GameState::Execute_Orders, "Execute Orders"};
    
    // Maps "execorder" to Execute_Orders state
    (*mapCommand)["execorder"] = {GameState::Execute_Orders, "Execute Orders"};
    
    // Maps "endexecorders" to Assign_Reinforcement state
    (*mapCommand)["endexecorders"] = {GameState::Assign_Reinforcement, "Assign Countries To Players"};
    
    // Maps "win" to Win state
    (*mapCommand)["win"] = {GameState::Win, "You Won!"};
    
    // Maps "end" to End state
    (*mapCommand)["end"] = {GameState::End, "Game Finished"};
    
    // Maps "play" to Start state
    (*mapCommand)["play"] = {GameState::Start, "Restart The Game"};

    // Define valid commands for each game state
    (*mapValidCommands)[GameState::Start] = {"loadmap"};

    // Valid commands for Map_Loaded state
    (*mapValidCommands)[GameState::Map_Loaded] = {"loadmap", "validatemap"};

    // Valid command for Map_Validated state
    (*mapValidCommands)[GameState::Map_Validated] = {"addplayer"};

    // Valid commands for Players_Added state
    (*mapValidCommands)[GameState::Players_Added] = {"addplayer", "assigncountries"};

    // Valid command for Assign_Reinforcement state
    (*mapValidCommands)[GameState::Assign_Reinforcement] = {"issueorder"};

    // Valid commands for Issue_Orders state
    (*mapValidCommands)[GameState::Issue_Orders] = {"issueorder", "endissueorders"};

    // Valid commands for Execute_Orders state
    (*mapValidCommands)[GameState::Execute_Orders] = {"execorder", "endexecorders", "win"};

    // Valid commands for Win state
    (*mapValidCommands)[GameState::Win] = {"play", "end"};

    playersList = {};
    currentMap = nullptr;
}

/**
 * Deep copy constructor for the GameEngine class.
 * 
 * @param copy The GameEngine object to copy.
 */
GameEngine::GameEngine(const GameEngine& copy)
{
    // Allocating new memory for the current game state and copying its value
    currentGameState = new GameState(*copy.currentGameState);

    // Allocating new memory for the command map and copying it
    mapCommand = new std::map<std::string, Command>(*(copy.mapCommand));

    // Allocating new memory for the valid commands map and copying it
    mapValidCommands = new std::map<GameState, std::vector<std::string>>(*(copy.mapValidCommands));
}

/**
 * Destructor for the GameEngine class.
 * Frees the dynamically allocated memory.
 */
GameEngine::~GameEngine()
{
    // Delete the pointer and free memory allocated for the current game state
    delete currentGameState;

    // Delete the pointer and free memory allocated for the command map
    delete mapCommand;

    // Delete the pointer and free memory allocated for the valid commands map
    delete mapValidCommands;

    delete currentMap;
    for(auto player : playersList)
    {
        delete player;
    }
}

/**
 * Function to convert a string to lowercase.
 * 
 * This method takes an input string, creates a copy of it, and transforms all its characters to lowercase.
 * It uses the std::transform function to iterate over the string and applies the tolower function to each character.
 * 
 * @param str The input string to convert to lower case.
 * @return The lowercase version of the input string.
 */
std::string GameEngine::toLowerCase(const std::string& str) const
{
    // Create a copy of the input string
    std::string s = str;

    // Transform to lowercase
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    // Return the lowercase string
    return s;
}

/**
 * Assignment operator overload for deep copying another GameEngine object.
 * 
 * This method checks for self-assignment and, if necessary, frees existing memory allocated 
 * for the current object before performing a deep copy of the provided GameEngine object.
 * 
 * @param copy The GameEngine object to copy from.
 * @return Reference to the current object.
 */
GameEngine& GameEngine::operator=(const GameEngine& copy)
{
    // Check for self-assignment
    if(this != &copy)
    {
        // Free existing memory allocated for current game state
        delete currentGameState;

        // Free existing memory allocated for command map
        delete mapCommand;

        // Free existing memory allocated for valid commands map
        delete mapValidCommands;

        // Perform a deep copy of the GameEngine
        // Copy the current game state
        currentGameState = new GameState(*copy.currentGameState);

        // Copy the command map
        mapCommand = new std::map<std::string, Command>(*(copy.mapCommand));

        // Copy the valid commands map
        mapValidCommands = new std::map<GameState, std::vector<std::string>>(*(copy.mapValidCommands));
    }

    // Return a reference to the current object
    return *this;
}

/**
 * Overloaded stream insertion operator for the GameEngine class.
 * 
 * This function prints the current game state and 
 * the valid commands for that state to the provided output stream.
 * 
 * @param os The output stream where the game state and commands will be printed.
 * @param gameEngine The GameEngine object whose state and commands are to be printed.
 * @return The output stream after printing the game state and valid commands.
 */
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
        
        // Iterate through each valid command
        for(const std::string& command: commands)
        {
            // Get the command's description from the command map
            const Command& commandDesc = (*gameEngine.mapCommand).at(command);

            // Print the command and its description to the output stream
            os << " - " << command << ": " << commandDesc.description << "\n";
        }
    }

    // Return the output stream for further chaining or printing
    return os;
}

/**
 * Function to process user input and make the respective transition.
 *
 * This function takes a user command, checks its validity in the current game state,
 * and performs the appropriate state transition if valid.
 *
 * @param command The user input command to process.
 */
void GameEngine::manageCommand(const std::string& command)
{
    // Convert the user input string to lowercase to ignore case sensitivity
    std::string lowerStringCommand = toLowerCase(command);

    // Check if the command is valid for the current game state
    if(isCommandValid(lowerStringCommand))
    {
        // Find the command in the command map
        auto c = mapCommand->find(lowerStringCommand);

        // Check if the command was found in the command map
        if(c != mapCommand->end())
        {
            // If it's a valid command, then transition to the next game state
            *currentGameState = c->second.nextState;
            // Notify the user about the transition to the new game state
            std::cout << "\nTransitioned to game state: " << getCurrentState() << "\n\n";

            // If we reach the game state End
            if(*currentGameState == GameState::End)
            {
                // Notify the user that the game is finished
                std::cout << "Game Finished.\n\n";

                // Call the destructor to free resources
                delete this;

                // Exit application and terminate the program
                exit(0);
            }
        }
    }
    else
    {
        // If the command is invalid for the current game state, inform the user
        std::cout << "\n**ERROR: Invalid command for the current game state.**\nTry again!\n";
    }
}

/**
 * Get the current game state as a string.
 *
 * This function returns a string representation of the current game state,
 * allowing external components to understand the game's status.
 *
 * @return A string describing the current game state.
 */
std::string GameEngine::getCurrentState() const
{
    // Switch statement to determine the current game state and return its string representation
    switch(*currentGameState)
    {
        // Return "Start" if the state is Start
        case GameState::Start:
            return "Start";

        // Return "Load Map" if the state is Map_Loaded
        case GameState::Map_Loaded:
            return "Load Map";

        // Return "Validate Map" if the state is Map_Validated
        case GameState::Map_Validated:
            return "Validate Map";

        // Return "Add Player" if the state is Players_Added
        case GameState::Players_Added:
            return "Add Player";

        // Return "Assign Reinforcement" if the state is Assign_Reinforcement
        case GameState::Assign_Reinforcement:
            return "Assign Reinforcement";

        // Return "Issue Orders" if the state is Issue_Orders
        case GameState::Issue_Orders:
            return "Issue Orders";

        // Return "Execute Orders" if the state is Execute_Orders
        case GameState::Execute_Orders:
            return "Execute Orders";

        // Return "End" if the state is End
        case GameState::End:
            return "End";

        // Return "Invalid State" for any unrecognized state
        default:
            return "Invalid State";
    }
}

/**
 * Display valid commands for the current game state.
 *
 * This function outputs a list of all valid commands along with 
 * their descriptions to the console, helping users understand what actions they can take.
 */
void GameEngine::displayCommands() const
{
    // Print a header for valid commands
    std::cout << "Valid commands: \n";

    // Iterate over each command in the command map
    for(const auto& cmd : *mapCommand)
    {
        // Output the command name and its description
        std::cout << " - " << cmd.first << ": " << cmd.second.description << "\n";
    }
}

/**
 * Check if the command input is valid for the current game state.
 *
 * This function verifies whether a given command is valid
 * based on the current state of the game.
 *
 * @param command The command string to validate.
 * @return True if the command is valid for the current game state; otherwise, false.
 */
bool GameEngine::isCommandValid(const std::string& command) const
{
    // Get the valid commands for the current game state
    auto validC = mapValidCommands->find(*currentGameState);

    // Check if the current game state has any valid commands
    if(validC != mapValidCommands->end())
    {
        // Retrieve the list of valid commands for the current game state
        const std::vector<std::string>& validCmd = validC->second;

        // Check if the given command exists in the list of valid commands
        return std::find(validCmd.begin(), validCmd.end(), command) != validCmd.end();
    }

    // The command wasn't found in the list of valid commands for the current game state
    return false;
}

void GameEngine::mainGameLoop()
{
    while(playersList.size() > 1)
    {
        for(int i = 0; i < playersList.size(); i++)
        {
            if(playersList[i]->getOwnedTerritories().empty())
            {
                delete playersList[i];
                playersList.erase(playersList.begin() + i);
                i--;
                continue;
            }

            reinforcementPhase(playersList[i]);
        }

        for(int i = 0; i < playersList.size(); i++)
        {
            issueOrdersPhase(playersList[i]);
        }

        executeOrdersPhase();
    }

    std::cout << "\nGame Over! Player " << playersList[0]->getPlayerName() << "has won! \n\n";
}

void GameEngine::reinforcementPhase(Player* player)
{
    // Display whose turn it is for their reinforcement phase 
    std::cout << "Reinforcement Phase for " << player->getPlayerName() << std::endl;

    // number of territories that the current player owns
    int numberOfTerritories = player->getOwnedTerritories().size();
    // give the number of army units in their reinforcement pool depending on the number of territories owned
    player->setNumArmies(std::max(static_cast<int>(3), static_cast<int>(std::floor(numberOfTerritories / 3))));

    // for each continent in the current map
    for(const auto& continentPair : currentMap->continents)
    {
        // get the continent value from the pair
        Continent* continent = continentPair.second;
        // assume player owns the current continent
        bool continentOwned = true;

        // for each territory in the current continent
        for(const auto& territoryPair : continent->childTerritories)
        {
            // get the territory value from the pair
            Territory* territory = territoryPair.second;
            // assume the player do not own the current territory
            bool territoryOwned = false;

            // for each territory in the player's owned territories
            for(const auto& ownedTerritory : player->getOwnedTerritories())
            {
                // if we find the current territory's name in the list of the player's owned territories
                if(ownedTerritory->name == territory->name)
                {
                    // then the player owns the territory
                    territoryOwned = true;
                    break;
                }
            }

            // if we find that the current territory isn't owned by the player
            if(!territoryOwned)
            {
                // then the player do not own the continent
                continentOwned = false;
                break;
            }
        }

        // if the player owns the current continent
        if(continentOwned)
        {
            // then update the number of army units in their reinforcement pool by adding the extra army units from the continent's bonus
            player->setNumArmies(player->getNumArmies() + continent->bonusPoints);
        }
    }
}

void GameEngine::issueOrdersPhase(Player* player)
{
    // Display whose turn it is for their issuing order phase 
    std::cout << "Issuing Orders Phase for " << player->getPlayerName() << std::endl;
    std::cout << "Invalid Orders will be ignored and won't be executed so please issue your orders carefully!\n\n";

    bool noMoreOrders = false;
    bool invalidO = false;
    std::string inputO;
    std::string inputOrderType;

    while(!noMoreOrders)
    {
        do
        {
            invalidO = false;
            std::cout << "Issue an Order? (Y/N): ";
            std::cin >> inputO;
            std::cout << "\n\n";

            if(toLowerCase(inputO) == "y")
            {
                std::cout << "- Order Types -\n\n";
                std::cout << "\t- Deploy\n\t- Advance\n\t- Airlift\n\t- Bomb\n\t- Blockade\n\t- Negotiate\n\n";
                std::cout << "Please issue an order type: ";
                std::cin >> inputOrderType;

                player->issueOrder(toLowerCase(inputOrderType));
            }
            else if(toLowerCase(inputO) == "n")
            {
                noMoreOrders = true;
            }
            else
            {
                std::cout << "Invalid Statement! Try again!\n\n ";
                invalidO = true;
            }
        } 
        while (invalidO);
    }

    std::cout << "Current list of territories to attack:\n";
    for(Territory* t : player->getToAttackTerritories())
    {
        std::cout << t->name << std::endl;
    }

    std::cout << "\nCurrent list of territories to defend:\n";
    for(Territory* t : player->getToDefendTerritories())
    {
        std::cout << t->name << std::endl;
    }
    
}

void GameEngine::executeOrdersPhase()
{
    bool ordersLeft = true;

    while(ordersLeft)
    {
        ordersLeft = false;
        
        for(int i = 0; i < playersList.size(); i++)
        {
            if(playersList[i]->getOrdersList()->ordersVector.empty())
            {
                continue;
            }

            ordersLeft = true;
            playersList[i]->getOrdersList()->ordersVector.front()->execute();
            playersList[i]->getOrdersList()->ordersVector.erase(playersList[i]->getOrdersList()->ordersVector.begin());
        } 
    }
}