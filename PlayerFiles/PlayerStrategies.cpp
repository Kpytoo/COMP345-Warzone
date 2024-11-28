#include "PlayerFiles/PlayerStrategies.h"
#include "PlayerFiles/Player.h"
#include "GameEngineFiles/GameEngine.h"
#include "MapFiles/Map.h"
#include "CardsFiles/Cards.h"
#include "OrdersFiles/Orders.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <limits>

// ----- HumanPlayerStrategy Implementation -----
std::vector<Territory *> HumanPlayerStrategy::toDefend(Player *player)
{
    // Set the player's owned territories as the territories to defend
    player->setToDefendTerritories(player->getOwnedTerritories());
    // Return the list of territories to defend
    return player->getToDefendTerritories();
}

std::vector<Territory *> HumanPlayerStrategy::toAttack(Player *player)
{
    // Create a set to ensure unique enemy territories (avoid duplicates)
    std::set<Territory *> enemyTerritories;

    // Iterate over each owned territory (territories to defend)
    for (Territory *defendTerritory : player->getToDefendTerritories())
    {
        // Check adjacent territories of each owned territory
        for (const auto &adjacentPair : defendTerritory->adjacentTerritories)
        {
            Territory *adjacentTerritory = adjacentPair.second;

            // If the adjacent territory is not already in "toDefend", it's an enemy
            if (std::find(player->getToDefendTerritories().begin(), player->getToDefendTerritories().end(), adjacentTerritory) == player->getToDefendTerritories().end())
            {
                enemyTerritories.insert(adjacentTerritory);
            }
        }
    }

    // Convert the set to a vector for easier handling
    std::vector<Territory *> enemyTerritoriesVector(enemyTerritories.begin(), enemyTerritories.end());

    // Update the player's list of attackable territories
    player->setToAttackTerritories(enemyTerritoriesVector);

    // Return the attackable territories
    return player->getToAttackTerritories();
}

