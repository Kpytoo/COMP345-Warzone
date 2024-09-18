#include <fstream>
#include <iostream>

#include "MapFiles/MapDriver.h"

#include "MapFiles/Map.h"

/**
 * @brief Tests loading multiple map files, validates them, and outputs their graph representation into text files.
 *
 * This function iterates over the map file names stored in `MAP_FILES`, loads each map using the `MapLoader::LoadMap`
 * function, validates it using `Map::Validate`, and then writes the result to a new output file.
 *
 * For each map, an output file is created by appending "-GRAPH.out" to the map file name.
 * If an output file cannot be opened, an error message is printed to the standard error stream (`std::cerr`) and the function returns -1.
 *
 * @return 0 if all maps are processed successfully, or -1 if an error occurs.
 */
int testLoadMaps()
{
    for (auto& mapFile : MAP_FILES) {
        Map map;

        MapLoader::LoadMap(mapFile, &map);

        map.Validate();

        std::string outputFileName = mapFile + "-GRAPH.out";  // Append ".out" to the map file name

        std::ofstream outFile(outputFileName);

        if (!outFile.is_open()) {
            std::cerr << "Error: Could not open output file: " << outputFileName << "\n";
            return -1;
        }

        outFile << map;

        outFile.close();
    }

    return 0;
}