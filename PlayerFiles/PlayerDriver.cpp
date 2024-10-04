#include "PlayerFiles/PlayerDriver.h"

#include <iostream>
#include "PlayerFiles/Player.h"
#include "MapFiles/Map.h"
#include "OrdersFiles/Orders.h"
#include "CardsFiles/Cards.h" 


// Function to test the Player class functionality
void testPlayers()
{
    // Create territories dynamically
    auto t1 = new Territory();
    t1->name = "Territory1";
    auto t2 = new Territory();
    t2->name = "Territory2";
    auto t3 = new Territory();
    t3->name = "Territory3";

    // Create a player with a name
    Player player1("Player 1");

    // Add territories to the player's owned territories
    std::vector<Territory *> territories;
    territories.push_back(t1);
    territories.push_back(t2);
    territories.push_back(t3);
    player1.setOwnedTerritories(territories);

    // Display player details using overloaded << operator
    std::cout << "Player Details after Initialization:" << std::endl;
    std::cout << player1 << std::endl;

    // Call toDefend and toAttack methods to retrieve territories and print them
    std::vector<Territory *> defendTerritories = player1.toDefend();
    std::vector<Territory *> attackTerritories = player1.toAttack();

    std::cout << "\nTerritories to Defend: ";
    for (auto &t : defendTerritories)
    {
        std::cout << t->name << " "; // Display names of territories to defend
    }
    std::cout << std::endl;

    std::cout << "Territories to Attack: ";
    for (auto &t : attackTerritories)
    {
        std::cout << t->name << " "; // Display names of territories to attack
    }
    std::cout << std::endl;

    // Issue an order and display the updated orders list
    std::cout << "\nIssuing Order..." << std::endl;
    player1.issueOrder();
    std::cout << "Order issued. Orders List:" << std::endl;
//    std::cout << *(player1.getOrdersList()) << std::endl; // TODO: uncomment

    // Test the copy constructor by creating a copy of player1
    std::cout << "\nTesting Copy Constructor:" << std::endl;
    Player player2 = player1;
    std::cout << "Copied Player:" << std::endl;
    std::cout << player2 << std::endl;

    // Test the assignment operator by assigning player1 to a new player instance
    std::cout << "\nTesting Assignment Operator:" << std::endl;
    Player player3;
    player3 = player1;
    std::cout << "Assigned Player:" << std::endl;
    std::cout << player3 << std::endl;

    // Clean up dynamically allocated memory for territories
    delete t1;
    delete t2;
    delete t3;
}