void HumanPlayerStrategy::issueOrder(Player *player, const std::string &orderType, Deck *deck)
{
    std::string sourceTName, targetTName;
    int armies = 0;

    if (orderType == "deploy")
    {
        if (player->getNumArmies() == 0)
        {
            std::cout << "Error: No reinforcement units left to deploy!\n";
            return;
        }

        std::cout << "- Owned Territories -\n";
        for (Territory *t : player->getOwnedTerritories())
        {
            std::cout << t->name << std::endl;
        }

        std::cout << "Enter the territory's name where you want to deploy your army units: ";
        std::getline(std::cin, targetTName);

        std::cout << "Enter the number of army units to deploy: ";
        std::cin >> armies;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        player->getOrdersList()->add(new DeployOrder(player, targetTName, armies));
    }
    else if (orderType == "advance")
    {
        std::cout << "Enter the source territory: ";
        std::getline(std::cin, sourceTName);

        std::cout << "Enter the target territory: ";
        std::getline(std::cin, targetTName);

        std::cout << "Enter the number of armies to advance: ";
        std::cin >> armies;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        player->getOrdersList()->add(new AdvanceOrder(player, nullptr, sourceTName, targetTName, armies));
    }
    else if (orderType == "airlift" || orderType == "blockade" || orderType == "negotiate" || orderType == "bomb")
    {
        // Card-based orders
        for (Card *c : player->getPlayerHand()->handVector)
        {
            if (c->cardType == orderType)
            {
                if (orderType == "airlift")
                {
                    std::cout << "Enter the source territory: ";
                    std::getline(std::cin, sourceTName);

                    std::cout << "Enter the target territory: ";
                    std::getline(std::cin, targetTName);

                    std::cout << "Enter the number of armies to airlift: ";
                    std::cin >> armies;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    player->getOrdersList()->add(new AirliftOrder(sourceTName, targetTName, armies));
                }
                else if (orderType == "blockade")
                {
                    std::cout << "Enter the territory to blockade: ";
                    std::getline(std::cin, targetTName);

                    player->getOrdersList()->add(new BlockadeOrder(targetTName));
                }
                else if (orderType == "negotiate")
                {
                    std::cout << "Enter the name of the player to negotiate with: ";
                    std::getline(std::cin, targetTName);

                    player->getOrdersList()->add(new NegotiateOrder(player, targetTName));
                }
                else if (orderType == "bomb")
                {
                    std::cout << "Enter the enemy territory to bomb: ";
                    std::getline(std::cin, targetTName);

                    player->getOrdersList()->add(new BombOrder(targetTName));
                }

                // Play the card
                c->play(*player->getOrdersList(), *deck, *player->getPlayerHand());
                return;
            }
        }

        std::cout << "Error: No card available for this order.\n";
    }
    else
    {
        std::cout << "Invalid order type.\n";
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

void BenevolentPlayerStrategy::issueOrder(Player *player, const std::string &typeOfOrder, Deck *deck)
{
    if (typeOfOrder == "deploy")
    {
        // Deploy armies to the weakest territories
        for (Territory *weakestTerritory : toDefend(player))
        {
            if (player->getNumArmies() > 0)
            {
                // Deploy as many armies as possible to the weakest territory
                int armiesToDeploy = std::min(player->getNumArmies(), weakestTerritory->numberOfArmies + 1);

                // Create a deploy order and add it to the player's orders list
                player->getOrdersList()->add(new DeployOrder(player, weakestTerritory->name, armiesToDeploy));
            }
        }
    }
    else if (typeOfOrder == "advance")
    {
        // Benevolent players do not attack but can advance armies to reinforce weaker territories
        std::vector<Territory *> defendableTerritories = toDefend(player);

        // Ensure we have at least two territories to work with
        if (defendableTerritories.size() >= 2)
        {
            // Sort territories from weakest to strongest
            Territory *target = defendableTerritories.front(); // Weakest
            Territory *source = defendableTerritories.back();  // Strongest

            // Only move armies if the source territory has enough to spare
            if (source != target && source->numberOfArmies > 1)
            {
                int armiesToAdvance = source->numberOfArmies - 1;

                // Create an advance order and add it to the player's orders list
                player->getOrdersList()->add(new AdvanceOrder(player, nullptr, source->name, target->name, armiesToAdvance));
            }
        }
    }
    else if (typeOfOrder == "airlift")
    {
        // Play the airlift card to strengthen weak territories
        bool hasAirliftCard = false;
        for (Card *card : player->getPlayerHand()->handVector)
        {
            if (card->cardType == "airlift")
            {
                hasAirliftCard = true;

                // Identify source and target territories for airlift
                std::vector<Territory *> defendableTerritories = toDefend(player);
                if (defendableTerritories.size() >= 2)
                {
                    Territory *source = defendableTerritories.back();
                    Territory *target = defendableTerritories.front();
                    if (source != target && source->numberOfArmies > 1)
                    {
                        int armiesToAirlift = source->numberOfArmies - 1;
                        player->getOrdersList()->add(new AirliftOrder(source->name, target->name, armiesToAirlift));

                        // Play the card
                        card->play(*player->getOrdersList(), *deck, *player->getPlayerHand());
                        std::cout << "Benevolent Player airlifted " << armiesToAirlift << " units from "
                                  << source->name << " to " << target->name << ".\n";
                    }
                }
                break;
            }
        }
        if (!hasAirliftCard)
        {
            std::cout << "Benevolent Player does not have an airlift card.\n";
        }
    }
    else if (typeOfOrder == "blockade")
    {
        // Play the blockade card to protect weak territories
        bool hasBlockadeCard = false;
        for (Card *card : player->getPlayerHand()->handVector)
        {
            if (card->cardType == "blockade")
            {
                hasBlockadeCard = true;

                // Blockade the weakest territory
                std::vector<Territory *> defendableTerritories = toDefend(player);
                if (!defendableTerritories.empty())
                {
                    Territory *target = defendableTerritories.front();
                    player->getOrdersList()->add(new BlockadeOrder(target->name));

                    // Play the card
                    card->play(*player->getOrdersList(), *deck, *player->getPlayerHand());
                    std::cout << "Benevolent Player used blockade on " << target->name << ".\n";
                }
                break;
            }
        }
        if (!hasBlockadeCard)
        {
            std::cout << "Benevolent Player does not have a blockade card.\n";
        }
    }
    else
    {
        std::cout << "Error: Benevolent Player cannot issue order of type \"" << typeOfOrder << "\".\n";
    }
}

std::vector<Territory *> CheaterPlayerStrategy::toDefend(Player *player)
{
    // Return all owned territories
    return player->getOwnedTerritories();
}

std::vector<Territory *> CheaterPlayerStrategy::toAttack(Player *player)
{
    // Use a set to store unique enemy territories
    std::set<Territory *> enemyTerritories;

    // Iterate through each owned territory
    for (Territory *ownedTerritory : player->getOwnedTerritories())
    {
        // Check each adjacent territory
        for (const auto &adjacentTerritory : ownedTerritory->adjacentTerritories)
        {
            Territory *enemyTerritory = adjacentTerritory.second;

            // If the adjacent territory is not owned by the cheater, add it to the set
            if (std::find(player->getOwnedTerritories().begin(), player->getOwnedTerritories().end(), enemyTerritory) == player->getOwnedTerritories().end())
            {
                enemyTerritories.insert(enemyTerritory);
            }
        }
    }

    // Convert set to vector for easier handling
    std::vector<Territory *> attackableTerritories(enemyTerritories.begin(), enemyTerritories.end());

    // Update the player's attackable territories
    player->setToAttackTerritories(attackableTerritories);

    return player->getToAttackTerritories();
}

void CheaterPlayerStrategy::issueOrder(Player *player, const std::string &orderType, Deck *deck)
{
    if (orderType == "advance")
    {
        // Identify all attackable territories
        std::vector<Territory *> attackableTerritories = toAttack(player);

        for (Territory *enemyTerritory : attackableTerritories)
        {

            std::cout << enemyTerritory->name;
        }

        std::vector<std::pair<Player *, Territory *>> toConquer;

        for (Territory *enemyTerritory : attackableTerritories)
        {
            Player *enemyPlayer = player->FindTerritoryOwner(enemyTerritory->name);

            if (!enemyPlayer)
            {
                std::cerr << "No owner found for territory: " << enemyTerritory->name << ". Skipping.\n";
                continue;
            }

            if (enemyPlayer)
            {
                toConquer.emplace_back(enemyPlayer, enemyTerritory);
            }
        }

        // Perform ownership updates after the loop
        for (const auto &conquerPair : toConquer)
        {
            Player *enemyPlayer = conquerPair.first;
            Territory *enemyTerritory = conquerPair.second;

            std::cout << "CheaterPlayer conquers " << enemyTerritory->name << " from " << enemyPlayer->getPlayerName() << ".\n";

            // Remove from enemy player
            std::vector<Territory *> updatedEnemyTerritories = enemyPlayer->getOwnedTerritories();
            updatedEnemyTerritories.erase(
                std::remove(updatedEnemyTerritories.begin(), updatedEnemyTerritories.end(), enemyTerritory),
                updatedEnemyTerritories.end());
            enemyPlayer->setOwnedTerritories(updatedEnemyTerritories);

            // Add to cheater player
            std::vector<Territory *> updatedCheaterTerritories = player->getOwnedTerritories();
            updatedCheaterTerritories.push_back(enemyTerritory);
            player->setOwnedTerritories(updatedCheaterTerritories);
        }
    }
    else
    {
        std::cout << "Invalid order type for Cheater Player.\n";
    }
}
