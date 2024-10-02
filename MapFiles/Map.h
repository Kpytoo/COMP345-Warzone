#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <map>
#include <string>

/**
 * The Territory class represents a territory in the game, containing information such as its name,
 * number of armies, coordinates, and adjacent territories.
 */
class Territory
{
public:
    int numberOfArmies; ///< The number of armies present in this territory.
    int x, y; ///< The x and y coordinates of the center of the territory.

    /**
     * Default constructor
     */
    Territory() : numberOfArmies(0), x(0), y(0) {}

    std::map<std::string, Territory *> adjacentTerritories;  ///< A map of adjacent territories.

    /**
     * Copy constructor for the Territory class.
     * Creates a deep copy of the adjacentTerritories map, ensuring each pointer is properly duplicated.
     *
     * @param other The Territory object to copy from.
     */
    Territory(const Territory& other);

    /**
    * Copy assignment operator for the Territory class.
    * Assigns values from another Territory object, making sure to release and reallocate memory for the adjacentTerritories map.
    *
    * @param other The Territory object to assign from.
    * @return A reference to the current Territory object.
    */
    Territory& operator=(const Territory& other) {
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
     * Overloads the insertion operator to print details of a Territory, including its adjacent territories.
     *
     * @param out The output stream.
     * @param t The Territory to output.
     * @return The output stream with the territory's details.
     */
    friend std::ostream & operator << (std::ostream &out,  Territory &t);
};

/**
 * The Continent class represents a continent in the game, which holds multiple territories
 * and a bonus for owning all of them.
 */
class Continent
{
public:
    /**
     * Default constructor for the Continent class.
     * Initializes a new Continent object.
     */
    Continent() : bonusPoints(0) {}

    int bonusPoints; ///< The bonus points awarded for controlling the continent.

    std::map<std::string, Territory *> childTerritories; ///< A map of territories that belong to this continent.

    /**
     * Copy constructor for the Continent class.
     * Creates a deep copy of the childTerritories map.
     *
     * @param other The Continent object to copy from.
     */
    Continent(const Continent& other);

    /**
     * Copy assignment operator for the Continent class.
     * Assigns values from another Continent object, ensuring proper memory management for the childTerritories map.
     *
     * @param other The Continent object to assign from.
     * @return A reference to the current Continent object.
     */
    Continent& operator=(const Continent& other) {
        if (this == &other) return *this;

        bonusPoints = other.bonusPoints;

        childTerritories.clear();

        for (const auto& pair : other.childTerritories)
        {
            childTerritories.insert({pair.first, new Territory(*pair.second)});
        }

        return *this;
    }

    /**
     * Overloads the insertion operator to print the details of a Continent, including bonus points
     * and its territories.
     *
     * @param out The output stream.
     * @param c The Continent to output.
     * @return The output stream with the continent's details.
     */
    friend std::ostream & operator << (std::ostream &out,  Continent &c);
};

/**
 * The Map class represents the entire game map, containing multiple continents and territories.
 * It provides validation functions to ensure the map's integrity.
 */
class Map
{
public:
    std::string imageFilename; ///< Name of filename for map graphical bitmap (useful if SFML is added later for GUI implementation)

    std::map<std::string, Continent *> continents; ///< A map of the continents on the map.
    std::map<std::string, Territory *> territories; ///< A map of all territories on the map.

    /**
     * Default constructor
     */
    Map() {}

    /**
     * Validates the map by checking if:
     * 1) The map is a connected graph.
     * 2) Each continent is a connected subgraph.
     * 3) Each territory belongs to exactly one continent.
     *
     * @return true if the map is valid, false otherwise.
     */
    bool Validate();

    /**
     * Copy constructor for the Map class.
     * Performs a deep copy of the continents and territories maps.
     *
     * @param other The Map object to copy from.
     */
    Map(const Map& other);

    /**
     * Copy assignment operator for the Map class.
     * Releases any allocated memory and assigns values from another Map object.
     *
     * @param other The Map object to assign from.
     * @return A reference to the current Map object.
     */
    Map& operator=(const Map& other) {
        if (this == &other) return *this;

        imageFilename = other.imageFilename;

        // Clear continents and territories
        for (auto& pair : continents)
        {
            delete pair.second;
        }
        continents.clear();

        for (auto& pair : territories)
        {
            delete pair.second;
        }
        territories.clear();

        // Deep copy
        for (const auto& pair : other.continents)
        {
            continents.insert({pair.first, new Continent(*pair.second)});
        }

        for (const auto& pair : other.territories)
        {
            territories.insert({pair.first, new Territory(*pair.second)});
        }

        return *this;
    }

    /**
     * Overloads the insertion operator to print the details of the Map, including its continents and territories.
     *
     * @param out The output stream.
     * @param m The Map to output.
     * @return The output stream with the map's details.
     */
    friend std::ostream & operator << (std::ostream &out,  Map &m);

    /**
     * Destructor for the Map class. Frees all dynamically allocated memory for territories and continents.
     */
    virtual ~Map();

private:
    /**
     * Checks if the given set of territories forms a connected graph.
     * This is used to validate whether the map or a continent is fully connected.
     *
     * @param territories The set of territories to check.
     * @param validTerritories The set of valid territories to consider in the connection check.
     * @return true if the territories form a connected graph, false otherwise.
     */
    static bool IsConnectedGraph(std::map<std::string, Territory *>& territories, const std::map<std::string, Territory *>& validTerritories);
};

/**
 * The MapLoader class handles loading and parsing a map file into a Map object.
 */
class MapLoader
{
public:
    /**
     * Loads a map from the specified file and populates the provided Map object.
     *
     * @param sFileName The name of the map file to load.
     * @param map The Map object to populate.
     */
    static void LoadMap(const std::string& sFileName, Map* map);

private:
    /**
     * Parses metadata from the map file.
     *
     * @param mapFile The input file stream.
     * @param map The Map object to populate with metadata.
     */
    static void ParseMapMetaData(std::ifstream& mapFile, Map* map);

    /**
     * Parses the continents section from the map file and populates the Map object with continents.
     *
     * @param mapFile The input file stream.
     * @param map The Map object to populate with continents.
     */
    static void ParseContinents(std::ifstream& mapFile, Map* map);

    /**
     * Parses the territories section from the map file and populates the Map object with territories.
     *
     * @param mapFile The input file stream.
     * @param map The Map object to populate with territories.
     */
    static void ParseTerritories(std::ifstream& mapFile, Map* map);

    /**
     * Populates the adjacentTerritories pointers for each territory, linking adjacent territories that were parsed.
     *
     * @param map The Map object containing the territories.
     */
    static void PopulateAdjacentTerritories(Map* map);
};

#endif