#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include "MapFiles/Map.h"
#include "CardsFiles/Cards.h"
#include "/OrdersList.h"       // Placeholder for OrdersList, to be included when implemented


class Player
{
private:
    std::string playerName;               // Player's name
    std::vector<Territory *> OwnedTerritories; // List of territories owned by the player
    Hand *playerHand;                     // Pointer to the player's hand of cards
    OrdersList *ordersList;               // Pointer to the player's orders list
    int numArmies;                        // Number of armies player controls

public:
    // Constructors, assignment operator, and destructor
    Player();
    Player(std::string playerName);
    Player(const Player &other);
    Player &operator=(const Player &other);
    friend std::ostream &operator<<(std::ostream &os, const Player &obj);
    ~Player();

    // Getters for accessing private member variables
    std::string getPlayerName() const;
    std::vector<Territory *> getOwnedTerritories() const;
    Hand *getPlayerHand() const;
    OrdersList *getOrdersList() const;
    int getNumArmies() const;

    // Setters for modifying private member variables
    void setPlayerName(const std::string &name);
    void setOwnedTerritories(const std::vector<Territory *> &territories);
    void setPlayerHand(Hand *hand);
    void setOrdersList(OrdersList *ordersList);
    void setNumArmies(int numArmies);

    // Core gameplay methods
    std::vector<Territory *> toDefend(); // Returns territories the player should defend
    std::vector<Territory *> toAttack(); // Returns territories the player may attack
    void issueOrder();                   // Issues an order for the player
};

#endif
