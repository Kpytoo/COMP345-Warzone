#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include "MapFiles/Map.h"

class Player
{

private:
    std::string playerName;
    std::vector<Territory *> OwnedTerritories;
    Hand *playerHand;       // Need to implement deck and hand
    OrdersList *ordersList; // orderList object need to be implemented
    int numArmies;          // idk if necessary?

public:
    Player();
    Player(std::string playerName);
    Player(const Player &other);
    Player &operator=(const Player &other);
    friend std::ostream &operator<<(std::ostream &os, const Player &obj);
    ~Player();

    // Getters
    std::string getPlayerName() const;
    std::vector<Territory *> getOwnedTerritories() const;
    Hand *getPlayerHand() const;
    OrdersList *getOrdersList() const;
    int getNumArmies() const;

    // Setters
    void setPlayerName(const std::string &name);
    void setOwnedTerritories(const std::vector<Territory *> &territories);
    void setPlayerHand(Hand *hand);
    void setOrdersList(OrdersList *ordersList);
    void setNumArmies(int numArmies);

    std::vector<Territory *> toDefend();
    std::vector<Territory *> toAttack();
    void issueOrder();
};

#endif
