#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <string>

class Continent
{
public:
    const int bonusPoints;
    Player *owner;
    std::vector<Territory *> territories; // Territories that belong to this continent
};

class Territory
{

public:
    std::string name;
    int numberOfArmies;
    Player *owner;        // need Player object
    Continent *continent; // Maybe do continent class? If not just an integer to associate territory to continent
    std::vector<Territory *> adjacentTerritories;
};

class Map
{

public:
};

#endif