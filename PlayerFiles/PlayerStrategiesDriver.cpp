#include "PlayerFiles/PlayerStrategiesDriver.h"
#include "PlayerFiles/Player.h"
#include "CardsFiles/Cards.h"
#include <iostream>
#include <vector>

/**
 * @brief Test driver function to demonstrate the Player Strategy pattern implementation
 *
 * This function demonstrates three key aspects of the Strategy pattern:
 * 1. Different players can be assigned different strategies that lead to different behaviors
 *    - Tests HumanPlayerStrategy, AggressivePlayerStrategy, BenevolentPlayerStrategy,
 *      NeutralPlayerStrategy, and CheaterPlayerStrategy
 *    - Shows how each strategy implements unique toDefend() and toAttack() behaviors
 *    - Demonstrates different order issuing patterns for each strategy type
 *
 * 2. Strategy can be changed dynamically during gameplay
 *    - Shows conversion of a Neutral player to an Aggressive player
 *    - Demonstrates behavior changes after strategy switch
 *
 * 3. Human vs Computer decision making
 *    - Shows how human players make decisions through user interaction
 *    - Demonstrates automatic decision making for computer-controlled players
 *
 * The test uses a Montreal map with various territories assigned to different players.
 * Each player is given initial armies and territories to demonstrate territory management
 * and order issuing behaviors. The test also includes execution of issued orders and
 * display of player information including card hands.
 *
 * Memory management: The function handles cleanup of all dynamically allocated objects
 * including the game map, deck, and player objects.
 */
