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

void HumanPlayerStrategy::issueOrder(Player *player, Deck *deck)
{
    // Clear the input buffer to avoid issues between std::cin and std::getline
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true)
    {
        // Display available options
        std::cout << "\nHumanPlayerStrategy: Please decide an order to issue for Player " << player->getPlayerName() << ".\n";
        std::cout << "Available options: deploy, advance, airlift, blockade, negotiate, bomb, or end turn.\n";

        // Show the player's hand
        std::cout << "\nYour current hand of cards:\n";
        if (player->getPlayerHand()->handVector.empty())
        {
            std::cout << "- No cards available.\n";
        }
        else
        {
            for (Card *card : player->getPlayerHand()->handVector)
            {
                std::cout << "- " << card->cardType << std::endl;
            }
        }

        // Get the order type from the player
        std::string orderType;
        std::cout << "\nEnter the type of order: ";
        std::getline(std::cin, orderType);

        // Handle the "end turn" case
        if (orderType == "end turn")
        {
            std::cout << "Ending turn.\n";
            break;
        }

        // Handle the order type
        issueOrder(player, orderType, deck);
    }
}

void HumanPlayerStrategy::issueOrder(Player *player, const std::string &orderType, Deck *deck)
{
    std::string sourceTName, targetTName;
    int armies = 0;

    if (orderType == "deploy")
    {
        // Deploy logic
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

        // Validate target territory
        Territory *targetTerritory = nullptr;
        for (Territory *t : player->getOwnedTerritories())
        {
            if (t->name == targetTName)
            {
                targetTerritory = t;
                break;
            }
        }

        if (!targetTerritory)
        {
            std::cout << "Error: Invalid territory name! You must select one of your owned territories.\n";
            return;
        }

        std::cout << "Enter the number of army units to deploy: ";
        std::cin >> armies;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (armies <= 0 || armies > player->getNumArmies())
        {
            std::cout << "Error: Invalid number of armies. Please try again.\n";
            return;
        }

        player->getOrdersList()->ordersVector.push_back(new DeployOrder(player, targetTName, armies));
    }
    else if (orderType == "advance")
    {
        // Advance logic
        std::cout << "- Owned Territories -\n";
        for (Territory *t : player->getOwnedTerritories())
        {
            std::cout << t->name << " (Armies: " << t->numberOfArmies << ")" << std::endl;
        }

        std::cout << "Enter the source territory: ";
        std::getline(std::cin, sourceTName);

        std::cout << "Enter the target territory: ";
        std::getline(std::cin, targetTName);

        std::cout << "Enter the number of armies to advance: ";
        std::cin >> armies;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        Player *enemyPlayer = player->FindTerritoryOwner(targetTName, *player->getPlayers());
        player->getOrdersList()->ordersVector.push_back(new AdvanceOrder(player, enemyPlayer, sourceTName, targetTName, armies));
    }
    else if (orderType == "airlift" || orderType == "blockade" || orderType == "negotiate" || orderType == "bomb")
    {
        // Validate that the player has the required card
        bool hasCard = false;
        for (Card *c : player->getPlayerHand()->handVector)
        {
            if (c->cardType == orderType)
            {
                hasCard = true;

                // Specific card logic
                if (orderType == "airlift")
                {
                    std::cout << "Enter the source territory: ";
                    std::getline(std::cin, sourceTName);

                    std::cout << "Enter the target territory: ";
                    std::getline(std::cin, targetTName);

                    std::cout << "Enter the number of armies to airlift: ";
                    std::cin >> armies;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    player->getOrdersList()->ordersVector.push_back(new AirliftOrder(sourceTName, targetTName, armies));
                }
                else if (orderType == "blockade")
                {
                    std::cout << "Enter the territory to blockade: ";
                    std::getline(std::cin, targetTName);

                    player->getOrdersList()->ordersVector.push_back(new BlockadeOrder(targetTName));
                }
                else if (orderType == "negotiate")
                {
                    std::cout << "Enter the name of the player to negotiate with: ";
                    std::string targetPlayerName;
                    std::getline(std::cin, targetPlayerName);

                    Player *targetPlayer = nullptr;
                    for (Player *p : *player->getPlayers())
                    {
                        if (p->getPlayerName() == targetPlayerName)
                        {
                            targetPlayer = p;
                            break;
                        }
                    }

                    if (!targetPlayer)
                    {
                        std::cout << "Error: Invalid player name! Negotiation target must be a valid player.\n";
                        return;
                    }

                    player->getOrdersList()->ordersVector.push_back(new NegotiateOrder(player, targetPlayerName));
                }
                else if (orderType == "bomb")
                {
                    std::cout << "Enter the enemy territory to bomb: ";
                    std::getline(std::cin, targetTName);

                    player->getOrdersList()->ordersVector.push_back(new BombOrder(targetTName));
                }

                // Play the card
                c->play(*player->getOrdersList(), *deck, *player->getPlayerHand());
                break; // Exit the loop after playing the card
            }
        }

        if (!hasCard)
        {
            std::cout << "Error: You do not have a " << orderType << " card in your hand!\n";
        }
    }
    else
    {
        std::cout << "Error: Invalid order type. Valid options are: deploy, advance, airlift, blockade, negotiate, bomb, end turn.\n";
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
    static bool cardPlayed = false; // Track whether a card has been played this turn

    if (typeOfOrder == "deploy")
    {
        for (Territory *weakestTerritory : toDefend(player))
        {
            if (player->getNumArmies() > 0)
            {
                int armiesToDeploy = std::min(player->getNumArmies(), weakestTerritory->numberOfArmies + 1);
                player->getOrdersList()->ordersVector.push_back(new DeployOrder(player, weakestTerritory->name, armiesToDeploy));
                player->setNumArmies(player->getNumArmies() - armiesToDeploy);
            }
        }
    }
    else if (typeOfOrder == "advance")
    {
        std::vector<Territory *> defendableTerritories = toDefend(player);
        if (defendableTerritories.size() >= 2)
        {
            Territory *source = defendableTerritories.back();
            Territory *target = defendableTerritories.front();
            if (source != target && source->numberOfArmies > 1)
            {
                int armiesToAdvance = source->numberOfArmies - 1;
                player->getOrdersList()->ordersVector.push_back(new AdvanceOrder(player, nullptr, source->name, target->name, armiesToAdvance));
            }
        }
    }
    else if (typeOfOrder == "airlift" || typeOfOrder == "blockade" || typeOfOrder == "bomb" || typeOfOrder == "negotiate")
    {
        if (cardPlayed)
        {
            std::cout << "Error: A card has already been played this turn!\n";
            return;
        }

        bool hasCard = false;
        Card *cardToPlay = nullptr;
        for (Card *card : player->getPlayerHand()->handVector)
        {
            if (card->cardType == typeOfOrder)
            {
                hasCard = true;
                cardToPlay = card;
                break;
            }
        }

        if (!hasCard)
        {
            std::cout << "Error: Benevolent Player does not have a " << typeOfOrder << " card!\n";
            return;
        }

        if (typeOfOrder == "airlift")
        {
            Territory *source = player->getOwnedTerritories().front();
            Territory *target = toDefend(player).front();
            if (source != target && source->numberOfArmies > 1)
            {
                int armiesToAirlift = source->numberOfArmies - 1;
                player->getOrdersList()->ordersVector.push_back(new AirliftOrder(source->name, target->name, armiesToAirlift));
            }
        }
        else if (typeOfOrder == "blockade")
        {
            Territory *target = toDefend(player).front();
            player->getOrdersList()->ordersVector.push_back(new BlockadeOrder(target->name));
        }

        cardToPlay->play(*player->getOrdersList(), *deck, *player->getPlayerHand());
        cardPlayed = true; // Set the flag after playing the card
    }
    else
    {
        std::cout << "Error: Invalid order type \"" << typeOfOrder << "\".\n";
    }
}

std::vector<Territory *> CheaterPlayerStrategy::toDefend(Player *player)
{
    // Return all owned territories
    return player->getOwnedTerritories();
}

std::vector<Territory *> CheaterPlayerStrategy::toAttack(Player *player)
{
    // Identify all enemy territories adjacent to owned territories
    std::set<Territory *> adjacentEnemyTerritories;

    for (Territory *ownedTerritory : player->getOwnedTerritories())
    {
        for (const auto &adjacentPair : ownedTerritory->adjacentTerritories)
        {
            Territory *adjacentTerritory = adjacentPair.second;

            // Check if the adjacent territory is not owned by the player
            if (std::find(player->getOwnedTerritories().begin(), player->getOwnedTerritories().end(), adjacentTerritory) == player->getOwnedTerritories().end())
            {
                adjacentEnemyTerritories.insert(adjacentTerritory);
            }
        }
    }

    // Convert the set to a vector for easier handling
    return std::vector<Territory *>(adjacentEnemyTerritories.begin(), adjacentEnemyTerritories.end());
}

void CheaterPlayerStrategy::issueOrder(Player *player, const std::string &typeOfOrder, Deck *deck)
{
    if (typeOfOrder == "deploy")
    {
        // No deployment for the Cheater player
        std::cout << "Cheater Player: Skipping deploy phase.\n";
    }
    else if (typeOfOrder == "advance")
    {
        // Automatically conquer adjacent enemy territories
        std::vector<Territory *> territoriesToConquer = toAttack(player);
        std::cout << "Cheater Player: Automatically conquering adjacent territories.\n";

        for (Territory *enemyTerritory : territoriesToConquer)
        {
            // Find the current owner of the enemy territory
            Player *enemyPlayer = player->FindTerritoryOwner(enemyTerritory->name, *player->players);

            if (enemyPlayer)
            {
                // Remove the territory from the enemy player's ownership
                std::vector<Territory *> &enemyOwnedTerritories = enemyPlayer->getOwnedTerritories();
                enemyOwnedTerritories.erase(
                    std::remove(enemyOwnedTerritories.begin(), enemyOwnedTerritories.end(), enemyTerritory),
                    enemyOwnedTerritories.end());
            }

            // Add the territory to the Cheater player's ownership
            std::vector<Territory *> &cheaterOwnedTerritories = player->getOwnedTerritories();
            if (std::find(cheaterOwnedTerritories.begin(), cheaterOwnedTerritories.end(), enemyTerritory) == cheaterOwnedTerritories.end())
            {
                cheaterOwnedTerritories.push_back(enemyTerritory);
            }

            std::cout << "Cheater Player has conquered " << enemyTerritory->name << ".\n";
        }
    }
    else
    {
        std::cout << "Error: Invalid order type for Cheater Player. Only 'deploy' or 'advance' is allowed.\n";
    }
}
