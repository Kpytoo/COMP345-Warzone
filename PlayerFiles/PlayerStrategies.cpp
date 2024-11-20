#include "PlayerFiles/PlayerStrategies.h"
#include "PlayerFiles/Player.h"
#include "GameEngineFiles/GameEngine.h"

#include <iostream>
#include <algorithm>

// ----- HumanPlayerStrategy Implementation -----
std::vector<Territory *> HumanPlayerStrategy::toDefend(Player *player)
{
    std::cout << "Human Player: Please select territories to defend (by entering their IDs): ";
    // Assuming input handling for territory IDs exists
    return player->getOwnedTerritories();
}

std::vector<Territory *> HumanPlayerStrategy::toAttack(Player *player)
{
    std::cout << "Human Player: Please select territories to attack (by entering their IDs): ";
    // Assuming input handling for territory IDs exists
    return player->getToAttackTerritories();
}

void HumanPlayerStrategy::issueOrder(Player *player, const std::string &orderType, Deck *deck)
{
    std::cout << "Human Player: Enter details for issuing " << orderType << " order.\n";
    player->issueOrder(orderType, deck);
}

// ----- AggressivePlayerStrategy Implementation -----
std::vector<Territory *> AggressivePlayerStrategy::toDefend(Player *player)
{
    std::vector<Territory *> territories = player->getOwnedTerritories();
    std::sort(territories.begin(), territories.end(), [](Territory *a, Territory *b)
              {
                  return a->numberOfArmies > b->numberOfArmies; // Strongest first
              });
    return {territories.front()}; // Only defend the strongest territory
}

std::vector<Territory *> AggressivePlayerStrategy::toAttack(Player *player)
{
    return player->getToAttackTerritories(); // Attack all potential targets
}

void AggressivePlayerStrategy::issueOrder(Player *player, const std::string &orderType, Deck *deck)
{
    std::cout << "Aggressive Player: Issuing attack orders.\n";
    for (Territory *t : player->getToAttackTerritories())
    {
        std::cout << "Attacking " << t->name << std::endl;
        player->issueOrder("advance", deck);
    }
}

// ----- BenevolentPlayerStrategy Implementation -----
std::vector<Territory *> BenevolentPlayerStrategy::toDefend(Player *player)
{
    std::vector<Territory *> territories = player->getOwnedTerritories();
    std::sort(territories.begin(), territories.end(), [](Territory *a, Territory *b)
              {
                  return a->numberOfArmies < b->numberOfArmies; // Weakest first
              });
    return territories;
}

std::vector<Territory *> BenevolentPlayerStrategy::toAttack(Player *player)
{
    return {}; // Benevolent players do not attack
}

void BenevolentPlayerStrategy::issueOrder(Player *player, const std::string &orderType, Deck *deck)
{
    std::cout << "Benevolent Player: Deploying armies to defend territories.\n";
    for (Territory *t : toDefend(player))
    {
        player->issueOrder("deploy", deck);
    }
}

std::vector<Territory *> NeutralPlayerStrategy::toDefend(Player *player)
{
    return {}; // Neutral players don't defend
}

std::vector<Territory *> NeutralPlayerStrategy::toAttack(Player *player)
{
    return {}; // Neutral players don't attack
}

void NeutralPlayerStrategy::issueOrder(Player *player, const std::string &orderType, Deck *deck)
{
    std::cout << "Neutral Player: No orders issued.\n";
}

std::vector<Territory *> CheaterPlayerStrategy::toDefend(Player *player)
{
    // Defend all owned territories
    return player->getOwnedTerritories();
}

std::vector<Territory *> CheaterPlayerStrategy::toAttack(Player *player)
{
    // Attack all adjacent territories not owned by the cheater
    std::vector<Territory *> enemyTerritories;

    for (Territory *owned : player->getOwnedTerritories())
    {
        for (auto &adjacent : owned->adjacentTerritories)
        {
            Territory *adjacentTerritory = adjacent.second;

            // Check if the territory is not owned by the cheater
            bool isOwnedByCheater = std::find(player->getOwnedTerritories().begin(),
                                              player->getOwnedTerritories().end(),
                                              adjacentTerritory) != player->getOwnedTerritories().end();
            if (!isOwnedByCheater)
            {
                enemyTerritories.push_back(adjacentTerritory);
            }
        }
    }

    return enemyTerritories;
}

void CheaterPlayerStrategy::issueOrder(Player *player, const std::string &orderType, Deck *deck)
{
    std::cout << "Cheater Player: Automatically conquering all adjacent territories.\n";

    for (Territory *owned : player->getOwnedTerritories())
    {
        for (auto &adjacent : owned->adjacentTerritories)
        {
            Territory *adjacentTerritory = adjacent.second;

            // Check if this territory is not already owned by the cheater
            bool isOwnedByCheater = std::find(player->getOwnedTerritories().begin(),
                                              player->getOwnedTerritories().end(),
                                              adjacentTerritory) != player->getOwnedTerritories().end();

            if (!isOwnedByCheater)
            {
                // Find the current owner of the territory
                Player *currentOwner = nullptr;
                for (Player *p : *players) // Use the pointer to the players vector
                {
                    auto &ownedTerritories = p->getOwnedTerritories();
                    if (std::find(ownedTerritories.begin(), ownedTerritories.end(), adjacentTerritory) != ownedTerritories.end())
                    {
                        currentOwner = p;
                        break;
                    }
                }

                // Transfer ownership
                if (currentOwner != nullptr)
                {
                    // Remove the territory from the current owner
                    auto &ownedTerritories = currentOwner->getOwnedTerritories();
                    ownedTerritories.erase(std::remove(ownedTerritories.begin(), ownedTerritories.end(), adjacentTerritory),
                                           ownedTerritories.end());
                }

                // Add the territory to the cheater player's ownership
                player->getOwnedTerritories().push_back(adjacentTerritory);

                std::cout << "Conquered " << adjacentTerritory->name << "\n";
            }
        }
    }
}
