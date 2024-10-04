#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <queue>

#include "MapFiles/Map.h"

/**
 * Removes leading and trailing whitespace from a given string.
 *
 * @param str The string to trim.
 * @return A new string with no leading or trailing whitespace.
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

/**
 * Copy constructor for the Territory class.
 * Creates a deep copy of the adjacentTerritories map, ensuring each pointer is properly duplicated.
 *
 * @param other The Territory object to copy from.
 */
Territory::Territory(const Territory& other) {
    numberOfArmies = other.numberOfArmies;
    x = other.x;
    y = other.y;

    for (const auto& pair : other.adjacentTerritories)
    {
        adjacentTerritories.insert({pair.first, pair.second}); // These pointers are updated by the Map copy constructor
    }
}

/**
* Copy assignment operator for the Territory class.
* Assigns values from another Territory object, making sure to release and reallocate memory for the adjacentTerritories map.
*
* @param other The Territory object to assign from.
* @return A reference to the current Territory object.
*/
Territory& Territory::operator=(const Territory& other) {
    if (this == &other) return *this;

    numberOfArmies = other.numberOfArmies;
    x = other.x;
    y = other.y;

    adjacentTerritories.clear();
    for (const auto& pair : other.adjacentTerritories)
    {
        adjacentTerritories.insert({pair.first, new Territory(*pair.second)});
    }

    return *this;
}

/**
 * Overloads the insertion operator to output a Territory's position and adjacent territories.
 *
 * @param out The output stream.
 * @param t The Territory object to print.
 * @return The output stream with the territory's data.
 */
std::ostream & operator << (std::ostream &out,  Territory &t) {
    out << " (Position: " << t.x << ", " << t.y << ")\n\t\t\t\tAdjacent Territories:\n";

    for (auto& territory : t.adjacentTerritories) {
        out << "\t\t\t\t\t" << territory.first << "\n";
    }

    return out;
}

/**
 * Copy constructor for the Continent class.
 * Creates a deep copy of the childTerritories map.
 *
 * @param other The Continent object to copy from.
 */
Continent::Continent(const Continent& other)
{
    bonusPoints = other.bonusPoints;

    for (const auto& pair : other.childTerritories)
    {
        childTerritories.insert({pair.first, pair.second}); // These pointers are updated by the Map copy constructor
    }
}

/**
     * Copy assignment operator for the Continent class.
     * Assigns values from another Continent object, ensuring proper memory management for the childTerritories map.
     *
     * @param other The Continent object to assign from.
     * @return A reference to the current Continent object.
     */
Continent& Continent::operator=(const Continent& other) {
    if (this == &other) return *this;

    bonusPoints = other.bonusPoints;

    // Deallocate existing child territories
    for (auto& pair : childTerritories) {
        delete pair.second;  // Release the memory for the territories in this continent
    }
    childTerritories.clear();

    for (const auto& pair : other.childTerritories) {
        childTerritories.insert({pair.first, new Territory(*pair.second)});
    }

    return *this;
}

/**
 * Overloads the insertion operator to output a Continent's bonus points and its territories.
 *
 * @param out The output stream.
 * @param c The Continent object to print.
 * @return The output stream with the continent's data.
 */
std::ostream & operator << (std::ostream &out,  Continent &c) {
    out << " (Bonus Points: " << c.bonusPoints << ")\n\t\tTerritories:\n";

    for (auto& territory : c.childTerritories) {
        out << "\t\t\t" << territory.first << "" << *territory.second;
    }

    return out;
}

/**
 * Validates the map according to the following rules:
 * 1) The map must be a connected graph.
 * 2) Each continent must be a connected subgraph.
 * 3) Each territory must belong to only one continent.
 *
 * @return true if the map is valid, false otherwise.
 */
