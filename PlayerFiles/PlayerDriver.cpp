#include <iostream>
#include "PlayerFiles/Player.h"
#include "MapFiles/Territory.h"
#include "/OrdersList.h" //TODO when created
#include "/Hand.h"       //TODO when created

void testPlayers()
{

    Territory *t1 = new Territory("Territory1");
    Territory *t2 = new Territory("Territory2");
    Territory *t3 = new Territory("Territory3");

    // Creating a player
    Player player1("Player 1");

    // Adding territories to the player's owned territories
    player1.OwnedTerritories.push_back(t1);
    player1.OwnedTerritories.push_back(t2);
    player1.OwnedTerritories.push_back(t3);

    // Display the player info
    std::cout << "Player Details after Initialization:" << std::endl;
    std::cout << player1 << std::endl;

    // Call toDefend and toAttack methods and print the results
    std::vector<Territory *> defendTerritories = player1.toDefend();
    std::vector<Territory *> attackTerritories = player1.toAttack();

    std::cout << "\nTerritories to Defend: ";
    for (auto &t : defendTerritories)
    {
        std::cout << t->getName() << " ";
    }
    std::cout << std::endl;

    std::cout << "Territories to Attack: ";
    for (auto &t : attackTerritories)
    {
        std::cout << t->getName() << " ";
    }
    std::cout << std::endl;

    // Issue an order
    std::cout << "\nIssuing Order..." << std::endl;
    player1.issueOrder();
    std::cout << "Order issued. Orders List:" << std::endl;
    std::cout << *(player1.ordersList) << std::endl;

    // Copy constructor test
    std::cout << "\nTesting Copy Constructor:" << std::endl;
    Player player2 = player1;
    std::cout << "Copied Player:" << std::endl;
    std::cout << player2 << std::endl;

    // Assignment operator test
    std::cout << "\nTesting Assignment Operator:" << std::endl;
    Player player3;
    player3 = player1;
    std::cout << "Assigned Player:" << std::endl;
    std::cout << player3 << std::endl;

    // Clean up dynamic memory
    delete t1;
    delete t2;
    delete t3;
}

int main()
{
    // Run the test function
    testPlayers();

    return 0;
}
