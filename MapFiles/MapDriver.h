//
// Created by william on 17/09/24.
//

#ifndef COMP345_WARZONE_MAPDRIVER_H
#define COMP345_WARZONE_MAPDRIVER_H

#include <string>

/**
 * Array of file paths for the map files used in the testLoadMaps function.
 *
 * TODO: add some maps that cause validation errors due to improper graphing and/or parsing errors
 */
const std::string MAP_FILES[7] = {
        "../SomeMapsFromOnline/3D/3D.map",
        "../SomeMapsFromOnline/Montreal/Grand Montreal.map",
        "../SomeMapsFromOnline/Aden/Aden.map",
        "../SomeMapsFromOnline/RomanEmpire/Roman Empire.map",

        "../BadMapsForTesting/continent_disconnected.map",
        "../BadMapsForTesting/map_disconnected.map",
        "../BadMapsForTesting/multiple_continents_assigned_to_territory.map"
};

int testLoadMaps();

#endif //COMP345_WARZONE_MAPDRIVER_H
