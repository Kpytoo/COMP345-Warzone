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

// Function to create and setup territories and adjacency
void testPlayerStrategies()
{
    std::cout << "Entered testPlayerStrategies function.\n";
    srand(static_cast<unsigned>(time(0)));

    // Initialize components
    std::vector<Territory *> territories;
    std::vector<Player *> players;
    Deck deck;

    // Setup territories and adjacency
    auto *territory1 = new Territory();
    auto *territory2 = new Territory();
    auto *territory3 = new Territory();
    auto *territory4 = new Territory();
    auto *territory5 = new Territory();

    // Set names and armies
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

    // Set adjacency
    territory1->adjacentTerritories["Territory2"] = territory2;
    territory1->adjacentTerritories["Territory3"] = territory3;
    territory2->adjacentTerritories["Territory1"] = territory1;
    territory2->adjacentTerritories["Territory4"] = territory4;
    territory3->adjacentTerritories["Territory1"] = territory1;
    territory3->adjacentTerritories["Territory5"] = territory5;

    territories = {territory1, territory2, territory3, territory4, territory5};

    // Setup players
    auto *humanPlayer = new Player("HumanPlayer", {territories[0], territories[1]});
    auto *benevolentPlayer = new Player("BenevolentPlayer", {territories[2]});
    auto *cheaterPlayer = new Player("CheaterPlayer", {territories[3]});

    std::cout << "Debug: Created HumanPlayer, BenevolentPlayer, and CheaterPlayer.\n";

    // Set strategies
    humanPlayer->setStrategy(new HumanPlayerStrategy());
    benevolentPlayer->setStrategy(new BenevolentPlayerStrategy());
    cheaterPlayer->setStrategy(new CheaterPlayerStrategy());

    std::cout << "Debug: Strategies assigned to players.\n";

    // Assign hands
    humanPlayer->setPlayerHand(new Hand());
    benevolentPlayer->setPlayerHand(new Hand());
    cheaterPlayer->setPlayerHand(new Hand());

    std::cout << "Debug: Hands assigned to players.\n";

    // Push players into the vector
    players.push_back(humanPlayer);
    players.push_back(benevolentPlayer);
    players.push_back(cheaterPlayer);

    std::cout << "Number of players: " << players.size() << "\n";

    // Assign reinforcement units
    humanPlayer->setNumArmies(10);
    benevolentPlayer->setNumArmies(5);
    cheaterPlayer->setNumArmies(8);

    // Verify adjacency setup
    std::cout << "Debug: Verifying adjacency setup.\n";
    for (auto *territory : territories)
    {
        std::cout << territory->name << " adjacencies: ";
        for (const auto &adjacent : territory->adjacentTerritories)
        {
            std::cout << adjacent.first << " ";
        }
        std::cout << "\n";
    }

    // Test strategies
    for (Player *player : players)
    {
        std::cout << "\n=== Player: " << player->getPlayerName() << " ===\n";

        // Print owned territories
        std::cout << "Owned Territories:\n";
        for (Territory *t : player->getOwnedTerritories())
        {
            std::cout << "- " << t->name << " (" << t->numberOfArmies << " armies)\n";
        }

        // Print toDefend territories
        std::cout << "\nTo Defend:\n";
        for (Territory *t : player->toDefend())
        {
            std::cout << "- " << t->name << "\n";
        }

        // Print toAttack territories
        std::cout << "\nTo Attack:\n";
        for (Territory *t : player->toAttack())
        {
            std::cout << "- " << t->name << "\n";
        }

        // Test strategy-specific orders
        if (dynamic_cast<HumanPlayerStrategy *>(player->getStrategy()))
        {
            std::istringstream simulatedInput("deploy\nTerritory1\n5\nend turn\n");
            std::cin.rdbuf(simulatedInput.rdbuf());
            player->issueOrder("deploy", &deck);
            std::cout << "Debug: Finished issuing orders for HumanPlayer.\n";
        }
        else if (dynamic_cast<BenevolentPlayerStrategy *>(player->getStrategy()))
        {
            auto beforeDefend = player->toDefend();
            std::cout << "Debug: Weakest territory before deploy: " << beforeDefend.front()->name
                      << " with " << beforeDefend.front()->numberOfArmies << " armies.\n";

            player->issueOrder("deploy", &deck);

            auto afterDefend = player->toDefend();
            std::cout << "Debug: Weakest territory after deploy: " << afterDefend.front()->name
                      << " with " << afterDefend.front()->numberOfArmies << " armies.\n";

            if (afterDefend.front()->numberOfArmies > beforeDefend.front()->numberOfArmies)
            {
                std::cout << "Benevolent Player successfully reinforced weakest territory.\n";
            }
            else
            {
                std::cout << "Benevolent Player failed to reinforce weakest territory. Check logic.\n";
            }
        }
        else if (dynamic_cast<CheaterPlayerStrategy *>(player->getStrategy()))
        {
            size_t beforeOwnedTerritories = player->getOwnedTerritories().size();
            std::cout << "Debug: CheaterPlayer owned territories before: " << beforeOwnedTerritories << "\n";

            player->issueOrder("advance", &deck);

            size_t afterOwnedTerritories = player->getOwnedTerritories().size();
            std::cout << "Debug: CheaterPlayer owned territories after: " << afterOwnedTerritories << "\n";

            if (afterOwnedTerritories > beforeOwnedTerritories)
            {
                std::cout << "Cheater Player successfully conquered territories.\n";
            }
            else
            {
                std::cout << "Cheater Player failed to conquer territories. Check logic.\n";
            }
        }
    }

    // Cleanup
    for (Territory *territory : territories)
    {
        delete territory;
    }
    for (Player *player : players)
    {
        delete player;
    }
}
