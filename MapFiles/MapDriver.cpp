#include "MapFiles/MapDriver.h"

#include "MapFiles/Map.h"

int testLoadMaps()
{
    MapLoader mapLoader;

    Map map;

//    mapLoader.LoadMap("../SomeMapsFromOnline/Montreal/Grand Montreal.map", &map);
    mapLoader.LoadMap("../SomeMapsFromOnline/Aden/Aden.map", &map);

    std::cout << map;

    return 0;
};