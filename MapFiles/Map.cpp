#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>

#include "MapFiles/Map.h"

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

std::string Territory::to_string() {
    std::ostringstream ss;
    ss << "\t\t\t\tAdjacent Territories:\n";

    for (auto& territory : adjacentTerritories) {
        ss << "\t\t\t\t\t" << territory.first << "\n";
    }

    return ss.str();
}

Continent::Continent() {
}

std::string Continent::to_string() {
    std::ostringstream ss;
    ss << "\t\tTerritories:\n";

    for (auto& territory : childTerritories) {
        ss << "\t\t\t" << territory.first << ":\n" << territory.second->to_string();
    }

    return ss.str();
}

std::string Map::to_string() {
    std::ostringstream ss;
    ss << "Continents:\n";

    for (auto& continent : continents) {
        ss << "\t" << continent.first << ":\n" << continent.second->to_string();
    }

    return ss.str();
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
            mapFile.seekg(0, std::ios::beg);
            ParseTerritories(mapFile, map);
        }
    }

    catch (const std::exception &e)
    {

        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void MapLoader::ParseContinents(std::ifstream &mapFile, Map *map) {

    std::string line;
    bool inContinentsSection = false;

    while (mapFile.peek()!=EOF) {
        std::getline(mapFile, line);

        if (line == "[Continents]") {
            inContinentsSection = true;
            continue;
        }

        // Exit the section if it's empty or we encounter another section
        if (line.empty() || line[0] == '[') {
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
    std::string line;
    bool inTerritoriesSection = false;

    while (mapFile.peek()!=EOF) {
        std::getline(mapFile, line);

        if (line == "[Territories]") {
            inTerritoriesSection = true;
            continue;
        }

        // Exit the section if it's empty or we encounter another section
        if (line.empty()) {
            if (line[0] == '[') {
                inTerritoriesSection = false;
            }
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

            // Read the neighbors
            while (std::getline(iss, token, ',')) {
                token.erase(token.begin(), std::find_if(token.begin(), token.end(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }));
                newTerritory->adjacentTerritories.insert({token, NULL});
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
