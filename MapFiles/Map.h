#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <map>
#include <string>

class Territory
{
public:
    std::string name;
    int numberOfArmies;
    int x, y;
//    Player *owner; // need Player object
    std::map<std::string, Territory *> adjacentTerritories;

    std::string to_string();
};

class Continent
{
public:
    Continent();

    int bonusPoints;
//    Player *owner;
    std::map<std::string, Territory *> childTerritories; // Territories that belong to this continent

    std::string to_string();
};

class Map
{
public:
    std::map<std::string, Continent *> continents;
    std::map<std::string, Territory *> territories;

    std::string to_string();
};

class MapLoader
{
public:
    void LoadMap(std::string sFileName, Map* map);

private:
    bool ParseMapMetaData(std::ifstream& mapFile, Map* map);

    void ParseContinents(std::ifstream& mapFile, Map* map);

    void ParseTerritories(std::ifstream& mapFile, Map* map);

    void PopulateAdjacentTerritories(Map* map);
};

#endif