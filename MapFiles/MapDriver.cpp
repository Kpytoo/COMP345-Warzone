#include "MapFiles/MapDriver.h"

#include "MapFiles/Map.h"

int testLoadMaps()
{
    MapLoader mapLoader;

    Map map;

    mapLoader.LoadMap("../SomeMapsFromOnline/Montreal/Grand Montreal.map", &map);

    std::cout << map.to_string();

    return 0;
};