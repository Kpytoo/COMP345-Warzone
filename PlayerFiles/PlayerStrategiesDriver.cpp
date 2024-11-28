#include "PlayerFiles/Player.h"
#include "CardsFiles/Cards.h"
#include "OrdersFiles/Orders.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>

void printOwnedTerritories(const std::vector<Player *> &players)
{
    for (Player *player : players)
    {
        std::cout << "\n[ " << player->getPlayerName() << " ]\n";
        for (Territory *territory : player->getOwnedTerritories())
        {
            std::cout << "  - " << territory->name << " (Armies: " << territory->numberOfArmies << ")\n";
        }
    }
    std::cout << "----------------------------------------\n";
}

void testPlayerStrategies()
{
    // Setup players and territories
    auto *territory1 = new Territory();
    auto *territory2 = new Territory();
    auto *territory3 = new Territory();
    auto *territory4 = new Territory();
    auto *territory5 = new Territory();

    territory1->name = "Territory1";
    territory1->numberOfArmies = 10;

    territory2->name = "Territory2";
    territory2->numberOfArmies = 5;

    territory3->name = "Territory3";
    territory3->numberOfArmies = 3;

    territory4->name = "Territory4";
    territory4->numberOfArmies = 8;

    territory5->name = "Territory5";
    territory5->numberOfArmies = 6;

    territory1->adjacentTerritories["Territory2"] = territory2;
    territory2->adjacentTerritories["Territory1"] = territory1;
    territory2->adjacentTerritories["Territory3"] = territory3;
    territory3->adjacentTerritories["Territory2"] = territory2;
    territory3->adjacentTerritories["Territory4"] = territory4;
    territory4->adjacentTerritories["Territory3"] = territory3;
    territory4->adjacentTerritories["Territory5"] = territory5;
    territory5->adjacentTerritories["Territory4"] = territory4;

    auto *humanPlayer = new Player("HumanPlayer", {territory1, territory2});
    auto *benevolentPlayer = new Player("BenevolentPlayer", {territory3, territory4});
    auto *cheaterPlayer = new Player("CheaterPlayer", {territory5});

    // Assign strategies
    humanPlayer->setStrategy(new HumanPlayerStrategy());
    benevolentPlayer->setStrategy(new BenevolentPlayerStrategy());
    cheaterPlayer->setStrategy(new CheaterPlayerStrategy());

    // Assign sufficient reinforcement units
    humanPlayer->setNumArmies(10);
    benevolentPlayer->setNumArmies(10);
    cheaterPlayer->setNumArmies(0);

    // Display game setup
    std::cout << "\n========== GAME SETUP ==========\n";
    printOwnedTerritories({humanPlayer, benevolentPlayer, cheaterPlayer});

    // DEPLOY PHASE
    std::cout << "\n========== DEPLOY PHASE ==========\n";
    std::cout << "HumanPlayer deploys 5 armies to Territory1 and 5 to Territory2.\n";
    humanPlayer->getOrdersList()->add(new DeployOrder(humanPlayer, "Territory1", 5));
    humanPlayer->getOrdersList()->add(new DeployOrder(humanPlayer, "Territory2", 5));

    std::cout << "BenevolentPlayer deploys 6 armies to Territory3 and 4 to Territory4.\n";
    benevolentPlayer->getOrdersList()->add(new DeployOrder(benevolentPlayer, "Territory3", 6));
    benevolentPlayer->getOrdersList()->add(new DeployOrder(benevolentPlayer, "Territory4", 4));

    std::cout << "CheaterPlayer skips deploy phase.\n";

    // Execute deploy orders
    std::cout << "\n--- Executing Deploy Orders ---\n";
    for (Player *player : {humanPlayer, benevolentPlayer, cheaterPlayer})
    {
        for (Order *order : player->getOrdersList()->ordersVector)
        {
            if (order->orderType == "deploy")
            {
                order->execute();
            }
        }
    }

    // Print owned territories after deploy phase
    std::cout << "\n--- Owned Territories After Deploy Phase ---\n";
    printOwnedTerritories({humanPlayer, benevolentPlayer, cheaterPlayer});

    // ADVANCE PHASE
    std::cout << "\n========== ADVANCE PHASE ==========\n";
    std::cout << "HumanPlayer advances 4 armies from Territory1 to Territory2.\n";
    humanPlayer->getOrdersList()->add(new AdvanceOrder(humanPlayer, nullptr, "Territory1", "Territory2", 4));

    std::cout << "BenevolentPlayer advances 7 armies from Territory4 to Territory3.\n";
    benevolentPlayer->getOrdersList()->add(new AdvanceOrder(benevolentPlayer, nullptr, "Territory4", "Territory3", 7));

    std::cout << "CheaterPlayer skips advance phase.\n";

    // Execute advance orders
    std::cout << "\n--- Executing Advance Orders ---\n";
    for (Player *player : {humanPlayer, benevolentPlayer})
    {
        for (Order *order : player->getOrdersList()->ordersVector)
        {
            if (order->orderType == "advance")
            {
                order->execute();
            }
        }
        player->getOrdersList()->ordersVector.clear(); // Clear executed orders
    }

    // Print owned territories after advance phase
    std::cout << "\n--- Owned Territories After Advance Phase ---\n";
    printOwnedTerritories({humanPlayer, benevolentPlayer, cheaterPlayer});

    // Cleanup
    delete humanPlayer;
    delete benevolentPlayer;
    delete cheaterPlayer;
    delete territory1;
    delete territory2;
    delete territory3;
    delete territory4;
    delete territory5;
}
