#include "PlayerFiles/PlayerStrategies.h"
#include "PlayerFiles/Player.h"
#include "GameEngineFiles/GameEngine.h"
#include "MapFiles/Map.h"
#include "CardsFiles/Cards.h"
#include "OrdersFiles/Orders.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <string>
#include <limits>

std::string toLower(const std::string &str)
{
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

bool PlayerStrategy::isIssuingOrders() const {
    return issuingOrders;
}

void PlayerStrategy::setIssuingOrders(bool state) {
    issuingOrders = state;
}

// ----- HumanPlayerStrategy Implementation -----
std::vector<Territory *> HumanPlayerStrategy::toDefend()
{
    // Set the player's owned territories as the territories to defend
    player->setToDefendTerritories(player->getOwnedTerritories());
    // Return the list of territories to defend
    return player->getToDefendTerritories();
}

std::vector<Territory *> HumanPlayerStrategy::toAttack()
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

void HumanPlayerStrategy::issueOrder(Deck *deck)
{
    std::string sourceTName, targetTName;
    int armies = 0;

    // Input for the type of order to issue
    std::string orderType;

    // Display available order types
    std::cout << "- Order Types -\n\n";
    std::cout << "\t- Deploy\n\t- Advance\n\t- Airlift (Use one Airlift card)\n\t- Bomb (Use one Bomb card)\n\t- Blockade (Use one Blockade card)\n\t- Negotiate (Use one Diplomacy card)\n\n";

    // Print out the player's hand
    std::cout << *(player->getPlayerHand()) << std::endl;

    // Prompt player for selection
    std::cout << "Please issue an order type: ";
    std::getline(std::cin, orderType);

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
                deck->deckVector.push_back(c);
                player->getPlayerHand()->handVector.erase(std::remove(player->getPlayerHand()->handVector.begin(),
                                                                      player->getPlayerHand()->handVector.end(), c),
                                                          player->getPlayerHand()->handVector.end());
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
std::vector<Territory *> BenevolentPlayerStrategy::toDefend()
{
    std::vector<Territory *> territories = player->getOwnedTerritories();
    std::sort(territories.begin(), territories.end(), [](Territory *a, Territory *b)
              {
                  return a->numberOfArmies < b->numberOfArmies; // Weakest first
              });
    return territories;
}

std::vector<Territory *> BenevolentPlayerStrategy::toAttack()
{
    return {}; // Benevolent players do not attack
}

void BenevolentPlayerStrategy::issueOrder(Deck *deck)
{
    if (!issuingOrders) {
        issuingOrders = true; // indicate to the caller that the player may continue issuing orders this turn
    }

    // Deploy armies to the weakest territories
    if (deploying) {
        int armies_available = player->getNumArmies();

        for (Territory *weakestTerritory : toDefend())
        {
            if (armies_available > 0)
            {
                // Deploy as many armies as possible to the weakest territory
                int armiesToDeploy = player->getNumArmies(); // TODO: maybe update to spread out the armies more at the start

                // Create a deploy order and add it to the player's orders list
                player->getOrdersList()->add(new DeployOrder(player, weakestTerritory->name, armiesToDeploy));
                armies_available -= armies_available;
            }
        }
        deploying = false;
        return;
    }

    if (advancing)
    {
        // Benevolent players do not attack but can advance armies to reinforce weaker territories
        std::vector<Territory *> defendableTerritories = player->toDefend();

        // Ensure we have at least two territories to work with
        if (defendableTerritories.size() >= 2)
        {
            // Sort territories from weakest to strongest
            Territory *target = defendableTerritories.front(); // Weakest
            Territory *source = nullptr;  // Strongest
            int maxArmies = 0;

            for (Territory* territory : defendableTerritories) {
                // Check if this territory is adjacent to the weakest territory
                if (territory->adjacentTerritories.count(target->name) > 0) {
                    if (territory->numberOfArmies > maxArmies) {
                        maxArmies = territory->numberOfArmies;
                        source = territory; // Get the strongest territory adjacent to target
                    }
                }
            }

            // Only move armies if the source territory has enough to spare
            if (source != target && source != nullptr && source->numberOfArmies > 1)
            {
                int armiesToAdvance = source->numberOfArmies - 1;

                // Create an advance order and add it to the player's orders list
                player->getOrdersList()->add(new AdvanceOrder(player, nullptr, source->name, target->name, armiesToAdvance));
            }
        }

        advancing = false;
        return;
    }

    // Play the airlift and/or blockcade cards to strengthen weak territories
    if (blockading || airlifting) {
        bool hasValidAirliftCard = false;
        bool hasValidBlockadeCard = false;
        for (Card *card : player->getPlayerHand()->handVector)
        {
            if (airlifting && toLower(card->cardType) == "airlift")
            {
                // Identify source and target territories for airlift
                std::vector<Territory *> defendableTerritories = toDefend();
                player->setOwnedTerritories(defendableTerritories);
                if (defendableTerritories.size() >= 2)
                {
                    Territory *source = defendableTerritories.back();
                    Territory *target = defendableTerritories.front();
                    if (source != target && source->numberOfArmies > 1)
                    {

                        int armiesToAirlift = source->numberOfArmies - 1;
                        auto airliftOrder = new AirliftOrder(source->name, target->name, armiesToAirlift);
                        airliftOrder->setPlayer(player);

                        player->getOrdersList()->add(airliftOrder);

                        // Play the card
                        deck->deckVector.push_back(card);
                        player->getPlayerHand()->handVector.erase(std::remove(player->getPlayerHand()->handVector.begin(),
                                                                              player->getPlayerHand()->handVector.end(), card),
                                                                  player->getPlayerHand()->handVector.end());
                        std::cout << "Benevolent Player airlifted " << armiesToAirlift << " units from "
                                  << source->name << " to " << target->name << ".\n";
                        hasValidAirliftCard = true;
                        return; // airlift as much as possible
                    }
                }
            }

            if (blockading && toLower(card->cardType) == "blockade")
            {
                // Blockade the weakest territory
                std::vector<Territory *> defendableTerritories = toDefend();
                if (!defendableTerritories.empty())
                {
                    Territory *target = defendableTerritories.front();
                    auto blockadeOrder = new BlockadeOrder(target->name);
                    blockadeOrder->setPlayer(player);

                    for (auto p : Player::players) {
                        if (dynamic_cast<NeutralPlayerStrategy*>(p->getStrategy()) != nullptr) {
                            blockadeOrder->neutralPlayer = p;
                            break;
                        }
                    }

                    if (blockadeOrder->neutralPlayer != nullptr) {
                        player->getOrdersList()->add(blockadeOrder);

                        // Play the card
                        deck->deckVector.push_back(card);
                        player->getPlayerHand()->handVector.erase(std::remove(player->getPlayerHand()->handVector.begin(),
                                                                              player->getPlayerHand()->handVector.end(), card),
                                                                  player->getPlayerHand()->handVector.end());
                        std::cout << "Benevolent Player used blockade on " << target->name << ".\n";
                        hasValidBlockadeCard = true;
                        blockading = false; // only blockade once
                        break;
                    }
                }
            }
        }

        if (!hasValidAirliftCard && !hasValidBlockadeCard) {
            airlifting = false;
            blockading = false;
        }
    } else {
        deploying = true;
        advancing = true;
        airlifting = true;
        blockading = true;

        issuingOrders = false;
    }
}

// ----- CheaterPlayerStrategy Implementation -----
std::vector<Territory *> CheaterPlayerStrategy::toDefend()
{
    // Return all owned territories
    return player->getOwnedTerritories();
}

std::vector<Territory *> CheaterPlayerStrategy::toAttack()
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

void CheaterPlayerStrategy::issueOrder(Deck *deck)
{
    // Identify all attackable territories
    std::vector<Territory *> attackableTerritories = toAttack();

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


// ----- AggressivePlayerStrategy Implementation -----

std::vector<Territory *> AggressivePlayerStrategy::toDefend()
{
    // Set the territories to defend
    player->setToDefendTerritories(player->getOwnedTerritories());
    // Return the territories to defend
    return player->getToDefendTerritories(); 
}

std::vector<Territory *> AggressivePlayerStrategy::toAttack()
{
    // Retrieve owned territories of the aggressive player
    std::vector<Territory *> ownedTerritories = player->getOwnedTerritories();
    // Initialize a vector that will contain all attackable enemy territories
    std::vector<Territory *> enemyTerritories;

    // For each owned territory
    for(Territory *ownedTerritory : ownedTerritories)
    {   
        // For each adjacent territory
        for(const auto &adjacentTerritory : ownedTerritory->adjacentTerritories)
        {   
            // Create an instance of the enemy/adjacent territory
            Territory *enemyTerritory = adjacentTerritory.second;
            // Check if enemy/adjacent territory is not one of ours
            if (std::find(ownedTerritories.begin(), ownedTerritories.end(), enemyTerritory) == ownedTerritories.end())
            {   
                // Append the enemy territory to be attacked
                enemyTerritories.push_back(enemyTerritory);
            }
        }
    } 

    // Update the set of territories that an aggressive player can attack
    player->setToAttackTerritories(enemyTerritories);
    // Return the set of territories that an aggressive player can attack
    return player->getToAttackTerritories();

}


void AggressivePlayerStrategy::issueOrder(Deck *deck)
{
    if (!issuingOrders) {
        issuingOrders = true; // indicate to the caller that the player may continue issuing orders this turn
    }

    if (deploying)
    {
        int highestArmy = 0;
        Territory *strongestTerritory;
        // Deploy armies to the strongest territories
        for (Territory *strongestTerritories : toDefend())
        {
            // Find the territory with the strongest (highest army)
            if(strongestTerritories->numberOfArmies >= highestArmy)
            {
                highestArmy = strongestTerritories->numberOfArmies;
                strongestTerritory =  strongestTerritories;
            }
        }

        // Create a deploy order to be added to the aggressive player's order list
        player->getOrdersList()->add(new DeployOrder(player, strongestTerritory->name, player->reinforcement_units));

        deploying = false;
        return;
    }

    if (advancing)
    {
        // Try and attack each adjacent enemy territory
        for (Territory *enemyTerritory : player->toAttack())
        {   
            int maxArmy = 0;
            std::string attackingTerritory;
            // Find the owned territory with the largest army
            for(Territory *ownedTerritory : player->getOwnedTerritories())
            {
                // Check if this territory is adjacent
                if (ownedTerritory->adjacentTerritories.count(enemyTerritory->name) > 0) {
                    if (ownedTerritory->numberOfArmies > maxArmy) {
                        maxArmy = ownedTerritory->numberOfArmies;
                        attackingTerritory = ownedTerritory->name;
                    }
                }
            }

            if (!attackingTerritory.empty() && maxArmy > 0) {
                Player *enemyPlayer = player->FindTerritoryOwner(enemyTerritory->name);
                player->getOrdersList()->ordersVector.push_back(
                        new AdvanceOrder(player, enemyPlayer, attackingTerritory, enemyTerritory->name, maxArmy));
                advancing = false;
                break;
            }
        }
    }

    if (bombing) {
        bool hasBombCard = false;

        for (Card *card : player->getPlayerHand()->handVector) {
            if (toLower(card->cardType) == "bomb") {
                int highestArmy = 0;
                Territory *strongestTerritory;
                // Deploy armies to the strongest territories
                for (Territory *strongestTerritories: player->getOwnedTerritories()) {
                    if (player->getNumArmies() > 0) {
                        // Find the territory with the strongest (highest army)
                        if (strongestTerritories->numberOfArmies >= highestArmy) {
                            highestArmy = strongestTerritories->numberOfArmies;
                            strongestTerritory = strongestTerritories;
                        }
                    }
                }

                // Create a deploy order to be added to the aggressive player's order list
                player->getOrdersList()->add(new BombOrder(player, strongestTerritory->name));

                // Play the card
                deck->deckVector.push_back(card);
                player->getPlayerHand()->handVector.erase(std::remove(player->getPlayerHand()->handVector.begin(),
                                                                      player->getPlayerHand()->handVector.end(), card),
                                                          player->getPlayerHand()->handVector.end());

                hasBombCard = true;
            }
        }

        if (!hasBombCard) {
            bombing = false;
        }
        return;
    } else {
        deploying = true;
        advancing = true;
        bombing = true;

        issuingOrders = false;
    }
}


// ----- NeutralPlayerStrategy Implementation -----


std::vector<Territory *> NeutralPlayerStrategy::toDefend()
{
    // Set the territories to defend
    player->setToDefendTerritories(player->getOwnedTerritories());
    // Return the territories to defend
    return player->getToDefendTerritories(); 
}

std::vector<Territory *> NeutralPlayerStrategy::toAttack()
{
    return {}; // Neutral players do not attack 
}

void NeutralPlayerStrategy::issueOrder(Deck *deck)
{
    // Neutral players do not issue any orders
    std::cout << "Neutral Player do not issue any orders" << std::endl;
}
