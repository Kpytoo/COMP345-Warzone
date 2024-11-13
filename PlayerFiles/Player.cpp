#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <set>

#include "PlayerFiles/Player.h"
#include "CardsFiles/Cards.h"
#include "OrdersFiles/Orders.h"
#include "MapFiles/Map.h"

// Default constructor: initializes player with empty name, zero armies, and new Hand and OrdersList instances
Player::Player() : playerName(""), OwnedTerritories(), playerHand(new Hand()), ordersList(new OrdersList()), numArmies(0)
{
}

// Parameterized constructor: initializes player with a given name and new Hand and OrdersList instances
Player::Player(std::string playerName, const std::vector<Territory *> &ownedTerritories)
    : playerName(playerName), OwnedTerritories(ownedTerritories), playerHand(new Hand()), ordersList(new OrdersList()), numArmies(0)
{
}

Player::Player(std::string name) : playerName(name), OwnedTerritories(), playerHand(new Hand()), ordersList(new OrdersList()), numArmies(0)
{
}

// Destructor: frees memory allocated for player's hand and orders list
Player::~Player()
{
    delete playerHand;
//    delete ordersList;
}

// Copy constructor: creates a deep copy of another Player object
Player::Player(const Player &other)
{
    playerName = other.playerName;
    numArmies = other.numArmies;

    // Deep copy for playerHand and ordersList
    playerHand = new Hand(*other.playerHand);
    ordersList = new OrdersList(*other.ordersList);
    playerHand = new Hand();
    ordersList = new OrdersList();

    // Shallow copy of owned territories (assuming pointer structure is shared)
    OwnedTerritories = other.OwnedTerritories;
    toAttackTerritories = other.toAttackTerritories;
    toDefendTerritories = other.toDefendTerritories;
}

// Assignment operator: assigns deep copies of resources to the current Player
Player &Player::operator=(const Player &other)
{
    if (this == &other)
        return *this;

    // Copy player details
    playerName = other.playerName;
    numArmies = other.numArmies;

    // Clean up current resources
    delete playerHand;
    delete ordersList;

    // Allocate new copies for playerHand and ordersList
    playerHand = new Hand(*other.playerHand);
    ordersList = new OrdersList(*other.ordersList);
    playerHand = new Hand();
    ordersList = new OrdersList();

    // Shallow copy of owned territories
    OwnedTerritories = other.OwnedTerritories;
    toAttackTerritories = other.toAttackTerritories;
    toDefendTerritories = other.toDefendTerritories;
    return *this;
}

// Stream insertion operator: outputs player details to an output stream
std::ostream &operator<<(std::ostream &os, const Player &obj)
{
    os << "Player Name: " << obj.playerName << std::endl;
    os << "Number of Armies: " << obj.numArmies << std::endl;

    os << "Owned Territories: ";
    for (auto &territory : obj.OwnedTerritories)
    {
        os << territory->name << " "; // Outputs each territory's name
    }
    os << std::endl;

    os << "To Defend Territories: ";
    for (auto &territory : obj.toDefendTerritories)
    {
        os << territory->name << " "; // Outputs each territory's name
    }
    os << std::endl;

    os << "To Attack Territories: ";
    for (auto &territory : obj.toAttackTerritories)
    {
        os << territory->name << " "; // Outputs each territory's name
    }
    os << std::endl;

    // Outputs player's hand and orders list (assuming operator<< is defined for Hand and OrdersList)
    os << "Player Hand: " << *(obj.playerHand) << std::endl;
    os << "Orders List: " << *(obj.ordersList) << std::endl;

    return os;
}

// Getter definitions: retrieve player details
std::string Player::getPlayerName() const { return playerName; }
std::vector<Territory *> &Player::getOwnedTerritories() { return OwnedTerritories; }
std::vector<Territory *> &Player::getToDefendTerritories() { return toDefendTerritories; }
std::vector<Territory *> &Player::getToAttackTerritories() { return toAttackTerritories; }
Hand *Player::getPlayerHand() { return playerHand; }
OrdersList *Player::getOrdersList() const { return ordersList; }
int Player::getNumArmies() const { return numArmies; }