bool Map::Validate() {
    // Step 3: Ensure each territory belongs to one and only one continent
    std::unordered_set<std::string> assignedTerritories;
    for (const auto& continentPair : continents) {
        for (const auto& territoryPair : continentPair.second->childTerritories) {
            if (assignedTerritories.find(territoryPair.first) != assignedTerritories.end()) {
                std::cerr << imageFilename << " | Map validation failed: Territory '" << territoryPair.first
                          << "' is assigned to multiple continents.\n";
                return false;
            }
            assignedTerritories.insert(territoryPair.first);
        }
    }

    // Step 2: Check if each continent is a connected subgraph
    for (const auto& continentPair : continents) {
        Continent* continent = continentPair.second;
        if (!IsConnectedGraph(continent->childTerritories, continent->childTerritories)) {
            std::cerr << imageFilename << " | Map validation failed: Continent '" << continentPair.first
                      << "' is not a connected subgraph.\n";
            return false;
        }
    }

    // Step 1: Check if the entire map is a connected graph
    if (!IsConnectedGraph(territories, territories)) {
        std::cerr << imageFilename << " | Map validation failed: The map is not a connected graph.\n";
        return false;
    }

    std::cout << imageFilename << " | Map validation successful.\n";
    return true;
}

/**
 * Copy constructor for the Map class.
 * Performs a deep copy of the continents and territories maps.
 *
 * @param other The Map object to copy from.
 */
Map::Map(const Map& other) {
    DeepCopyMapData(other);
}

/**
 * Copy assignment operator for the Map class.
 * Releases any allocated memory and assigns values from another Map object.
 *
 * @param other The Map object to assign from.
 * @return A reference to the current Map object.
 */
Map& Map::operator=(const Map& other) {
    if (this == &other) return *this;

    // Clear existing continents and territories
    for (auto& pair : continents) {
        delete pair.second;  // Delete old continents
    }
    continents.clear();

    for (auto& pair : territories) {
        delete pair.second;  // Delete old territories
    }
    territories.clear();

    DeepCopyMapData(other);

    return *this;
}

/**
 * Overloads the insertion operator to output the details of a Map, including its continents and territories.
 *
 * @param out The output stream.
 * @param m The Map object to print.
 * @return The output stream with the map's data.
 */
std::ostream & operator << (std::ostream &out,  Map &m) {
    out << "Map Image Filename: " << m.imageFilename << "\n" << "Continents:\n";

    for (auto& continent : m.continents) {
        out << "\t" << continent.first << "" << *continent.second;
    }

    return out;
}

/**
 * Destructor for the Map class. Frees all dynamically allocated memory for territories and continents.
 */
Map::~Map() {
    for (auto& continent : continents) {
        delete continent.second; // Delete all continent instances that were dynamically allocated.
    }
    continents.clear();

    for (auto& territory : territories) {
        delete territory.second; // Delete all territory instances that were dynamically allocated.
    }
    territories.clear();
}

/**
 * Checks if the given set of territories forms a connected graph.
 * This is used to validate if the map or a continent is fully connected.
 *
 * @param territories The set of territories to check.
 * @param validTerritories The set of territories that are considered valid for the connection check.
 * @return true if the territories form a connected graph, false otherwise.
 */
bool Map::IsConnectedGraph(std::map<std::string, Territory *>& territories, const std::map<std::string, Territory *>& validTerritories) {
    if (territories.empty()) return false;

    std::unordered_set<std::string> visited;
    std::queue<std::string> q;

    // Start with any territory (first in the map)
    auto it = territories.begin();
    q.push(it->first);
    visited.insert(it->first);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();
        Territory* currentTerritory = territories[current];

        // Visit all adjacent territories that are part of the valid set (e.g., within the same continent)
        for (const auto& adj : currentTerritory->adjacentTerritories) {
            if (validTerritories.find(adj.first) != validTerritories.end() &&
                visited.find(adj.first) == visited.end()) {
                visited.insert(adj.first);
                q.push(adj.first);
            }
        }
    }

    // If all territories were visited, the map is connected
    return visited.size() == territories.size();
}

/**
     * Helper function to perform deep copy of map data from another Map instance.
     * Used by both the copy constructor and assignment operator.
     *
     * @param other The Map object to copy from.
     */
