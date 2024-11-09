#include <cmath>
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

    // Might need to modify these values for other parts
    playersList = {};
    currentMap = nullptr;
    mainDeck = nullptr;
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

// get players list, was just created for GameEngineDriver - can delete method if not needed
 std::vector<Player*>& GameEngine::getPlayersList()
 {
    return playersList;
 }

// set a game map, was just created for GameEngineDriver - can delete method if not needed
 void GameEngine::setCurrentMap(Map* map)
 {
    currentMap = map;
 }

// set a game map, was just created for GameEngineDriver - can delete method if not needed
 void GameEngine::setGameDeck(Deck* deck)
 {
    mainDeck = deck;
 }

/**
 * Main game loop that runs the core gameplay sequence.
 * 
 * This method continuously executes the game phases: reinforcement, issue orders,
 * and execute orders, until only one player remains. Players are removed from the game
 * if they have no territories left. After the loop finishes, the winner is announced.
 * 
 * @see reinforcementPhase(Player* player)  The phase where players get army units in their reinforcement pool.
 * @see issueOrdersPhase(Player* player)     The phase where players issue orders.
 * @see executeOrdersPhase()                 The phase where issued orders are executed.
 */
void GameEngine::mainGameLoop()
{
    // Loop continues as long as there is more than one player
    while(playersList.size() > 1)
    {
        // Switch to the game state Assign_Reinforcement
        *currentGameState = GameState::Assign_Reinforcement;

         // Iterate through each player to perform the reinforcement phase
        for(int i = 0; i < playersList.size(); i++)
        {
             // Check if the player has no territories left
            if(playersList[i]->getOwnedTerritories().empty())
            {
                // Delete the player object to free memory
                delete playersList[i];
                // Remove the player from the list of active players
                playersList.erase(playersList.begin() + i);
                // Decrement the index to recheck the current position after removal
                i--;
                // Skip the eliminated player for the current iteration, move to the next
                continue;
            }

            // Call the reinforcement phase for the player 
            reinforcementPhase(playersList[i]);
        }

        //Switch to game state Issue_Orders
        *currentGameState = GameState::Issue_Orders;
        // Iterate through each player to perform the issue orders phase
        for(int i = 0; i < playersList.size(); i++)
        {
            // Issue orders phase for the player (player decides actions)
            issueOrdersPhase(playersList[i]);
        }

        //Switch to game state Execute_Orders
        *currentGameState = GameState::Execute_Orders;
        // Execute the orders that have been issued by the players
        executeOrdersPhase();
    }

    // When one player remains, announce them as the winner
    std::cout << "\nGame Over! Player " << playersList[0]->getPlayerName() << "has won! \n\n";
    // Switch to the game state Win
    *currentGameState = GameState::Win;
}

/**
 * Executes the Reinforcement Phase for a given player.
 * 
 * In this phase, the player receives reinforcements based on the number of territories
 * they own, with a minimum of 3 army units. Additionally, if the player controls any
 * continents, they receive a continent bonus added to their reinforcement pool.
 * 
 * @param player The player who is undergoing the reinforcement phase. Their army units
 *               will be updated based on their owned territories and continent bonuses.
 * 
 * @see Player::getOwnedTerritories()       Method to retrieve the player's owned territories.
 * @see Player::setNumArmies(int numArmies) Method to set the number of armies in the player's reinforcement pool.
 * @see Continent::bonusPoints              The bonus points given to the player if they own the entire continent.
 */
void GameEngine::reinforcementPhase(Player* player)
{
    // Display whose turn it is for their reinforcement phase 
    std::cout << "Reinforcement Phase for " << player->getPlayerName() << std::endl;

    // Number of territories that the current player owns
    int numberOfTerritories = player->getOwnedTerritories().size();
    // Calculate the number of army units based on territories owned (minimum of 3)
    player->setNumArmies(std::max(static_cast<int>(3), static_cast<int>(std::floor(numberOfTerritories / 3))));

    // Iterate over each continent in the map to check for continent ownership
    for(const auto& continentPair : currentMap->continents)
    {
        // Retrieve the continent object from the pair
        Continent* continent = continentPair.second;
        // Assume the player owns the continent initially
        bool continentOwned = true;

        // Check each territory in the continent to see if the player owns it
        for(const auto& territoryPair : continent->childTerritories)
        {
            // Retrieve the territory object from the pair
            Territory* territory = territoryPair.second;
            // Assume the player does not own the current territory
            bool territoryOwned = false;

            // Check if the player owns the territory
            for(const auto& ownedTerritory : player->getOwnedTerritories())
            {
                // If we find the current territory's name in the list of the player's owned territories
                if(ownedTerritory->name == territory->name)
                {
                    // then the player owns the territory
                    territoryOwned = true;
                    break;
                }
            }

             // If the player doesn't own this territory
            if(!territoryOwned)
            {
                // then the player do not own the continent
                continentOwned = false;
                break;
            }
        }

        // If the player owns the entire continent, grant the continent's bonus
        if(continentOwned)
        {
            // Add the continent bonus to the player's reinforcement pool
            player->setNumArmies(player->getNumArmies() + continent->bonusPoints);
        }
    }

    // Output the final number of army units in the player's reinforcement pool
    std::cout << "Player " << player->getPlayerName() << " has " << player->getNumArmies() << " army units in their reinforcement pool.\n\n";
}

