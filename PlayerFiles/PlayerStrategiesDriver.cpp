#include "PlayerFiles/Player.h"
#include "PlayerFiles/PlayerStrategies.h"
#include "MapFiles/Map.h"
#include "PlayerFiles/PlayerStrategiesDriver.h"
#include <iostream>
#include <vector>

// Function to simulate creating test data
std::vector<Territory *> createMockTerritories()
{
    std::vector<Territory *> territories;

    // Create some mock territories
    auto *territory1 = new Territory();
    territory1->name = "Territory1";
    territory1->numberOfArmies = 10;

    auto *territory2 = new Territory();
    territory2->name = "Territory2";
    territory2->numberOfArmies = 5;

    auto *territory3 = new Territory();
    territory3->name = "Territory3";
    territory3->numberOfArmies = 20;

    territories.push_back(territory1);
    territories.push_back(territory2);
    territories.push_back(territory3);

    return territories;
}

// Test function for player strategies
void testPlayerStrategies()
{
    // Create mock territories
    std::vector<Territory *> mockTerritories = createMockTerritories();

    // Create a player and assign territories
    Player player("TestPlayer");
    player.setOwnedTerritories(mockTerritories);

    // Assign a HumanPlayerStrategy
    HumanPlayerStrategy humanStrategy;
    player.setStrategy(&humanStrategy);

    std::cout << "Testing HumanPlayerStrategy:\n";
    player.toDefend();
    player.toAttack();
    player.issueOrder("deploy", nullptr); // Passing nullptr for Deck as a placeholder

    // Assign an AggressivePlayerStrategy
    AggressivePlayerStrategy aggressiveStrategy;
    player.setStrategy(&aggressiveStrategy);

    std::cout << "\nTesting AggressivePlayerStrategy:\n";
    player.toDefend();
    player.toAttack();
    player.issueOrder("advance", nullptr);

    // Assign a BenevolentPlayerStrategy
    BenevolentPlayerStrategy benevolentStrategy;
    player.setStrategy(&benevolentStrategy);

    std::cout << "\nTesting BenevolentPlayerStrategy:\n";
    player.toDefend();
    player.toAttack();
    player.issueOrder("deploy", nullptr);

    // Clean up mock territories
    for (auto *territory : mockTerritories)
    {
        delete territory;
    }
}
