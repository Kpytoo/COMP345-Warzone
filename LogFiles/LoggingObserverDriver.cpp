//
// Created by william on 06/11/24.
//

#include "LoggingObserverDriver.h"
#include "LoggingObserver.h"
#include "OrdersFiles/Orders.h"
#include "GameEngineFiles/CommandProcessing.h"
#include "GameEngineFiles/GameEngine.h"

void testLoggingObserver() {
    LogObserver logObserver;

    // Logging in command processing
    FileCommandProcessorAdapter cp("../GameEngineFiles/Commands.txt");
    cp.attach(&logObserver);

    Command command = cp.getCommand(); // Just get first load command from commands file
    command.attach(&logObserver);

    command.saveEffect("Map was successively loaded!");

    // Logging in game engine
    GameEngine engine;
    Map gameMap;
    MapLoader::LoadMap("../SomeMapsFromOnline/Montreal/Grand Montreal.map", &gameMap);
    engine.attach(&logObserver);

    engine.manageCommand(command); // This should cause a transition from start to map loaded state

    // Logging in order handling
    Player player;
    player.setNumArmies(20);
    player.getOwnedTerritories().push_back(gameMap.territories.at("Ahuntsic"));
    OrdersList orderList;
    orderList.attach(&logObserver);

    auto* deployOrder = new DeployOrder(&player, "Ahuntsic", 10);
    deployOrder->player = &player;
    deployOrder->attach(&logObserver);

    orderList.add(deployOrder);
    deployOrder->execute();
}