/**
 * Executes the Issuing Orders Phase for a given player.
 * 
 * In this phase, the player can issue orders (e.g., Deploy, Advance, Airlift, etc.), and
 * also manage their orders list by moving or removing orders before execution. The phase
 * allows the player to either issue new orders or stop issuing orders. After all orders
 * are issued, the player can review and modify their orders list before the execution phase.
 * 
 * @param player The player who is issuing orders. Their orders list will be updated as they issue new orders or modify existing ones.
 * 
 * @see Player::issueOrder(std::string orderType, Deck* deck)       The method to issue a specific type of order based on player input.
 * @see Player::getOrdersList()                                     Method to retrieve the player's orders list for review and management.
 * @see OrdersList::move(int oldPos, int newPos)                    Method to move an order in the player's orders list.
 * @see OrdersList::remove(int position)                            Method to remove an order from the player's orders list.
 */
void GameEngine::issueOrdersPhase(Player* player)
{
    // Display whose turn it is for issuing order phase
    std::cout << "Issuing Orders Phase for " << player->getPlayerName() << std::endl;
    std::cout << "Invalid Orders will be ignored and won't be executed so please issue your orders carefully!\n\n";

    // Flag to determine if the player is finished issuing orders
    bool noMoreOrders = false;
    // Flag to handle invalid input during the order issuance process
    bool invalidO = false;
    // Flag to determine if the player wants to finalize their orders list
    bool finalOrders = false;

    // Input to issue an order or not
    std::string inputO;
    // Input for the type of order to issue
    std::string inputOrderType;
    // Input for managing the orders list (move or remove)
    std::string inputOrderAction;

    // Loop to allow the player to issue orders until they decide to stop
    while(!noMoreOrders)
    {
        // Prompt the player to issue an order or not, ensuring valid input
        do
        {
            // Reset invalid input flag
            invalidO = false;
            // Prompt user for whether they want to issue an order
            std::cout << "Issue an Order? (Y/N): ";
            std::cin >> inputO;
            std::cout << "\n\n";

            // If the player wants to issue an order, ask for the order type
            if(toLowerCase(inputO) == "y")
            {
                // Display available order types 
                std::cout << "- Order Types -\n\n";
                std::cout << "\t- Deploy\n\t- Advance\n\t- Airlift (Use one Airlift card)\n\t- Bomb (Use one Bomb card)\n\t- Blockade (Use one Blockade card)\n\t- Negotiate (Use one Diplomacy card)\n\n";
                
                // Print out the player's hand
                std::cout << *(player->getPlayerHand()) << std::endl;
                
                // Prompt player for selection
                std::cout << "Please issue an order type: ";
                std::cin >> inputOrderType;

                // Issue the order based on user input (order type)
                player->issueOrder(toLowerCase(inputOrderType), mainDeck);
            }
            // If the player doesn't want to issue an order, exit the loop
            else if(toLowerCase(inputO) == "n")
            {
                // No more orders to issue
                noMoreOrders = true;
            }
            // Handle invalid input from the player
            else
            {
                // Set invalid input flag to true to prompt again
                std::cout << "Invalid Statement! Try again!\n\n ";
                invalidO = true;
            }
        } 
        // Repeat the loop if input is invalid
        while (invalidO);
        
    }

    // Loop to allow the player to manage their orders list before execution
    while(!finalOrders)
    {
        // Prompt the player to modify the orders list or not, ensuring valid input
        do
        {
            // Reset invalid input flag
            invalidO = false;

            // Display the player's current orders list
            std::cout << "- Orders List -\n";
            for(size_t i = 0; i < player->getOrdersList()->ordersVector.size(); ++i)
            {
                std::cout << "Order " <<  i + 1 << " - " << player->getOrdersList()->ordersVector[i];
            }

            // Ask if the player wants to manage their orders list
            std::cout << "Would you like to manage your orders list before Orders Execution Phase? (Y/N): ";
            std::cin >> inputO;
            std::cout << "\n\n";

            // If the player wants to manage their orders list, prompt for action
            if(toLowerCase(inputO) == "y")
            {
                std::cout << "\nMove Order (m)\nRemove Order (r)\n\n";
                std::cout << "Please choose an option: ";
                std::cin >> inputOrderAction;

                // Handle moving an order in the list
                if(toLowerCase(inputOrderAction) == "m")
                {
                    // Current position of the order
                    int oldPos;
                    // Position to move the order to
                    int newPos;

                    // Prompt for the current position and the new position to move the order
                    std::cout << "Enter the current position of the order you want to move: ";
                    std::cin >> oldPos;
                    std::cout << "Enter the new position for the order: ";
                    std::cin >> newPos;

                    // Call method to move the order in the list
                    player->getOrdersList()->move(oldPos, newPos);

                    // Confirm the move was successful if positions are valid
                    if(oldPos > 0 && oldPos <= player->getOrdersList()->ordersVector.size() && 
                    newPos > 0 && newPos <= player->getOrdersList()->ordersVector.size())
                    {
                        std::cout << "Order moved successfully.\n\n";
                    }  
                }
                // Handle removing an order from the list
                else if(toLowerCase(inputOrderAction) == "r")
                {
                    // Current position of the order
                    int orderPos;

                    // Ask for the position of the order to remove
                    std::cout << "Enter the position of the order you want to remove: ";
                    std::cin >> orderPos;

                    // Call method to remove the order
                    player->getOrdersList()->remove(orderPos);

                    // Confirm the removal was successful if the position is valid
                    if(orderPos > 0 && orderPos <= player->getOrdersList()->ordersVector.size())
                    {
                        std::cout << "Order removed successfully.\n\n"; 
                    }                    
                }
                // No action is made on the orders list if input for action was invalid
                else
                {
                    std::cout << "Invalid Statement! No change in the orders list.\n\n ";
                }
            }
            // If the player doesn't want to manage their orders, finalize the orders
            else if(toLowerCase(inputO) == "n")
            {
                // No more changes to the orders list
                finalOrders = true;
            }
            // Handle invalid input for managing orders
            else
            {
                // Set invalid input flag to true to prompt again
                std::cout << "Invalid Statement! Try again!\n\n ";
                invalidO = true;
            }
        } 
        // Repeat the loop if input is invalid
        while (invalidO);
    }
}

