#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <set>

#include "PlayerFiles/Player.h"
#include "CardsFiles/Cards.h"
#include "OrdersFiles/Orders.h"
#include "MapFiles/Map.h"

std::vector<Player *> Player::players;
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
    delete ordersList;
    delete strategy;
}

// Copy constructor: creates a deep copy of another Player object
Player::Player(const Player &other)
{
    playerName = other.playerName;
    numArmies = other.numArmies;

    // Deep copy for playerHand and ordersList
    playerHand = new Hand(*other.playerHand);
    ordersList = new OrdersList(*other.ordersList);

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
void Player::setStrategy(PlayerStrategy *newStrategy)
{

    strategy = newStrategy;
}

// Get the current strategy of the player
PlayerStrategy *Player::getStrategy() const
{
    return strategy;
}
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
    return strategy->toDefend(this);
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
    return strategy->toAttack(this);
}

Player *Player::FindTerritoryOwner(const std::string &territoryName)
{
    std::cout << "Finding owner for territory: " << territoryName << std::endl;

    for (Player *player : Player::players)
    {
        std::cout << player->playerName;
    }

    for (Player *player : Player::players) // Access the static Player::players vector
    {
        if (!player)
        {
            std::cerr << "Warning: Found a null player in the list!" << std::endl;
            continue;
        }

        // Check each territory owned by the player
        for (Territory *territory : player->getOwnedTerritories())
        {
            if (territory && territory->name == territoryName)
            {
                std::cout << "Owner found: " << player->getPlayerName() << " for territory " << territoryName << std::endl;
                return player;
            }
        }
    }

    std::cerr << "No owner found for territory: " << territoryName << std::endl;
    return nullptr; // Territory not found or no owner
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
    strategy->issueOrder(this, orderType, deck);
}
