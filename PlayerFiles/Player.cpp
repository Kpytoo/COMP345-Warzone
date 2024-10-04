#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include "PlayerFiles/Player.h"
#include "CardsFiles/Cards.h"
#include "OrdersFiles/Orders.h"
#include "MapFiles/Map.h"

// Default constructor: initializes player with empty name, zero armies, and new Hand and OrdersList instances
Player::Player() : playerName(""), playerHand(nullptr), ordersList(nullptr), numArmies(0)
{
    playerHand = new Hand();
    ordersList = new OrdersList();
}

// Parameterized constructor: initializes player with a given name and new Hand and OrdersList instances
Player::Player(std::string playerName) : playerName(playerName), playerHand(new Hand()), ordersList(new OrdersList()), numArmies(0)
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
//    playerHand = new Hand(*other.playerHand);
//    ordersList = new OrdersList(*other.ordersList);
    playerHand = new Hand();
    ordersList = new OrdersList();

    // Shallow copy of owned territories (assuming pointer structure is shared)
    OwnedTerritories = other.OwnedTerritories;
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
//    playerHand = new Hand(*other.playerHand);
//    ordersList = new OrdersList(*other.ordersList);
    playerHand = new Hand();
    ordersList = new OrdersList();

    // Shallow copy of owned territories
    OwnedTerritories = other.OwnedTerritories;

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

    // Outputs player's hand and orders list (assuming operator<< is defined for Hand and OrdersList)
//    os << "Player Hand: " << *(obj.playerHand) << std::endl; // TODO: uncomment this and the ones in the copy functions
//    os << "Orders List: " << *(obj.ordersList) << std::endl;

    return os;
}

// Getter definitions: retrieve player details
std::string Player::getPlayerName() const { return playerName; }
std::vector<Territory *> Player::getOwnedTerritories() const { return OwnedTerritories; }
Hand *Player::getPlayerHand() const { return playerHand; }
OrdersList *Player::getOrdersList() const { return ordersList; }
int Player::getNumArmies() const { return numArmies; }

// Setter definitions: modify player details
void Player::setPlayerName(const std::string &name) { playerName = name; }
void Player::setOwnedTerritories(const std::vector<Territory *> &territories) { OwnedTerritories = territories; }
void Player::setPlayerHand(Hand *hand) { playerHand = hand; }
void Player::setOrdersList(OrdersList *ordersList) { this->ordersList = ordersList; }
void Player::setNumArmies(int numArmies) { this->numArmies = numArmies; }

// toDefend: returns a list of territories to defend (placeholder behavior returning owned territories)
std::vector<Territory *> Player::toDefend()
{
    return OwnedTerritories;
}

// toAttack: returns a list of territories to attack (placeholder behavior returning owned territories)
std::vector<Territory *> Player::toAttack()
{
    return OwnedTerritories;
}

// issueOrder: creates a new order and adds it to the player's orders list
void Player::issueOrder()
{
    ordersList->ordersVector.push_back(new AdvanceOrder()); // Use advanced order for now
}