/**
 * Executes all orders in each player's orders list during the Orders Execution Phase of the game.
 * 
 * In this phase, each player sequentially executes the orders in their orders list. If a player has no orders left to execute, the system skips them.
 * After executing an order, it is removed from the player's list. This process repeats until all orders for all players have been executed.
 * 
 * @see Player::getOrdersList()                  Method to retrieve a player's orders list.
 * @see OrdersList::ordersVector                The vector holding the player's orders.
 * @see Order::execute()                        Method to execute a specific order.
 */
void GameEngine::executeOrdersPhase()
{
    // Flag to check if there are any orders left to execute in any player's orders list
    bool ordersLeft = true;
    // Flag to check if there are any deploy orders left to execute in any player's orders list
    bool deployOver = false;

    // Loop until all orders in the players' orders lists are executed
    while(ordersLeft)
    {
        // Assume that there are no more orders left to execute across all players initially
        ordersLeft = false;
        
        if(!deployOver)
        {
            // Iterate through each player in the players list
            for(int i = 0; i < playersList.size(); i++)
            {
                // Display whose turn it is for the deploy orders execution phase
                std::cout << "Deploy Orders Execution Phase for " << playersList[i]->getPlayerName() << std::endl;

                // If the current player has no orders left in their orders list
                if(playersList[i]->getOrdersList()->ordersVector.empty())
                {
                    // Inform the player that they have no deploy orders to execute
                    std::cout << "No deploy orders to execute for " << playersList[i]->getPlayerName() << ".\n";

                    // Skip the current player and move on to the next player
                    continue;
                }

                // Iterate through the orders list of the current player
                for(int j = 0; j < playersList[i]->getOrdersList()->ordersVector.size(); j++)
                {
                    // If the order is of type "deploy", the order will be executed.
                    if(playersList[i]->getOrdersList()->ordersVector[j]->orderType == "deploy")
                    {
                        // If the player still has orders to execute, set ordersLeft to true to continue executing orders
                        ordersLeft = true;
                        // Executes the first deploy order in the player's orders list.
                        playersList[i]->getOrdersList()->ordersVector.front()->execute();
                        // After executing the order, remove it from the player's orders list
                        playersList[i]->getOrdersList()->ordersVector.erase(playersList[i]->getOrdersList()->ordersVector.begin());
                    }
                    // Stop checking further orders if the current order is not "deploy"
                    else
                    {
                        break;
                    }
                }
            }

            // Every player has finished executing their deploy orders in their orders list
            deployOver = true;
        }

        // Iterate through each player in the game
        for(int i = 0; i < playersList.size(); i++)
        {
            // Display whose turn it is for the orders execution phase
            std::cout << "Orders Execution Phase for " << playersList[i]->getPlayerName() << std::endl;

            // If the current player has no orders left in their orders list
            if(playersList[i]->getOrdersList()->ordersVector.empty())
            {
                // Inform the player that they have no more orders to execute
                std::cout << "No more orders to execute for " << playersList[i]->getPlayerName() << ".\n";

                // Skip the current player and move on to the next player
                continue;
            }

            // If the player still has orders to execute, set ordersLeft to true to continue executing orders
            ordersLeft = true;

            // Execute the first order in the player's orders list
            playersList[i]->getOrdersList()->ordersVector.front()->execute();
            // After executing the order, remove it from the player's orders list
            playersList[i]->getOrdersList()->ordersVector.erase(playersList[i]->getOrdersList()->ordersVector.begin());
        } 
    }
}