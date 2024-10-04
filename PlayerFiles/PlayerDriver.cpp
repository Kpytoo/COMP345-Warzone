#include <iostream>
#include "PlayerFiles/Player.h"
#include "MapFiles/Map.h"
#include "/OrdersList.h"      //placeholder
#include "CardsFiles/Cards.h" 


// Function to test the Player class functionality
void testPlayers()
{
    // Create territories dynamically
    Territory *t1 = new Territory("Territory1");
    Territory *t2 = new Territory("Territory2");
    Territory *t3 = new Territory("Territory3");

    // Create a player with a name
    Player player1("Player 1");

    // Add territories to the player's owned territories
    player1.OwnedTerritories.push_back(t1);
    player1.OwnedTerritories.push_back(t2);
    player1.OwnedTerritories.push_back(t3);

    // Display player details using overloaded << operator
    std::cout << "Player Details after Initialization:" << std::endl;
    std::cout << player1 << std::endl;

    // Call toDefend and toAttack methods to retrieve territories and print them
    std::vector<Territory *> defendTerritories = player1.toDefend();
    std::vector<Territory *> attackTerritories = player1.toAttack();

    std::cout << "\nTerritories to Defend: ";
    for (auto &t : defendTerritories)
    {
        std::cout << t->getName() << " "; // Display names of territories to defend
    }
    std::cout << std::endl;

    std::cout << "Territories to Attack: ";
    for (auto &t : attackTerritories)
    {
        std::cout << t->getName() << " "; // Display names of territories to attack
    }
    std::cout << std::endl;

    // Issue an order and display the updated orders list
    std::cout << "\nIssuing Order..." << std::endl;
    player1.issueOrder();
    std::cout << "Order issued. Orders List:" << std::endl;
    std::cout << *(player1.ordersList) << std::endl;

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

int main()
{
    // Run the test function to verify Player class functionality
    testPlayers();

    return 0;
}
