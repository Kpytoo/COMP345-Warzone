#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include "MapFiles/Map.h"
#include "CardsFiles/Cards.h"
#include "OrdersFiles/Orders.h"
#include "PlayerFiles/PlayerStrategies.h"

class Player
{
private:
    std::string playerName;                       // Player's name
    std::vector<Territory *> OwnedTerritories;    // List of territories owned by the player
    std::vector<Territory *> toDefendTerritories; // Pointer list to OwnedTerritories elements to defend
    std::vector<Territory *> toAttackTerritories; // List of territories the player wants to attack
    Hand *playerHand;                             // Pointer to the player's hand of cards
    OrdersList *ordersList;                       // Pointer to the player's orders list
    int numArmies;                                // Number of armies player controls
    PlayerStrategy *strategy;

public:
    // Constructors, assignment operator, and destructor
    Player();
    Player(std::string name);
    Player(std::string playerName, const std::vector<Territory *> &ownedTerritories);

    Player(const Player &other);
    Player &operator=(const Player &other);
    friend std::ostream &operator<<(std::ostream &os, const Player &obj);
    ~Player();

    // Getters for accessing private member variables
    std::string getPlayerName() const;
    std::vector<Territory *> &getOwnedTerritories();
    std::vector<Territory *> &getToDefendTerritories();
    std::vector<Territory *> &getToAttackTerritories();
    Hand *getPlayerHand();
    OrdersList *getOrdersList() const;
    int getNumArmies() const;
    PlayerStrategy *getStrategy() const;

    // Setters for modifying private member variables
    void setPlayerName(const std::string &name);
    void setOwnedTerritories(const std::vector<Territory *> &territories);
    void setToAttackTerritories(const std::vector<Territory *> &territories);
    void setToDefendTerritories(const std::vector<Territory *> &territories);
    void setPlayerHand(Hand *hand);
    void setOrdersList(OrdersList *ordersList);
    void setNumArmies(int numArmies);
    void setStrategy(PlayerStrategy *newStrategy);

    // Core gameplay methods
    std::vector<Territory *> toDefend(); // Returns territories the player should defend
    std::vector<Territory *> toAttack(); // Returns territories the player may attack
    Player *FindTerritoryOwner(const std::string &territoryName);
    void issueOrder(std::string orderType, Deck *deck); // Issues an order for the player

    Deck *deck = nullptr; // Pointer to the deck for player to draw from

    static std::vector<Player *> players; // Pointer to player that would be stored in GameEngine, used for finding owners of enemy territories.

    std::vector<Player *> *getPlayers();
    int reinforcement_units = 0;
};

#endif