// Setter definitions: modify player details
void Player::setPlayerName(const std::string &name) { playerName = name; }
void Player::setOwnedTerritories(const std::vector<Territory *> &territories) { OwnedTerritories = territories; }
void Player::setToDefendTerritories(const std::vector<Territory *> &territories) { toDefendTerritories = territories; }
void Player::setToAttackTerritories(const std::vector<Territory *> &territories) { toAttackTerritories = territories; }
void Player::setPlayerHand(Hand *hand) { playerHand = hand; }
void Player::setOrdersList(OrdersList *ordersList) { this->ordersList = ordersList; }
void Player::setNumArmies(int numArmies) { this->numArmies = numArmies; }

/**
 * Returns a list of territories that the player should defend.
 *
 * This method generates a list of territories owned by the player and
 * marks them as territories to defend. The list is stored in the
 * `toDefendTerritories` member variable, which is updated by
 * the `setToDefendTerritories()` method.
 *
 * @return A vector of pointers to the territories that the player needs to defend.
 */
std::vector<Territory *> Player::toDefend()
{
    // Set the player's owned territories as the territories to defend
    setToDefendTerritories(getOwnedTerritories());
    // Return the list of territories to defend
    return toDefendTerritories;
}

/**
 * Returns a list of territories that the player should attack.
 *
 * This method generates a list of enemy territories that the player can attack.
 * It does this by checking the player's own "to defend" territories and finding
 * adjacent territories that are not owned by the player which means enemy territories.
 *
 * @return A vector of pointers to the territories that the player should attack.
 */
std::vector<Territory *> Player::toAttack()
{
    // Create a set to store the enemy territories that the player can attack
    std::set<Territory *> enemyTerritories;

    // Loop through each territory the player needs to defend
    for (Territory *t : toDefend())
    {
        // Loop through the adjacent territories of the current defending territory
        for (const auto &pair : t->adjacentTerritories)
        {
            // Flag to check if the adjacent territory is an enemy territory
            bool isEnemyT = true;

            // Loop through the player's own defended territories to check if this is one of them
            for (Territory *ownedT : toDefend())
            {
                // If the adjacent territory is one of the player's own, mark it as not an enemy
                if (pair.second == ownedT)
                {
                    // Break out of the loop as we know this is not an enemy territory
                    isEnemyT = false;
                    break;
                }
            }

            // If the adjacent territory is not owned by the player, it's an enemy territory
            if (isEnemyT)
            {
                // Add the enemy territory to the set
                enemyTerritories.insert(pair.second);
            }
        }
    }

    // Convert the set of enemy territories to a vector
    std::vector<Territory *> enemyTerritoriesVector(enemyTerritories.begin(), enemyTerritories.end());
    // Set the list of enemy territories as the territories to attack
    setToAttackTerritories(enemyTerritoriesVector);

    // Return the list of territories the player should attack
    return toAttackTerritories;
}

Player* Player::FindTerritoryOwner(const std::string& territoryName, const std::vector<Player*>& players) {
    // Search through all players
    for (Player* player : players) {
        // Check each territory owned by the current player
        for (Territory* territory : player->getOwnedTerritories()) {
            if (territory->name == territoryName) {
                return player;
            }
        }
    }
    // Territory not found or not owned by any player
    return nullptr;
}

/**
 * Issues a new order based on the provided order type and adds it to the player's orders list.
 *
 * The function handles different types of orders, including "deploy", "advance", "airlift", "bomb", "blockade", and "negotiate".
 * The method will check if the player has army units left in the reinforcement pool before issuing an order other than "deploy".
 * Depending on the order type, it will prompt the player for additional details.
 *
 * @param orderType The type of the order to be issued. Possible values include "deploy", "advance", "airlift", "bomb", "blockade", and "negotiate".
 */
