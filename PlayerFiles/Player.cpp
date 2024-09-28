#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include "PlayerFiles/Player.h"

// default constructor
Player::Player() : playerName(""), playerHand(nullptr), ordersList(nullptr), numArmies(0)
{
    playerHand = new Hand();
    ordersList = new OrdersList();
}

// parameterized constructor
Player::Player(std::string playerName) : playerName(playerName), playerHand(new Hand()), ordersList(new OrdersList()), numArmies(0)
{
}

// destructor
Player::~Player()
{
    delete playerHand;
    delete ordersList;
}

// copy constructor

Player::Player(const Player &other)
{
    playerName = other.playerName;
    numArmies = other.numArmies;

    playerHand = new Hand(*other.playerHand);
    ordersList = new OrdersList(*other.ordersList);

    OwnedTerritories = other.OwnedTerritories;
}

// Assignment operator

Player &Player::operator=(const Player &other)
{
    if (this == &other)
        return *this;

    playerName = other.playerName;
    numArmies = other.numArmies;

    // Clean up old resources
    delete playerHand;
    delete ordersList;

    // Deep copy
    playerHand = new Hand(*other.playerHand);
    ordersList = new OrdersList(*other.ordersList);

    // Deep copy of OwnedTerritories
    OwnedTerritories = other.OwnedTerritories;

    return *this;
}

// Stream insertion operator overload
std::ostream &operator<<(std::ostream &os, const Player &obj)
{
    os << "Player Name: " << obj.playerName << std::endl;
    os << "Number of Armies: " << obj.numArmies << std::endl;
    os << "Owned Territories: ";
    for (auto &territory : obj.OwnedTerritories)
    {
        os << territory->getName() << " ";
    }
    os << std::endl;

    os << "Player Hand: " << *(obj.playerHand) << std::endl;
    os << "Orders List: " << *(obj.ordersList) << std::endl;

    return os;
}

// Getter definitions
std::string Player::getPlayerName() const
{
    return playerName;
}

std::vector<Territory *> Player::getOwnedTerritories() const
{
    return OwnedTerritories;
}

Hand *Player::getPlayerHand() const
{
    return playerHand;
}

OrdersList *Player::getOrdersList() const
{
    return ordersList;
}

int Player::getNumArmies() const
{
    return numArmies;
}

// Setter definitions
void Player::setPlayerName(const std::string &name)
{
    playerName = name;
}

void Player::setOwnedTerritories(const std::vector<Territory *> &territories)
{
    OwnedTerritories = territories;
}

void Player::setPlayerHand(Hand *hand)
{
    playerHand = hand;
}

void Player::setOrdersList(OrdersList *ordersList)
{
    this->ordersList = ordersList;
}

void Player::setNumArmies(int numArmies)
{
    this->numArmies = numArmies;
}

// toDefend method (returns an arbitrary list of territories to defend)
std::vector<Territory *> Player::toDefend()
{
    // For now, just return the owned territories, idk what its suppose to return...
    return OwnedTerritories;
}

// toAttack method (returns an arbitrary list of territories to attack)
std::vector<Territory *> Player::toAttack()
{
    // For now, just return the owned territories, idk what its suppose to return...
    return OwnedTerritories;
}

// issueOrder method (creates and adds an order to the player's orders list)
void Player::issueOrder()
{
    Order *newOrder = new Order();
    ordersList->addOrder(newOrder); // need to create addorder ethod
}
