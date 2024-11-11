#include "OrdersFiles/OrdersDriver.h"
#include "OrdersFiles/Orders.h"
#include <iostream>
#include "PlayerFiles/Player.h"
#include "GameEngineFiles/GameEngine.h"
#include "Orders.h"
#include "CardsFiles/Cards.h"
#include "PlayerFiles/Player.h"
#include "MapFiles/Map.h"
void testOrdersLists() {
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
}

// Test function to demonstrate order validation and execution
void testOrderExecution()
{
    // Setup game engine and map
    GameEngine gameEngine;
    Map *gameMap = new Map();
    MapLoader mapLoader;

    // Load the map from file
    mapLoader.LoadMap("../SomeMapsFromOnline/Montreal/Grand Montreal.map", gameMap);
    gameEngine.setCurrentMap(gameMap);

    // Assign territories to players based on map data
    std::vector<Territory *> player1Territories;
    std::vector<Territory *> player2Territories;

    if (gameMap->territories.count("Lachine"))
    {
        player1Territories.push_back(gameMap->territories["Lachine"]);
    }
    if (gameMap->territories.count("Cote St-Luc"))
    {
        player1Territories.push_back(gameMap->territories["Cote St-Luc"]);
    }
    if (gameMap->territories.count("NDG CDN"))
    {
        player2Territories.push_back(gameMap->territories["NDG CDN"]);
    }
    if (gameMap->territories.count("Mont Royal"))
    {
        player2Territories.push_back(gameMap->territories["Mont Royal"]);
    }

    // Create players
    Player player1("Player1", player1Territories);
    Player player2("Player2", player2Territories);
    // Create Neutral player for blockade orders
    Player neutralPlayer("Neutral");

    // Set players' reinforcement pools
    player1.setNumArmies(20);
    player2.setNumArmies(20);

    // Set up deck and hands for players
    Hand* hand1Pointer = new Hand();
    Hand* hand2Pointer = new Hand();
    Deck deck;
    Hand& hand1 = *hand1Pointer;
    Hand& hand2 = *hand1Pointer;
    gameEngine.setGameDeck(&deck);

    // Draw cards for both players
    deck.draw(hand1);
    deck.draw(hand2);

    player1.setPlayerHand(hand1Pointer);
    player2.setPlayerHand(hand2Pointer);
    player1.deck = &deck;
    player2.deck = &deck;

    // Test 1: Deploy Order
    std::cout << "=== Testing Deploy Order ===\n";
    DeployOrder* deployOrder = new DeployOrder(&player1, "Lachine", 14);
    deployOrder->execute();
    std::cout << "Lachine now has " << gameMap->territories["Lachine"]->numberOfArmies << " armies\n\n";

    // Test invalid deploy
    std::cout << "Testing invalid deploy to enemy territory:\n";
    DeployOrder* invalidDeploy = new DeployOrder(&player1, "NDG CDN", 5);
    invalidDeploy->execute();
    std::cout << "\n";

    // Test 2: Advance Order (Moving)
    std::cout << "=== Testing Advance Order (Moving) ===\n";
    AdvanceOrder* moveOrder = new AdvanceOrder(&player1, &player2, "Lachine", "NDG CDN", 7);
    moveOrder->execute();
    std::cout << "After advance attack:\n";
    std::cout << "Lachine (Player1): " << gameMap->territories["Lachine"]->numberOfArmies << " armies\n";
    std::cout << "NDG CDN (Player2): " << gameMap->territories["NDG CDN"]->numberOfArmies << " armies\n";
    std::cout << "Player1 Cards: " << *player1.getPlayerHand() << "\n\n";

    // Test 3: Bomb Order
    std::cout << "=== Testing Bomb Order ===\n";
    BombOrder* bombOrder = new BombOrder("NDG CDN");
    bombOrder->player = &player2;
    bombOrder->execute();
    std::cout << "NDG CDN now has " << gameMap->territories["NDG CDN"]->numberOfArmies << " armies\n\n";

    // Test 4: Blockade Order
    std::cout << "=== Testing Blockade Order ===\n";
    BlockadeOrder* blockadeOrder = new BlockadeOrder("Lachine");
    blockadeOrder->player = &player1;
    blockadeOrder->neutralPlayer = &neutralPlayer;
    blockadeOrder->execute();
    std::cout << "Lachine now belongs to Neutral player with " << gameMap->territories["Lachine"]->numberOfArmies << " armies\n\n";

    // Test 5: Airlift Order
    player1.getOwnedTerritories().push_back(gameMap->territories["Longueuil"]); // Give player fair away territory to use for airlift
    gameMap->territories["Longueuil"]->numberOfArmies = 100;

    std::cout << "=== Testing Airlift Order ===\n";
    AirliftOrder* airliftOrder = new AirliftOrder("Longueuil", "NDG CDN", 50);
    airliftOrder->player = &player1;
    airliftOrder->execute();
    std::cout << "Longueuil (Player1): " << gameMap->territories["Longueuil"]->numberOfArmies << " armies\n";
    std::cout << "NDG CDN (Player1): " << gameMap->territories["NDG CDN"]->numberOfArmies << " armies\n\n";
    std::cout << "\n";

    /**
     * Invalid order tests
     */
    std::cout << "\n=== TESTING INVALID ORDERS ===\n";
    // Invalid Deploy Order Tests
    std::cout << "\n=== Testing Invalid Deploy Orders ===\n";
    // Test deploying more armies than available in reinforcement pool
    DeployOrder* invalidDeployTooMany = new DeployOrder(&player1, "Longueuil", 50); // More than player1's 20 armies
    invalidDeployTooMany->execute();

    // Invalid Advance Order Tests
    std::cout << "\n=== Testing Invalid Advance Orders ===\n";
    // Test advancing from territory player doesn't own
    AdvanceOrder* invalidAdvanceWrongSource = new AdvanceOrder(&player1, &player2, "NDG CDN", "Lachine", 5);
    invalidAdvanceWrongSource->execute();
    // Test advancing to non-adjacent territory
    AdvanceOrder* invalidAdvanceNotAdjacent = new AdvanceOrder(&player1, &player2, "Cote St-Luc", "Mont Royal", 5);
    invalidAdvanceNotAdjacent->execute();
    // Test advancing more armies than present in source territory
    AdvanceOrder* invalidAdvanceTooMany = new AdvanceOrder(&player1, &player2, "Lachine", "NDG CDN", 100);
    invalidAdvanceTooMany->execute();

    // Invalid Bomb Order Tests
    std::cout << "\n=== Testing Invalid Bomb Orders ===\n";
    // Test bombing own territory
    BombOrder* invalidBombOwnTerritory = new BombOrder("Cote St-Luc");
    invalidBombOwnTerritory->player = &player1;
    invalidBombOwnTerritory->execute();
    // Test bombing non-adjacent territory
    BombOrder* invalidBombNotAdjacent = new BombOrder("Rosemere");
    invalidBombNotAdjacent->player = &player1;
    invalidBombNotAdjacent->execute();

    // Invalid Blockade Order Tests
    std::cout << "\n=== Testing Invalid Blockade Orders ===\n";
    // Test blockading enemy territory
    BlockadeOrder* invalidBlockadeEnemy = new BlockadeOrder("Mont Royal");
    invalidBlockadeEnemy->player = &player1;
    invalidBlockadeEnemy->neutralPlayer = &neutralPlayer;
    invalidBlockadeEnemy->execute();

    // Invalid Airlift Order Tests
    std::cout << "\n=== Testing Invalid Airlift Orders ===\n";
    // Test airlifting from territory player doesn't own
    AirliftOrder* invalidAirliftWrongSource = new AirliftOrder("Mont Royal", "Lachine", 5);
    invalidAirliftWrongSource->player = &player1;
    invalidAirliftWrongSource->execute();
    // Test airlifting to enemy territory
    AirliftOrder* invalidAirliftEnemyTarget = new AirliftOrder("Cote St-Luc", "Mont Royal", 5);
    invalidAirliftEnemyTarget->player = &player1;
    invalidAirliftEnemyTarget->execute();
    // Test airlifting more armies than present
    AirliftOrder* invalidAirliftTooMany = new AirliftOrder("Longueuil", "Cote St-Luc", 150);
    invalidAirliftTooMany->player = &player1;
    invalidAirliftTooMany->execute();

    // Invalid Negotiate Order Tests
    std::cout << "\n=== Testing Invalid Negotiate Orders ===\n";
    // Test negotiating with self
    NegotiateOrder* invalidNegotiateSelf = new NegotiateOrder(&player1, "Player1");
    invalidNegotiateSelf->execute();

    // Test 6: Negotiate Order
    std::cout << "\n\n=== Testing Negotiate Order ===\n";
    NegotiateOrder* negotiateOrder = new NegotiateOrder(&player1, "Player2");
    negotiateOrder->execute();

    // Test that negotiation prevents attacks
    std::cout << "Testing attack between negotiated players:\n";
    AdvanceOrder* preventedAttack = new AdvanceOrder(&player1, &player2, "NDG CDN", "Mont Royal", 1);
    preventedAttack->execute();

    delete deployOrder;
    delete invalidDeploy;
    delete moveOrder;
    delete bombOrder;
    delete blockadeOrder;
    delete airliftOrder;
    delete negotiateOrder;
    delete preventedAttack;
    delete invalidDeployTooMany;
    delete invalidAdvanceWrongSource;
    delete invalidAdvanceNotAdjacent;
    delete invalidAdvanceTooMany;
    delete invalidBombOwnTerritory;
    delete invalidBombNotAdjacent;
    delete invalidBlockadeEnemy;
    delete invalidAirliftWrongSource;
    delete invalidAirliftEnemyTarget;
    delete invalidAirliftTooMany;
    delete invalidNegotiateSelf;
}