//
// Created by william on 06/11/24.
//

#include "LoggingObserverDriver.h"
#include "LoggingObserver.h"
#include "OrdersFiles/Orders.h"

void testLoggingObserver() {
    LogObserver logObserver;

//    CommandProcessor cp;
//    cp.attach(&logObserver);
//
//    cp.readCommand("loadmap example_map");
//    cp.readCommand("addplayer Alice");
//
//    Command cmd("quit");
//    cmd.saveEffect("Game ended");

    OrdersList orderList;
    orderList.attach(&logObserver);

    Order* deployOrder = new DeployOrder();
    deployOrder->attach(&logObserver);

    orderList.add(deployOrder);
    deployOrder->execute();
}
