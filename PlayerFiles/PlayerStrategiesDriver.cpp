#include "PlayerFiles/PlayerStrategies.h"
#include "PlayerFiles/Player.h"
#include "CardsFiles/Cards.h"
#include "OrdersFiles/Orders.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>

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
    auto *benevolentPlayer = new Player("BenevolentPlayer", {territory3});
    auto *cheaterPlayer = new Player("CheaterPlayer", {territory4});

    // Assign strategies
    humanPlayer->setStrategy(new HumanPlayerStrategy());
    benevolentPlayer->setStrategy(new BenevolentPlayerStrategy());
    cheaterPlayer->setStrategy(new CheaterPlayerStrategy());

    // Assign sufficient reinforcement units
    humanPlayer->setNumArmies(50);
    benevolentPlayer->setNumArmies(50);
    cheaterPlayer->setNumArmies(50);

    // Display game setup
    std::cout << "Game setup:\n";
    std::cout << "- HumanPlayer owns Territory1 and Territory2.\n";
    std::cout << "- BenevolentPlayer owns Territory3.\n";
    std::cout << "- CheaterPlayer owns Territory4.\n";

    // Process strategies
    std::vector<Player *> players = {humanPlayer, benevolentPlayer, cheaterPlayer};
    Deck *deck = new Deck();

    for (Player *player : players)
    {
        std::cout << "\n=== Player: " << player->getPlayerName() << " ===\n";

        if (dynamic_cast<HumanPlayerStrategy *>(player->getStrategy()))
        {
            // Human player issues orders interactively
            while (true)
            {
                std::cout << "\nHumanPlayer, choose an order type (deploy, advance, or end turn): ";
                std::string orderType;
                std::getline(std::cin, orderType);

                if (orderType == "end turn")
                {
                    std::cout << "Ending HumanPlayer's turn.\n";
                    break;
                }

                if (orderType == "deploy" || orderType == "advance")
                {
                    player->getStrategy()->issueOrder(player, orderType, deck);
                }
                else
                {
                    std::cout << "Invalid order type. Try again.\n";
                }
            }
        }
        else
        {
            // AI strategies issue orders automatically
            player->getStrategy()->issueOrder(player, "deploy", deck);
            player->getStrategy()->issueOrder(player, "advance", deck);
        }

        // Execute all issued orders
        std::cout << "\nExecuting orders for Player: " << player->getPlayerName() << "\n";
        for (Order *order : player->getOrdersList()->ordersVector)
        {
            order->execute();
        }

        // Print updated owned territories and their army counts
        std::cout << "\nUpdated Territories Owned by " << player->getPlayerName() << ":\n";
        for (Territory *territory : player->getOwnedTerritories())
        {
            std::cout << "- " << territory->name << " (Armies: " << territory->numberOfArmies << ")\n";
        }
    }

    // Cleanup
    delete humanPlayer;
    delete benevolentPlayer;
    delete cheaterPlayer;
    delete territory1;
    delete territory2;
    delete territory3;
    delete territory4;
    delete territory5;
    delete deck;
}
