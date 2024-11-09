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

// Destructor: frees memory allocated for player's hand and orders list
Player::~Player()
{
    delete playerHand;
    delete ordersList;
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
std::vector<Territory *>& Player::getOwnedTerritories() { return OwnedTerritories; }
std::vector<Territory *>& Player::getToDefendTerritories() { return toDefendTerritories; }
std::vector<Territory *>& Player::getToAttackTerritories() { return toAttackTerritories; }
Hand *Player::getPlayerHand() const { return playerHand; }
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
    std::set<Territory*> enemyTerritories;

    // Loop through each territory the player needs to defend
    for(Territory* t : toDefend())
    {
        // Loop through the adjacent territories of the current defending territory
        for(const auto& pair : t->adjacentTerritories)
        {
            // Flag to check if the adjacent territory is an enemy territory
            bool isEnemyT = true;

            // Loop through the player's own defended territories to check if this is one of them
            for(Territory* ownedT : toDefend())
            {
                // If the adjacent territory is one of the player's own, mark it as not an enemy
                if(pair.second == ownedT)
                {
                    // Break out of the loop as we know this is not an enemy territory
                    isEnemyT = false;
                    break;
                }
            }

            // If the adjacent territory is not owned by the player, it's an enemy territory
            if(isEnemyT)
            {
                // Add the enemy territory to the set
                enemyTerritories.insert(pair.second);
            }
        }
    }

    // Convert the set of enemy territories to a vector
    std::vector<Territory*> enemyTerritoriesVector(enemyTerritories.begin(), enemyTerritories.end());
    // Set the list of enemy territories as the territories to attack
    setToAttackTerritories(enemyTerritoriesVector);
    
    // Return the list of territories the player should attack
    return toAttackTerritories;
}

// issueOrder: creates a new order and adds it to the player's orders list
void Player::issueOrder(std::string orderType)
{
    if(orderType != "deploy" && getNumArmies() == 0)
    {
        std::cout << "You have army units left in the reinforcement pool!\nDeploy your army units!\n\n";
        return;
    }

    std::string sourceTName = "";
    std::string targetTName = "";
    int armies = 0;

    if(orderType == "deploy")
    {
        std::cout << "- Owned Territories -\n";
        for(Territory* t : getOwnedTerritories())
        {
            std::cout << t->name << std::endl;
        }

        std::cout << "\nPlease enter the territory's name where you want to deploy your army units :";
        std::cin >> targetTName;

        std::cout << "Number of army units left in the reinforcement pool: " << getNumArmies() << std::endl;
        std::cout << "Please enter the number of army units you want to deploy in " << targetTName << ": ";
        std::cin >> armies;
        
        ordersList->ordersVector.push_back(new DeployOrder(this, targetTName, armies));
    }
    else if(orderType == "advance")
    {
        std::cout << "Current list of territories to attack:\n";
        for(Territory* t : getToAttackTerritories())
        {
            std::cout << t->name << std::endl;
        }

        std::cout << "\nCurrent list of territories to defend:\n";
        for(Territory* t : getToDefendTerritories())
        {
            std::cout << t->name << std::endl;
        }
    
        std::cout << "\n\nChoose a source territory (army units should be on standby there): ";
        std::cin >> sourceTName;

        std::cout << "\nChoose a target territory to advance to: ";
        std::cin >> targetTName;

        std::cout << "\nEnter the number of army units to execute an advance order: ";
        std::cin >> armies;


        ordersList->ordersVector.push_back(new AdvanceOrder(this, sourceTName, targetTName, armies)); 
    }
    else if(orderType == "airlift")
    {
        std::cout << "\nChoose a source territory (army units should be on standby there): ";
        std::cin >> sourceTName;

        std::cout << "\nChoose a target territory to move to: ";
        std::cin >> targetTName;

        std::cout << "\nEnter the number of army units to execute an airlift order: ";
        std::cin >> armies;

        ordersList->ordersVector.push_back(new AirliftOrder()); // Need to have some logic that registers the details from the player's input
    }
    else if(orderType == "bomb")
    {
        std::cout << "\nChoose a target territory to execute a bomb order: ";
        std::cin >> targetTName;

        ordersList->ordersVector.push_back(new BombOrder()); // Need to have some logic that registers the details from the player's input
    }
    else if(orderType == "blockade")
    {
        std::cout << "\nChoose a target territory to execute a blockade order: ";
        std::cin >> targetTName;

        ordersList->ordersVector.push_back(new BlockadeOrder()); // Need to have some logic that registers the details from the player's input
    }
    else if(orderType == "negotiate")
    {
        std::string targetPName;
        std::cout << "\nChoose a target player to execute a negotiate order: ";
        std::cin >> targetPName;

        ordersList->ordersVector.push_back(new NegotiateOrder()); // Need to have some logic that registers the details from the player's input
    }
    else
    {
        std::cout << "Invalid Order Type! Please try again! \n\n";
    }
}
