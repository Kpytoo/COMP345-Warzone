#include "OrdersFiles/OrdersDriver.h"
#include "OrdersFiles/Orders.h"
#include "PlayerFiles/Player.h"
#include "GameEngineFiles/GameEngine.h"
#include <iostream>
#include <vector>
#include <string>

/* void testOrdersLists() {
    OrdersList ordersList;

    // Create various orders
    Order* deployOrder = new DeployOrder();
    Order* advanceOrder = new AdvanceOrder();
    Order* bombOrder = new BombOrder();
    Order* blockadeOrder = new BlockadeOrder();
    Order* airliftOrder = new AirliftOrder();
    Order* negotiateOrder = new NegotiateOrder();

    // Add orders to the list
    ordersList.add(deployOrder);
    ordersList.add(advanceOrder);
    ordersList.add(bombOrder);
    ordersList.add(blockadeOrder);
    ordersList.add(airliftOrder);
    ordersList.add(negotiateOrder);

    // Display the list
    std::cout << "Initial Orders List:" << std::endl;
    std::cout << ordersList << std::endl;

    // Move the last order (position 6) to the first position
    ordersList.move(6, 1);
    std::cout << "After moving order 6 to position 1:" << std::endl;
    std::cout << ordersList << std::endl;

    // Move the second order to the last position
    ordersList.move(2, 6);
    std::cout << "After moving order 2 to position 6:" << std::endl;
    std::cout << ordersList << std::endl;

    // Remove the order at position 3
    ordersList.remove(3);
    std::cout << "After removing order at position 3:" << std::endl;
    std::cout << ordersList << std::endl;

    // Remove the first order
    ordersList.remove(1);
    std::cout << "After removing order at position 1:" << std::endl;
    std::cout << ordersList << std::endl;
} */

#include "OrdersFiles/OrdersDriver.h"
#include "OrdersFiles/Orders.h"
#include <iostream>
#include "PlayerFiles/Player.h"
#include "GameEngineFiles/GameEngine.h"
#include "Orders.h"
#include "CardsFiles/Cards.h"
#include "PlayerFiles/Player.h"
#include "MapFiles/Map.h"
/* void testOrdersLists() {
    OrdersList ordersList;

    // Create various orders
    Order* deployOrder = new DeployOrder();
    Order* advanceOrder = new AdvanceOrder();
    Order* bombOrder = new BombOrder();
    Order* blockadeOrder = new BlockadeOrder();
    Order* airliftOrder = new AirliftOrder();
    Order* negotiateOrder = new NegotiateOrder();

    // Add orders to the list
    ordersList.add(deployOrder);
    ordersList.add(advanceOrder);
    ordersList.add(bombOrder);
    ordersList.add(blockadeOrder);
    ordersList.add(airliftOrder);
    ordersList.add(negotiateOrder);

    // Display the list
    std::cout << "Initial Orders List:" << std::endl;
    std::cout << ordersList << std::endl;

    // Move the last order (position 6) to the first position
    ordersList.move(6, 1);
    std::cout << "After moving order 6 to position 1:" << std::endl;
    std::cout << ordersList << std::endl;

    // Move the second order to the last position
    ordersList.move(2, 6);
    std::cout << "After moving order 2 to position 6:" << std::endl;
    std::cout << ordersList << std::endl;

    // Remove the order at position 3
    ordersList.remove(3);
    std::cout << "After removing order at position 3:" << std::endl;
    std::cout << ordersList << std::endl;

    // Remove the first order
    ordersList.remove(1);
    std::cout << "After removing order at position 1:" << std::endl;
    std::cout << ordersList << std::endl;
} */

// Test function to demonstrate order validation and execution

Player neutralPlayer("Neutral");

void testOrderExecution()
{
    // Setup game engine and map
    GameEngine gameEngine;
    Map *gameMap = new Map();
    MapLoader mapLoader;

    // Load the map from file
    mapLoader.LoadMap("sample_map.txt", gameMap); // Replace "sample_map.txt" with your actual map file name
    gameEngine.setCurrentMap(gameMap);

    // Create players
    Player player1("Player1");
    Player player2("Player2");

    // Assign territories to players based on map data
    std::vector<Territory *> player1Territories;
    std::vector<Territory *> player2Territories;

    if (gameMap->territories.count("TerritoryA"))
    {
        player1Territories.push_back(gameMap->territories["TerritoryA"]);
    }
    if (gameMap->territories.count("TerritoryB"))
    {
        player1Territories.push_back(gameMap->territories["TerritoryB"]);
    }
    if (gameMap->territories.count("TerritoryC"))
    {
        player2Territories.push_back(gameMap->territories["TerritoryC"]);
    }

    // Set each player's owned territories
    player1.setOwnedTerritories(player1Territories);
    player2.setOwnedTerritories(player2Territories);

    // Set players' reinforcement pools
    player1.setNumArmies(10);
    player2.setNumArmies(10);

    // Set up deck and hands for players
    Deck deck;
    Hand hand1, hand2;
    gameEngine.setGameDeck(&deck);

    // Draw cards for both players
    deck.draw(hand1);
    deck.draw(hand2);

    // Test AdvanceOrder without NegotiateOrder
    OrdersList ordersListNoNegotiate;
    std::cout << "\n--- Test: AdvanceOrder Without Negotiation ---\n";

    // Add an AdvanceOrder from player1 to attack player2's territory
    ordersListNoNegotiate.add(new AdvanceOrder(&player1, "TerritoryA", "TerritoryC", 3));

    for (auto order : ordersListNoNegotiate.ordersVector)
    {
        order->validate();
        order->execute();
        std::cout << *order << std::endl;
    }

    // Test AdvanceOrder with NegotiateOrder
    OrdersList ordersListWithNegotiate;
    std::cout << "\n--- Test: AdvanceOrder With Prior Negotiation ---\n";

    // NegotiateOrder is added before AdvanceOrder
    ordersListWithNegotiate.add(new NegotiateOrder(&player1, "Player2"));
    ordersListWithNegotiate.add(new AdvanceOrder(&player1, "TerritoryA", "TerritoryC", 3));

    for (auto order : ordersListWithNegotiate.ordersVector)
    {
        order->validate();
        order->execute();
        std::cout << *order << std::endl;
    }

    // Clean up dynamically allocated memory in orders lists
    for (auto order : ordersListNoNegotiate.ordersVector)
    {
        delete order;
    }
    ordersListNoNegotiate.ordersVector.clear();

    for (auto order : ordersListWithNegotiate.ordersVector)
    {
        delete order;
    }
    ordersListWithNegotiate.ordersVector.clear();

    // Clean up dynamically allocated objects
    delete gameMap;
}

int main()
{
    testOrderExecution();
    return 0;
}