void Player::issueOrder(std::string orderType, Deck *deck)
{
    // Check if the player has no army units left in the reinforcement pool and tries to issue another order than deploy.
    if (orderType != "deploy" && reinforcement_units != 0) {
        // Print a message informing the player they still have army units in the reinforcement pool and should deploy them.
        std::cout << "You have army units left in the reinforcement pool!\nDeploy your army units!\n\n";
        // Exit the function without issuing the order if the player needs to issue deploy orders.
        return;
    }

    // Stores the name of the source territory for the order.
    std::string sourceTName = "";
    // Stores the name of the target territory for the order.
    std::string targetTName = "";
    // Stores the number of army units involved in the order.
    int armies = 0;
    // Flag to track if the player has the required card in their hand
    bool hasCard = false;

    // Handle the "deploy" order type.
    if (orderType == "deploy")
    {
        std::cout << "- Owned Territories -\n";
        // Display a list of the player's owned territories to choose from.
        for (Territory *t : getOwnedTerritories())
        {
            // Print each owned territory's name.
            std::cout << t->name << std::endl;
        }

        // Prompt the player to enter the name of the target territory to deploy army units.
        std::cout << "\nPlease enter the territory's name where you want to deploy your army units :";
        // Get the name of the target territory.
        std::getline(std::cin, targetTName);

        // Display how many army units the player has left in their reinforcement pool.
        std::cout << "Number of army units left in the reinforcement pool: " << getNumArmies() << std::endl;
        // Prompt the player to enter the number of army units they wish to deploy to the selected territory.
        std::cout << "Please enter the number of army units you want to deploy in " << targetTName << ": ";
        // Get the number of army units to deploy.
        std::string input0;
        std::getline(std::cin, input0);
        armies = std::stoi(input0);
        // Create a new DeployOrder object and add it to the orders list.
        ordersList->ordersVector.push_back(new DeployOrder(this, targetTName, armies));
    }
    // Handle the "advance" order type.
    else if (orderType == "advance")
    {
        // Display a list of territories the player can attack.
        std::cout << "Current list of territories to attack:\n";
        for (Territory *t : getToAttackTerritories())
        {
            // Print each territory available for attack.
            std::cout << t->name << std::endl;
        }

        // Display a list of territories the player needs to defend.
        std::cout << "\nCurrent list of territories to defend:\n";
        for (Territory *t : toDefend())
        {
            // Print each territory that needs defending.
            std::cout << t->name << std::endl;
        }

        // Prompt the player to choose a source territory (where they have army units stationed).
        std::cout << "\n\nChoose a source territory (army units should be on standby there): ";
        // Get the name of the source territory.
        std::getline(std::cin, sourceTName);

        // Prompt the player to choose a target territory to advance to.
        std::cout << "\nChoose a target territory to advance to: ";
        // Get the name of the target territory.
        std::getline(std::cin, targetTName);

        // Prompt the player to enter the number of army units they want to advance.
        std::cout << "\nEnter the number of army units to execute an advance order: ";
        // Get the number of army units to advance.
        std::string input0;
        std::getline(std::cin, input0);
        armies = std::stoi(input0);

        // Create a new AdvanceOrder object and add it to the orders list.
        Player* enemyPlayer = FindTerritoryOwner(targetTName, *players); // Look for owner of target territory, if any
        ordersList->ordersVector.push_back(new AdvanceOrder(this, enemyPlayer, sourceTName, targetTName, armies));
    }
    else if (std::find(cardTypes.begin(), cardTypes.end(), orderType) != cardTypes.end())
    { // order type is valid
        // Handle the orders that requires a card to be used from the player's deck

        // Loop through each card in the player's hand to check for an "airlift" card
        for (Card *c : this->getPlayerHand()->handVector)
        {
            // If the card of type is found
            if (c->cardType == orderType)
            {
                // Set the flag to true when an "airlift" card is found
                hasCard = true;
                // Issue the order by playing the card
                c->play(*this->ordersList, *deck, *this->getPlayerHand());
                // Exit the loop early since we've found the card
                break;
            }
        }

        // If no "airlift" card is found
        if (!hasCard)
        {
            // Print an error message and exit the function
            std::cout << "You do not have an " << orderType << " card in your hand!\n";
            return;
        }
    }
    else
    {
        std::cout << "Invalid Order Type! Please try again! \n\n";
    }
}

Player::Player(const std::string &playerName, const std::vector<Territory *> &ownedTerritories,
               std::vector<Player *> *players) : playerName(playerName), OwnedTerritories(ownedTerritories),
                                                 players(players), playerHand(new Hand()) {}
