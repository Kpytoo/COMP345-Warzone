#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include "MapFiles/Map.h"

class Player
{

public:
    std::vector<Territory *> OwnedTerritories;
    std::vector<Hand *> PlayerHand; // Need to implement deck and hand

    std::vector<Territory *> toDefend();
    std::vector<Territory *> toAttack();
};

#endif