void testPlayerStrategies() {
    std::cout << "\n==================== Testing Player Strategies ====================\n";

    // Load a map for testing
    Map* gameMap = new Map();
    MapLoader::LoadMap("../SomeMapsFromOnline/Montreal/Grand Montreal.map", gameMap);

    // Initialize deck
    Deck* deck = new Deck();

    // 1. Demonstrate different players with different strategies
    std::cout << "\n1. Testing different player strategies:\n";

    // Create players with different strategies
    Player* humanPlayer = new Player("Human Player");
    humanPlayer->setStrategy(new HumanPlayerStrategy(humanPlayer));
    humanPlayer->getOwnedTerritories().push_back(gameMap->territories.at("Ahuntsic"));
    humanPlayer->getOwnedTerritories().push_back(gameMap->territories.at("Cote St-Luc"));
    humanPlayer->getOwnedTerritories().push_back(gameMap->territories.at("NDG CDN"));
    humanPlayer->getOwnedTerritories().push_back(gameMap->territories.at("Outremont"));
    Player::players.push_back(humanPlayer);

    Player* aggressivePlayer = new Player("Aggressive Player");
    aggressivePlayer->setStrategy(new AggressivePlayerStrategy(aggressivePlayer));
    aggressivePlayer->getOwnedTerritories().push_back(gameMap->territories.at("Lachine"));
    aggressivePlayer->getOwnedTerritories().push_back(gameMap->territories.at("Mont Royal"));
    aggressivePlayer->getOwnedTerritories().push_back(gameMap->territories.at("La Salle"));
    Player::players.push_back(aggressivePlayer);

    Player* benevolentPlayer = new Player("Benevolent Player");
    benevolentPlayer->setStrategy(new BenevolentPlayerStrategy(benevolentPlayer));
    benevolentPlayer->getOwnedTerritories().push_back(gameMap->territories.at("Brossard"));
    benevolentPlayer->getOwnedTerritories().push_back(gameMap->territories.at("Verdun"));
    benevolentPlayer->getOwnedTerritories().push_back(gameMap->territories.at("Westmount"));
    Player::players.push_back(benevolentPlayer);

    Player* neutralPlayer = new Player("Neutral Player");
    neutralPlayer->setStrategy(new NeutralPlayerStrategy(neutralPlayer));
    neutralPlayer->getOwnedTerritories().push_back(gameMap->territories.at("Boucherville"));
    neutralPlayer->getOwnedTerritories().push_back(gameMap->territories.at("Longueuil"));
    neutralPlayer->getOwnedTerritories().push_back(gameMap->territories.at("Greenfield Park"));
    Player::players.push_back(neutralPlayer);

    Player* cheaterPlayer = new Player("Cheater Player");
    cheaterPlayer->setStrategy(new CheaterPlayerStrategy(cheaterPlayer));
    cheaterPlayer->getOwnedTerritories().push_back(gameMap->territories.at("St-Hubert"));
    cheaterPlayer->getOwnedTerritories().push_back(gameMap->territories.at("St-Philippe"));
    cheaterPlayer->getOwnedTerritories().push_back(gameMap->territories.at("St-Mathieu"));
    Player::players.push_back(cheaterPlayer);

    // Set initial armies for testing
    humanPlayer->setNumArmies(5);
    humanPlayer->reinforcement_units = 5;
    aggressivePlayer->setNumArmies(5);
    aggressivePlayer->reinforcement_units = 5;
    benevolentPlayer->setNumArmies(5);
    benevolentPlayer->reinforcement_units = 5;
    neutralPlayer->setNumArmies(5);
    neutralPlayer->reinforcement_units = 5;
    cheaterPlayer->setNumArmies(5);
    cheaterPlayer->reinforcement_units = 5;

    // Test toDefend() and toAttack() for each player
    std::cout << "\nTesting toDefend() and toAttack() for each player:\n";

    // Human Player
    std::cout << "\nHuman Player territories to defend: ";
    for (Territory* t : humanPlayer->toDefend()) {
        std::cout << t->name << " ";
    }
    std::cout << "\nHuman Player territories to attack: ";
    for (Territory* t : humanPlayer->toAttack()) {
        std::cout << t->name << " ";
    }

    // Aggressive Player
    std::cout << "\n\nAggressive Player territories to defend: ";
    for (Territory* t : aggressivePlayer->toDefend()) {
        std::cout << t->name << " ";
    }
    std::cout << "\nAggressive Player territories to attack: ";
    for (Territory* t : aggressivePlayer->toAttack()) {
        std::cout << t->name << " ";
    }

    // Benevolent Player
    std::cout << "\n\nBenevolent Player territories to defend: ";
    for (Territory* t : benevolentPlayer->toDefend()) {
        std::cout << t->name << " ";
    }
    std::cout << "\nBenevolent Player territories to attack: ";
    for (Territory* t : benevolentPlayer->toAttack()) {
        std::cout << t->name << " ";
    }

    // Neutral Player
    std::cout << "\n\nNeutral Player territories to defend: ";
    for (Territory* t : neutralPlayer->toDefend()) {
        std::cout << t->name << " ";
    }
    std::cout << "\nNeutral Player territories to attack: ";
    for (Territory* t : neutralPlayer->toAttack()) {
        std::cout << t->name << " ";
    }

    // 2. Demonstrate dynamic strategy change
    std::cout << "\n\n2. Testing dynamic strategy change:\n";
    std::cout << "Converting Neutral Player to Aggressive Player (simulating being attacked)\n";

    // Change Neutral to Aggressive
    delete neutralPlayer->getStrategy();
    neutralPlayer->setStrategy(new AggressivePlayerStrategy(neutralPlayer));

    std::cout << "After conversion - territories to attack: ";
    for (Territory* t : neutralPlayer->toAttack()) {
        std::cout << t->name << " ";
    }

    // 3. Demonstrate order issuing
    std::cout << "\n\n3. Testing order issuing:\n";

    // Test automatic order issuing for computer players
    std::cout << "\nTesting Aggressive Player order issuing:\n";
    aggressivePlayer->issueOrder(deck);
    aggressivePlayer->getOrdersList()->ordersVector.at(0)->execute();
    aggressivePlayer->issueOrder(deck);

    std::cout << "\nTesting Benevolent Player order issuing:\n";
    benevolentPlayer->issueOrder(deck);
    benevolentPlayer->getOrdersList()->ordersVector.at(0)->execute();
    benevolentPlayer->issueOrder(deck);

    std::cout << "\nTesting Neutral Player order issuing:\n";
    neutralPlayer->issueOrder(deck);
    neutralPlayer->getOrdersList()->ordersVector.at(0)->execute();
    neutralPlayer->issueOrder(deck);

    std::cout << "\nTesting Cheater Player order issuing:\n";
    cheaterPlayer->issueOrder(deck);
    cheaterPlayer->issueOrder(deck);

    std::cout << "\nTesting Human Player order issuing (requires user input):\n";
    humanPlayer->issueOrder(deck);
    humanPlayer->getOrdersList()->ordersVector.at(0)->execute();
    humanPlayer->issueOrder(deck);

    // 4. Display all player info with hand info
    std::cout << "\n\4. Display all player info including card hand list:\n";
    std::cout << "\nAggressive Player:\n"<<*aggressivePlayer<<std::endl;

    std::cout << "\nBenevolent Player:\n"<<*benevolentPlayer<<std::endl;

    std::cout << "\nNeutral Player:\n"<<*neutralPlayer<<std::endl;

    std::cout << "\nCheater Player:\n"<<*cheaterPlayer<<std::endl;

    std::cout << "\nHuman Player:\n"<<*humanPlayer<<std::endl;

    // Cleanup
    delete gameMap;
    delete deck;
    delete humanPlayer;
    delete aggressivePlayer;
    delete benevolentPlayer;
    delete neutralPlayer;
    delete cheaterPlayer;

    std::cout << "\n================== Player Strategies Testing Complete ==================\n";
}