void Map::DeepCopyMapData(const Map& other) {
    imageFilename = other.imageFilename;

    /*
     * In order to do a proper deep copy, new territory instances have to be made and properly assigned to each of the
     * adjacent and child territory maps.
     */

    // Temporary mapping of old to new territories to manage pointers
    std::map<Territory*, Territory*> territoryMapping;

    // Step 1: Deep copy territories and store them in a map for reference
    for (const auto& pair : other.territories) {
        auto newTerritory = new Territory(*pair.second);
        territories[pair.first] = newTerritory;
        territoryMapping[pair.second] = newTerritory;  // Map old to new territories
    }

    // Step 2: Deep copy continents, assigning the correct new territory pointers
    for (const auto& pair : other.continents) {
        auto newContinent = new Continent(*pair.second);
        continents[pair.first] = newContinent;

        // Reassign childTerritories in the new Continent to the new Territory pointers
        for (auto& childPair : newContinent->childTerritories) {
            childPair.second = territoryMapping[childPair.second];
        }
    }

    // Step 3: Update the adjacentTerritories in each new Territory to point to the new Territory instances using old to new map
    for (const auto& pair : territories) {
        Territory* territory = pair.second;
        for (auto& adjPair : territory->adjacentTerritories) {
            adjPair.second = territoryMapping[adjPair.second];
        }
    }
}

/**
 * Loads a map from a file and populates the Map object.
 *
 * @param sFileName The name of the file to load.
 * @param map The Map object to populate.
 */
void MapLoader::LoadMap(const std::string& sFileName, Map* map) {
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
            ParseMapMetaData(mapFile, map);
            ParseContinents(mapFile, map);
            ParseTerritories(mapFile, map);
        }
    }

    catch (const std::exception &e)
    {

        std::cerr << "Error: " << e.what() << std::endl;
    }
}

/**
 * Parses metadata from the map file.
 *
 * @param mapFile The input file stream.
 * @param map The Map object to populate with metadata.
 */
void MapLoader::ParseMapMetaData(std::ifstream& mapFile, Map* map) {
    mapFile.seekg(0, std::ios::beg); // Ensure file read pos is at start of file

    std::string line;
    bool inMapSection = false;

    while (!mapFile.eof() && mapFile.peek() != EOF) {
        std::getline(mapFile, line);

        if (line == "[Map]") {
            inMapSection = true;
            continue;
        }

        if (line.empty()) {
            continue;
        }

        if (line[0] == '[') {
            inMapSection = false;
            continue;
        }

        if (inMapSection) {
            // Parse the key-value pairs
            std::istringstream iss(line);
            std::string key, value;

            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                key = trim_white(key);
                value = trim_white(value);

                if (key == "image") {
                    map->imageFilename = value;
                }
            }
        }
    }
}

/**
 * Parses the continents section of a map file and adds the continents to the Map object.
 *
 * @param mapFile The input file stream containing the map data.
 * @param map The Map object to populate.
 */
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

            auto *newContinent = new Continent();
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

/**
 * Parses the territories section of a map file, creates Territory objects, and adds them to the Map.
 *
 * @param mapFile The input file stream containing the map data.
 * @param map The Map object to populate.
 */
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

            auto *newTerritory = new Territory();
            std::string territoryName;
            std::string parentContinent;

            std::string token;

            // Read the territory name
            std::getline(iss, token, ',');
            territoryName = token;
            newTerritory->name = territoryName;

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

/**
 * Populates the adjacentTerritories pointers for each Territory by linking the adjacent territories that were parsed.
 * This ensures that each territory's adjacentTerritories point to the correct Territory object.
 *
 * @param map The Map object containing the territories.
 */
void MapLoader::PopulateAdjacentTerritories(Map *map) {
    for (auto& territory : map->territories) {
        for (auto& adjacentTerritory : territory.second->adjacentTerritories) {
            std::string name = adjacentTerritory.first;
            adjacentTerritory.second = map->territories.at(name);
        }
    }
}
