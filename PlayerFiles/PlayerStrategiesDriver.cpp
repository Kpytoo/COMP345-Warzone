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

void executeOrders(Player *player)
{
    std::cout << "\n--- Executing Orders for " << player->getPlayerName() << " ---\n";
    OrdersList *ordersList = player->getOrdersList();
    for (Order *order : ordersList->ordersVector)
    {
        order->execute(); // Call the execute method of each order
    }
    ordersList->ordersVector.clear(); // Optionally clear executed orders if needed
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
    territory5->numberOfArmies = 12;

    territory1->adjacentTerritories["Territory2"] = territory2;
    territory1->adjacentTerritories["Territory5"] = territory5;
    territory2->adjacentTerritories["Territory1"] = territory1;
    territory2->adjacentTerritories["Territory3"] = territory3;
    territory3->adjacentTerritories["Territory2"] = territory2;
    territory3->adjacentTerritories["Territory5"] = territory5;
    territory3->adjacentTerritories["Territory4"] = territory4;
    territory4->adjacentTerritories["Territory3"] = territory3;
    territory5->adjacentTerritories["Territory3"] = territory3;
    territory5->adjacentTerritories["Territory1"] = territory1;

    auto *humanPlayer = new Player("HumanPlayer", {territory1, territory2});
    auto *benevolentPlayer = new Player("BenevolentPlayer", {territory3, territory4});
    auto *cheaterPlayer = new Player("CheaterPlayer", {territory5});

    // Assign strategies
    humanPlayer->setStrategy(new HumanPlayerStrategy());
    benevolentPlayer->setStrategy(new BenevolentPlayerStrategy());
    cheaterPlayer->setStrategy(new CheaterPlayerStrategy());
    Player::players.push_back(humanPlayer);
    Player::players.push_back(benevolentPlayer);
    Player::players.push_back(cheaterPlayer);
    // Assign sufficient reinforcement units
    humanPlayer->setNumArmies(100);
    benevolentPlayer->setNumArmies(100);

    // Display game setup
    std::cout << "\n========== GAME SETUP ==========\n";
    printOwnedTerritories({humanPlayer, benevolentPlayer});

    // DEPLOY PHASE
    std::cout << "\n========== DEPLOY PHASE ==========\n";
    humanPlayer->issueOrder("deploy", nullptr);      // Human player makes decisions interactively
    benevolentPlayer->issueOrder("deploy", nullptr); // Benevolent player acts automatically

    // Execute deploy orders
    executeOrders(humanPlayer);
    executeOrders(benevolentPlayer);

    // Print owned territories after deploy phase
    std::cout << "\n--- Owned Territories After Deploy Phase ---\n";
    printOwnedTerritories({humanPlayer, benevolentPlayer});

    // ADVANCE PHASE
    std::cout << "\n========== ADVANCE PHASE ==========\n";
    humanPlayer->issueOrder("advance", nullptr);      // Human player makes decisions interactively
    benevolentPlayer->issueOrder("advance", nullptr); // Benevolent player acts automatically

    // Execute advance orders
    executeOrders(humanPlayer);
    executeOrders(benevolentPlayer);

    std::cout << "\n========== CHEATER PLAYER ADVANCE (Not Executed) ==========\n";
    cheaterPlayer->issueOrder("advance", nullptr); // Cheater player issues an advance order
    std::cout << "CheaterPlayer has issued an advance order but it will not be executed.\n";

    // Print owned territories after advance phase
    std::cout << "\n--- Owned Territories After Advance Phase ---\n";
    printOwnedTerritories({humanPlayer, benevolentPlayer, cheaterPlayer});

    // CHANGE STRATEGY DYNAMICALLY
    // std::cout << "\n========== DYNAMIC STRATEGY CHANGE ==========\n";
    // std::cout << "Changing HumanPlayer's strategy to BenevolentPlayerStrategy...\n";
    // humanPlayer->setStrategy(new BenevolentPlayerStrategy());

    // New deploy phase with updated strategy
    // std::cout << "\n========== NEW DEPLOY PHASE WITH UPDATED STRATEGY ==========\n";
    // humanPlayer->issueOrder("deploy", nullptr); // Now acts as a Benevolent player
    // benevolentPlayer->issueOrder("deploy", nullptr);

    // Execute orders again
    // executeOrders(humanPlayer);
    // executeOrders(benevolentPlayer);

    // Print owned territories after new deploy phase
    // std::cout << "\n--- Owned Territories After New Deploy Phase ---\n";
    // printOwnedTerritories({humanPlayer, benevolentPlayer});

    // Cleanup
    delete humanPlayer;
    delete benevolentPlayer;
    delete territory1;
    delete territory2;
    delete territory3;
    delete territory4;
}
