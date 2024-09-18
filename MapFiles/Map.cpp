#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>

#include "MapFiles/Map.h"

/*
 * TODO: Add more proper error handling for parsing
 * TODO: Update driver code to test all maps including some invalid ones
 * TODO: Dump parsed data to files for examination
 * TODO: Add doc comments to every function
 * TODO: Limit number of adjacent territories to 10, total territories to 255 and continents to 32
 */

std::string trim_white(const std::string& str) {
    size_t start = 0;
    while (start < str.size() && std::isspace(str[start])) {
        ++start;
    }

    size_t end = str.size();
    while (end > start && std::isspace(str[end - 1])) {
        --end;
    }

    return str.substr(start, end - start);
}

std::ostream & operator << (std::ostream &out,  Territory &t) {
    out << "\t\t\t\tAdjacent Territories:\n";

    for (auto& territory : t.adjacentTerritories) {
        out << "\t\t\t\t\t" << territory.first << "\n";
    }

    return out;
}

Continent::Continent() {
}

std::ostream & operator << (std::ostream &out,  Continent &c) {
    out << " (Bonus Points: " << c.bonusPoints << ")\n\t\tTerritories:\n";

    for (auto& territory : c.childTerritories) {
        out << "\t\t\t" << territory.first << ":\n" << *territory.second;
    }

    return out;
}

bool Map::Validate() {
    // 1) the map is a connected graph,
    // 2) continents are connected subgraphs
    // 3) each country belongs to one and only one continent.

    // For number 1, ensure that every continent has a connection to another continent through an adjacent territory.
    // For number 2, validate that for every adjacent territory, they have the parent territory as a parent territory.
    // For number 3, it should already be handled by the parser as it will not process (TODO: this needs to be properly validated and checked in the parser) any lines defining a territory that shares a name with already loaded territory.



    return true;
}

std::ostream & operator << (std::ostream &out,  Map &m) {
    out << "Continents:\n";

    for (auto& continent : m.continents) {
        out << "\t" << continent.first << "" << *continent.second;
    }

    return out;
}

Map::~Map() {
    for (auto& continent : continents) {
        delete continent.second; // Delete all continent instances that were dynamically allocated.
    }

    for (auto& territory : territories) {
        delete territory.second; // Delete all territory instances that were dynamically allocated.
    }
}

void MapLoader::LoadMap(std::string sFileName, Map* map) {
    try
    {
        std::ifstream mapFile(sFileName);
        mapFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        if (!mapFile.is_open())
        {
            throw std::runtime_error("File could not be opened: " + sFileName);
        }
        else
        {
            ParseContinents(mapFile, map);
            ParseTerritories(mapFile, map);
        }
    }

    catch (const std::exception &e)
    {

        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void MapLoader::ParseContinents(std::ifstream &mapFile, Map *map) {
    mapFile.seekg(0, std::ios::beg);

    std::string line;
    bool inContinentsSection = false;

    while (!mapFile.eof() && mapFile.peek() != EOF) {
        std::getline(mapFile, line);

        if (line == "[Continents]") {
            inContinentsSection = true;
            continue;
        }

        if (line.empty()) {
            continue;
        }

        if (line[0] == '[') {
            inContinentsSection = false;
            continue;
        }

        if (inContinentsSection) {
            std::istringstream iss(line);

            Continent *newContinent = new Continent();
            std::string continentName;

            std::string token;

            // Read the continent name
            std::getline(iss, token, '=');
            continentName = token;

            // Read the points for owning the continent
            std::getline(iss, token, ',');
            newContinent->bonusPoints = std::stoi(token);

            map->continents.insert({continentName, newContinent});
        }
    }
}

void MapLoader::ParseTerritories(std::ifstream &mapFile, Map *map) {
    mapFile.seekg(0, std::ios::beg);

    std::string line;
    bool inTerritoriesSection = false;

    while (!mapFile.eof() && mapFile.peek() != EOF) {
        std::getline(mapFile, line);

        if (line == "[Territories]") {
            inTerritoriesSection = true;
            continue;
        }

        if (line.empty()) {
            continue;
        }

        if (line[0] == '[') {
            inTerritoriesSection = false;
            continue;
        }

        if (inTerritoriesSection) {
            std::istringstream iss(line);

            Territory *newTerritory = new Territory();
            std::string territoryName;
            std::string parentContinent;

            std::string token;

            // Read the territory name
            std::getline(iss, token, ',');
            territoryName = token;

            // Read the coordinates
            std::getline(iss, token, ',');
            newTerritory->x = std::stoi(token);

            std::getline(iss, token, ',');
            newTerritory->y = std::stoi(token);

            // Read the parent Continent
            std::getline(iss, token, ',');
            parentContinent = trim_white(token);

            // Read the adjacent territories
            while (std::getline(iss, token, ',')) {
                token.erase(token.begin(), std::find_if(token.begin(), token.end(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }));
                newTerritory->adjacentTerritories.insert({token, NULL}); // Set null for now, only attempt to update pointers once all territories are parsed
            }

            map->continents.at(parentContinent)->childTerritories.insert({territoryName, newTerritory});
            map->territories.insert({territoryName, newTerritory});
        }
    }

    PopulateAdjacentTerritories(map);
}

void MapLoader::PopulateAdjacentTerritories(Map *map) {
    for (auto& territory : map->territories) {
        for (auto& adjacentTerritory : territory.second->adjacentTerritories) {
            std::string name = adjacentTerritory.first;
            adjacentTerritory.second = map->territories.at(name);
        }
    }
}
