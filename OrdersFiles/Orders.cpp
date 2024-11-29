#include <ctime>
#include <random>
#include <algorithm>
#include <sstream>
#include "Orders.h"
#include "PlayerFiles/Player.h"
#include "GameEngineFiles/GameEngine.h"

std::unordered_map<std::string, std::string> NegotiateOrder::negotiatedPlayers;

/*
Note that the orders’ actions do not need to be implemented at this point. Invalid
orders can be created and put in the list, but their execution will not result in any action.
*/

// <<<< Order Class Definitions >>>>

// ---------------------- Order Class Implementation ----------------------

/**
 * @brief Default constructor for the Order class.
 * Initializes the orderType as "unknown".
 */
Order::Order() : orderType("unknown") {}

/**
 * @brief Copy constructor for the Order class.
 * Copies the orderType from another Order instance.
 *
 * @param orderCopy The Order object to copy from.
 */
Order::Order(const Order &orderCopy) : orderType(orderCopy.orderType) {}

/**
 * @brief Assignment operator overload for the Order class.
 * Assigns the orderType from another Order instance.
 *
 * @param order The Order object to assign from.
 */
void Order::operator=(const Order &order)
{
    if (this != &order)
    {
        orderType = order.orderType;
    }
}

/**
 * @brief Stream insertion operator overload for the Order class.
 * Outputs the orderType to an output stream.
 *
 * @param COUT Output stream object.
 * @param ORDER The Order object to display.
 * @return std::ostream& The modified output stream.
 */
std::ostream &operator<<(std::ostream &COUT, const Order &ORDER)
{
    COUT << "Order: " << ORDER.orderType;
    return COUT;
}

/**
 * @brief Executes the Order.
 * Displays a message if executed.
 */
void Order::execute()
{
    std::cout << "Executing " << orderType << " order." << std::endl;
    notify(this);
}

std::string Order::stringToLog() const
{
    std::stringstream SS;
    SS << "Order Executed: " << *this;
    return SS.str();
}

// ---------------------- Deploy Order ----------------------

/**
 * @brief Constructor for the DeployOrder class.
 * Initializes the orderType as "deploy".
 */
DeployOrder::DeployOrder()
{
    orderType = "deploy";
    validOrder = false;
}

/**
 * Constructor for the DeployOrder class.
 *
 * This constructor initializes a deploy order, which is used to deploy a specified number of army units to a given territory.
 * It sets the associated player, the name of the territory where the army is being deployed, and the number of armies being deployed.
 * Additionally, it sets the `orderType` to "deploy" and initializes the `validOrder` flag to `false`, indicating that the order is not yet validated.
 *
 * @param p The player who issued the order.
 * @param tName The name of the territory where the army units are being deployed.
 * @param armyDeployed The number of army units being deployed to the territory.
 */
DeployOrder::DeployOrder(Player *p, const std::string tName, int armyDeployed)
    : player(p), territoryDeployName(tName), army(armyDeployed)
{
    orderType = "deploy";
    validOrder = false;
}

std::ostream &operator<<(std::ostream &COUT, const DeployOrder &ORDER)
{
    COUT << ORDER.orderType << ": " << "Deploy " << ORDER.army << " army units to territory " << ORDER.territoryDeployName << ".\n";
    return COUT;
}

/**
 * @brief Validates the DeployOrder.
 * Displays a validation message.
 */
void DeployOrder::validate()
{
    // Notify that the deploy order is being validated.
    std::cout << "Validating deploy order..." << std::endl;

    // Flag indicating if the target territory was found in the player's owned territories.
    bool tFound = false;
    // Flag indicating if the player has enough army units to deploy.
    bool enoughArmy = false;
    for (Territory *t : player->toDefend())
    {
        // If the player's owned territory matches the target territory name
        if (GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryDeployName))
        {
            // The target territory was found.
            tFound = true;

            // Check if the player has enough army units to deploy to the target territory.
            if (player->getNumArmies() >= army)
            {
                // Player has enough army units.
                enoughArmy = true;
            }

            // Exit the loop once the target territory is found.
            break;
        }
    }

    // If the target territory was not found in the player's owned territories, the order is invalid.
    if (!tFound)
    {
        // Mark the order as invalid.
        std::cout << "Order Invalid: Target territory not found in " << player->getPlayerName() << "'s owned territories.\n";
        validOrder = false;
    }
    // If the player does not have enough army units, the order is invalid.
    else if (!enoughArmy)
    {
        // Mark the order as invalid.
        std::cout << "Order Invalid: Not enough army units in " << player->getPlayerName() << "'s reinforcement pool to deploy.\n";
        validOrder = false;
    }
    // If the target territory is found and the player has enough army units, the order is valid.
    else
    {
        // Mark the order as valid.
        validOrder = true;
    }
}

/**
 * @brief Executes the DeployOrder.
 * Validates and then performs the deployment if valid.
 */
void DeployOrder::execute()

{
    DeployOrder::validate();

    if (validOrder)
    {
        Order::execute();

        for (Territory *t : player->toDefend())
        {
            if (GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryDeployName))
            {
                t->numberOfArmies += army;                           // Add the armies to the target territory
                player->setNumArmies(player->getNumArmies() - army); // Deduct armies from reinforcement pool
                std::cout << "Successfully deployed " << army << " units to " << t->name << ".\n";
                break;
            }
        }
    }
    else
    {
        std::cout << "Deploy order is invalid and will not be executed.\n";
    }
}
// ---------------------- Advance Order ----------------------

/**
 * @brief Constructor for the AdvanceOrder class.
 * Initializes the orderType as "advance".
 */
AdvanceOrder::AdvanceOrder() { orderType = "advance"; }

/**
 * Parameterized constructor for the AdvanceOrder class.
 * Initializes the order with player, source territory name, target territory name, and the number of army units.
 *
 * @param p A pointer to the player who is making the order.
 * @param sName The name of the source territory from which the army is advancing.
 * @param tName The name of the target territory to which the army is advancing.
 * @param armyUnits The number of army units involved in the advance order.
 */
AdvanceOrder::AdvanceOrder(Player *p, Player *enemyP, const std::string sName, const std::string tName, int armyUnits) : player(p), enemyPlayer(enemyP), territoryAdvanceSName(sName), territoryAdvanceTName(tName), army(armyUnits)
{
    // Sets the order type to "advance"
    orderType = "advance";
    // Initially invalid until validated
    validOrder = false;
}

std::ostream &operator<<(std::ostream &COUT, const AdvanceOrder &ORDER)
{
    COUT << ORDER.orderType << ": " << "Move " << ORDER.army << " army units from territory " << ORDER.territoryAdvanceSName << " to territory " << ORDER.territoryAdvanceTName << ".\n";
    return COUT;
}

/**
 * @brief Validates the AdvanceOrder.
 * Displays a validation message.
 */
void AdvanceOrder::validate()
{
    std::cout << "Validating advance order: " << territoryAdvanceSName << " -> " << territoryAdvanceTName
              << " with " << army << " armies.\n";

    bool sFound = false;
    bool adjacent = false;
    Territory *sourceT = nullptr;
    Territory *targetT = nullptr;

    // Check source territory
    for (Territory *t : player->getOwnedTerritories())
    {
        if (GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryAdvanceSName))
        {
            sFound = true;
            sourceT = t;
            break;
        }
    }

    if (!sFound)
    {
        std::cout << "Order Invalid: Source territory " << territoryAdvanceSName << " not found.\n";
        validOrder = false;
        return;
    }

    // Check adjacency
    for (const auto &pair : sourceT->adjacentTerritories)
    {
        if (GameEngine::toLowerCase(pair.second->name) == GameEngine::toLowerCase(territoryAdvanceTName))
        {
            adjacent = true;
            targetT = pair.second;
            break;
        }
    }

    if (!adjacent)
    {
        std::cout << "Order Invalid: Target territory " << territoryAdvanceTName << " is not adjacent to source.\n";
        validOrder = false;
        return;
    }

    // Validate army count
    if (sourceT->numberOfArmies >= army)
    {
        validOrder = true;
    }
    else
    {
        std::cout << "Order Invalid: Not enough armies in source territory " << territoryAdvanceSName << ".\n";
        validOrder = false;
    }
}

/**
 * @brief Executes the AdvanceOrder.
 * Moves or attacks armies based on the target's ownership if the order is valid.
 */
void AdvanceOrder::execute()
{
    validate();

    if (validOrder)
    {
        // Check if negotiation prevents the attack
        if (NegotiateOrder::negotiatedPlayers.count(player->getPlayerName()) > 0 &&
            NegotiateOrder::negotiatedPlayers.at(player->getPlayerName()) == enemyPlayer->getPlayerName())
        {
            std::cout << "Advance order prevented due to active negotiation between "
                      << player->getPlayerName() << " and " << enemyPlayer->getPlayerName() << ".\n";
            return; // Exit without executing the attack
        }

        Order::execute();

        // Proceed with the usual advance order logic
        Territory *sourceT = nullptr;
        for (Territory *t : player->getOwnedTerritories())
        {
            if (GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryAdvanceSName))
            {
                sourceT = t;
                sourceT->numberOfArmies -= army; // Deduct armies
                break;
            }
        }

        if (!sourceT)
        {
            std::cerr << "Error: Source territory not found.\n";
            return;
        }

        bool targetOwned = false;
        Territory *targetT = nullptr;
        for (Territory *t : player->getOwnedTerritories())
        {
            if (GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryAdvanceTName))
            {
                targetOwned = true;
                t->numberOfArmies += army; // Add armies
                std::cout << "Advanced " << army << " units from " << territoryAdvanceSName
                          << " to defend " << territoryAdvanceTName << ".\n";
                break;
            }
        }

        // If target is not owned, proceed with attack logic
        if (!targetOwned)
        {
            // Find the target territory among adjacent ones
            for (const auto &pair : sourceT->adjacentTerritories)
            {
                if (GameEngine::toLowerCase(pair.second->name) == GameEngine::toLowerCase(territoryAdvanceTName))
                {
                    targetT = pair.second;
                    break;
                }
            }

            if (!targetT)
            {
                std::cerr << "Error: Target territory is not adjacent to source territory.\n";
                return;
            }

            int attackingUnits = army;
            int defendingUnits = targetT->numberOfArmies;

            std::mt19937 rng(static_cast<unsigned>(std::time(0)));
            std::uniform_int_distribution<int> dist(1, 100);

            // Battle loop
            while (attackingUnits > 0 && defendingUnits > 0)
            {
                if (dist(rng) < 60)
                    defendingUnits--;
                if (dist(rng) < 70)
                    attackingUnits--;
                if (attackingUnits == 0 || defendingUnits == 0)
                    break; // Exit loop when one side loses
            }

            if (defendingUnits == 0)
            {
                targetT->numberOfArmies = attackingUnits;
                std::cout << "Attack successful: " << territoryAdvanceTName
                          << " conquered with " << attackingUnits << " remaining units.\n";
                player->getOwnedTerritories().push_back(targetT);

                if (enemyPlayer != nullptr)
                {
                    enemyPlayer->getOwnedTerritories().erase(std::remove(enemyPlayer->getOwnedTerritories().begin(),
                                                                         enemyPlayer->getOwnedTerritories().end(), targetT),
                                                             enemyPlayer->getOwnedTerritories().end());
                }

                if (player->deck != nullptr)
                {
                    player->deck->draw(*player->getPlayerHand());
                }
            }
            else
            {
                targetT->numberOfArmies = defendingUnits;
                std::cout << "Attack failed: " << territoryAdvanceTName
                          << " defended with " << defendingUnits << " units remaining.\n";
            }
        }
    }
    else
    {
        std::cout << "Advance order is invalid and will not be executed.\n";
    }
}

// ---------------------- Bomb Order ----------------------

/**
 * @brief Constructor for the BombOrder class.
 * Initializes the orderType as "bomb".
 */
BombOrder::BombOrder() { orderType = "bomb"; }

/**
 * Parameterized constructor for the BombOrder class.
 *
 * This constructor initializes the BombOrder with the target territory for the bomb.
 *
 * @param tBombName The name of the territory to bomb.
 */
BombOrder::BombOrder(std::string tBombName) : territoryBombName(tBombName)
{
    // Sets the order type to "bomb"
    orderType = "bomb";
    // Initially invalid until validated
    validOrder = false;
}

std::ostream &operator<<(std::ostream &COUT, const BombOrder &ORDER)
{
    COUT << ORDER.orderType << std::endl;
    return COUT;
}
void BombOrder::setPlayer(Player *p)
{
    player = p;
}

/**
 * @brief Validates the BombOrder.
 * Displays a validation message.
 */
void BombOrder::validate()
{
    std::cout << "Validating bomb order...\n";

    if (!player)
    {
        std::cout << "Order Invalid: Issuing player not specified.\n";
        validOrder = false;
        return;
    }

    bool targetIsEnemy = true; // Assume target is enemy until proven otherwise
    bool hasAdjacentTerritory = false;

    // Check if the target territory is not owned by the player
    for (Territory *t : player->getOwnedTerritories())
    {
        if (GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryBombName))
        {
            std::cout << "Order Invalid: Cannot bomb own territory " << territoryBombName << ".\n";
            validOrder = false;
            return;
        }

        // Check if any of the player's territories are adjacent to the target
        for (const auto &adjPair : t->adjacentTerritories)
        {
            if (GameEngine::toLowerCase(adjPair.second->name) == GameEngine::toLowerCase(territoryBombName))
            {
                hasAdjacentTerritory = true;
                break;
            }
        }

        if (hasAdjacentTerritory)
        {
            break; // Exit the loop if we found an adjacent territory
        }
    }

    if (!hasAdjacentTerritory)
    {
        std::cout << "Order Invalid: Target territory " << territoryBombName
                  << " is not adjacent to any territory owned by " << player->getPlayerName() << ".\n";
        validOrder = false;
        return;
    }

    if (!targetIsEnemy)
    {
        std::cout << "Order Invalid: Target territory not found among enemy territories.\n";
        validOrder = false;
    }
    else
    {
        validOrder = true;
    }
}

/**
 * @brief Executes the BombOrder.
 * Halves the army units in the target territory if the order is valid.
 */
void BombOrder::execute()
{
    validate();

    if (validOrder)
    {
        Order::execute();

        // Assuming the bomb simply halves the army in the target territory
        for (Territory *t : player->toAttack())
        {
            if (GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryBombName))
            {
                t->numberOfArmies /= 2; // Halve the armies
                std::cout << "Bombing " << territoryBombName << ": Armies reduced to " << t->numberOfArmies << ".\n";
                break;
            }
        }
    }
    else
    {
        std::cout << "Bomb order is invalid and will not be executed.\n";
    }
}

// ---------------------- BlockadeOrder Class Implementation ----------------------

/**
 * @brief Constructor for the BlockadeOrder class.
 * Initializes the orderType as "blockade".
 */
void BlockadeOrder::setPlayer(Player *p) { player = p; }

BlockadeOrder::BlockadeOrder()
{
    orderType = "blockade";
}

/**
 * Parameterized constructor for the BlockadeOrder class.
 *
 * This constructor initializes the BlockadeOrder with the target territory for the blockade.
 *
 * @param tBlockadeName The name of the territory to blockade.
 */
BlockadeOrder::BlockadeOrder(std::string tBlockadeName) : territoryBlockadeName(tBlockadeName)
{
    // Sets the order type to "blockade"
    orderType = "blockade";
    // Initially invalid until validated
    validOrder = false;
}

std::ostream &operator<<(std::ostream &COUT, const BlockadeOrder &ORDER)
{
    COUT << ORDER.orderType << std::endl;
    return COUT;
}

/**
 * @brief Validates the BlockadeOrder.
 * Displays a validation message.
 */
void BlockadeOrder::validate()
{
    std::cout << "Validating blockade order...\n";

    if (!player)
    {
        std::cout << "Order Invalid: Issuing player not specified.\n";
        validOrder = false;
        return;
    }

    bool targetIsOwned = false;

    // Check if the target territory is owned by the player
    for (Territory *t : player->getOwnedTerritories())
    {
        if (GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryBlockadeName))
        {
            targetIsOwned = true;
            break;
        }
    }

    if (!targetIsOwned)
    {
        std::cout << "Order Invalid: Target territory " << territoryBlockadeName << " is not owned by "
                  << player->getPlayerName() << ".\n";
        validOrder = false;
    }
    else
    {
        validOrder = true;
    }
}

/**
 * @brief Executes the BlockadeOrder.
 * Triples the army units in the target territory if the order is valid,
 * and removes the territory from the player's list of owned territories.
 */
void BlockadeOrder::execute()
{
    validate();

    if (validOrder)
    {
        Order::execute();

        auto &ownedTerritories = player->getOwnedTerritories();

        // Find the target territory in the player's owned territories
        auto it = std::find_if(ownedTerritories.begin(), ownedTerritories.end(),
                               [this](Territory *t)
                               { return GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryBlockadeName); });

        if (it != ownedTerritories.end())
        {
            Territory *targetTerritory = *it;
            targetTerritory->numberOfArmies *= 3; // Triple the armies in the territory

            // Remove the territory from the current player's list
            ownedTerritories.erase(it);

            // Transfer ownership to the Neutral player
            // Assuming `neutralPlayer` is a global or accessible instance of the Neutral player
            neutralPlayer->getOwnedTerritories().push_back(targetTerritory);

            std::cout << "Blockading " << territoryBlockadeName << ": Armies increased to "
                      << targetTerritory->numberOfArmies
                      << " and territory is now owned by the Neutral player.\n";
        }
    }
    else
    {
        std::cout << "Blockade order is invalid and will not be executed.\n";
    }
}

// ---------------------- AirliftOrder Class Implementation ----------------------

/**
 * @brief Constructor for the AirliftOrder class.
 * Initializes the orderType as "airlift".
 */
void AirliftOrder::setPlayer(Player *p) { player = p; }

AirliftOrder::AirliftOrder()
{
    orderType = "airlift";
}

/**
 * Parameterized constructor for the AirliftOrder class.
 *
 * This constructor initializes the AirliftOrder with the source and target territories
 * and the number of army units to airlift.
 *
 * @param airSName The name of the source territory from which the army units will be airlifted.
 * @param airTName The name of the target territory to which the army units will be airlifted.
 * @param units The number of army units to airlift.
 */
AirliftOrder::AirliftOrder(std::string airSName, std::string airTName, int units) : territoryAirliftSName(airSName), territoryAirliftTName(airTName), army(units)
{
    // Sets the order type to "airlift"
    orderType = "airlift";
    // Initially invalid until validated
    validOrder = false;
}

std::ostream &operator<<(std::ostream &COUT, const AirliftOrder &ORDER)
{
    COUT << ORDER.orderType << std::endl;
    return COUT;
}

/**
 * @brief Validates the AirliftOrder.
 * Displays a validation message.
 */
void AirliftOrder::validate()
{
    std::cout << "Validating airlift order...\n";
    for (Territory *t : player->getOwnedTerritories())
    {

        std::cout << t->name;
    }

    if (!player)
    {
        std::cout << "Order Invalid: Issuing player not specified.\n";
        validOrder = false;
        return;
    }

    Territory *sourceT = nullptr;
    Territory *targetT = nullptr;

    // Find source and target territories in the player's owned territories
    for (Territory *t : player->getOwnedTerritories())
    {
        if (GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryAirliftSName))
        {
            sourceT = t;
        }
        else if (GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryAirliftTName))
        {
            targetT = t;
        }

        // Exit loop early if both territories are found
        if (sourceT && targetT)
        {
            break;
        }
    }

    if (!sourceT)
    {
        std::cout << "Order Invalid: Source territory not found in " << player->getPlayerName() << "'s owned territories.\n";
        validOrder = false;
    }
    else if (!targetT)
    {
        std::cout << "Order Invalid: Target territory not found in " << player->getPlayerName() << "'s owned territories.\n";
        validOrder = false;
    }
    else if (sourceT->numberOfArmies < army)
    {
        std::cout << "Order Invalid: Not enough army units in source territory " << sourceT->name << " for airlift.\n";
        validOrder = false;
    }
    else
    {
        validOrder = true;
    }
}

/**
 * @brief Executes the AirliftOrder.
 * Transfers army units from source to target territory if the order is valid.
 */
void AirliftOrder::execute()
{
    validate();

    if (validOrder)
    {
        Order::execute();

        // Transfer the specified army units
        for (Territory *t : player->getOwnedTerritories())
        {
            if (GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryAirliftSName))
            {
                t->numberOfArmies -= army; // Deduct armies from source
            }
            else if (GameEngine::toLowerCase(t->name) == GameEngine::toLowerCase(territoryAirliftTName))
            {
                t->numberOfArmies += army; // Add armies to target
                std::cout << "Airlifting " << army << " units from " << territoryAirliftSName
                          << " to " << territoryAirliftTName << ".\n";
            }
        }
    }
    else
    {
        std::cout << "Airlift order is invalid and will not be executed.\n";
    }
}

// ---------------------- NegotiateOrder Class Implementation ----------------------

/**
 * @brief Constructor for the NegotiateOrder class.
 * Initializes the orderType as "negotiate".
 */
NegotiateOrder::NegotiateOrder()
{
    orderType = "negotiate";
}
void NegotiateOrder::setPlayer(Player *p)
{
    player = p;
}

/**
 * Parameterized constructor for the NegotiateOrder class.
 *
 * This constructor initializes the NegotiateOrder with the issuing player and the target player for negotiation.
 *
 * @param p The player issuing the negotiate order.
 * @param pTargetName The name of the player to negotiate with.
 */
NegotiateOrder::NegotiateOrder(Player *p, std::string pTargetName)
    : player(p), playerTargetName(std::move(pTargetName))
{
    orderType = "negotiate";
    validOrder = false; // Not validated until `validate()` is called.
}

/**
 * @brief Validates the NegotiateOrder.
 * Ensures both players are specified and are not the same.
 */
void NegotiateOrder::validate()
{
    std::cout << "Validating negotiate order...\n";

    if (!player || player->getPlayerName() == playerTargetName)
    {
        std::cout << "Order Invalid: Negotiation requires two distinct players.\n";
        validOrder = false;
        return;
    }

    validOrder = true;
}

/**
 * @brief Executes the NegotiateOrder.
 * Marks the negotiation as successful, preventing attacks between the two players.
 */
void NegotiateOrder::execute()
{
    validate();

    if (validOrder)
    {
        Order::execute();

        // Placeholder for negotiation logic enforcement
        // This would ideally be checked in the main game loop to prevent attacks between negotiated players

        std::cout << "Negotiation order executed between " << player->getPlayerName()
                  << " and " << playerTargetName << ". Attacks between these players are now prevented.\n";

        // Notify the game engine or a higher-level mechanism that the players are negotiating
        negotiatedPlayers[player->getPlayerName()] = playerTargetName;
        negotiatedPlayers[playerTargetName] = player->getPlayerName();
    }
    else
    {
        std::cout << "Negotiate order is invalid and will not be executed.\n";
    }
}

NegotiateOrder::NegotiateOrder(std::string pTargetNAme) : playerTargetName(pTargetNAme)
{
}

// ---------------------- OrdersList Class Implementation ----------------------

/**
 * @brief Default constructor for the OrdersList class.
 */
OrdersList::OrdersList() {}

/**
 * @brief Copy constructor for the OrdersList class.
 * Copies the ordersVector from another OrdersList instance.
 *
 * @param ordersListCopy The OrdersList object to copy from.
 */
OrdersList::OrdersList(const OrdersList &ordersListCopy)
{
    ordersVector = ordersListCopy.ordersVector;
}

/**
 * @brief Assignment operator overload for the OrdersList class.
 * Assigns the ordersVector from another OrdersList instance.
 *
 * @param ordersList The OrdersList object to assign from.
 */
void OrdersList::operator=(const OrdersList &ordersList)
{
    if (this != &ordersList)
    {
        ordersVector = ordersList.ordersVector;
    }
}

/**
 * @brief Adds an Order to the ordersVector.
 * Orders are added sequentially to the list.
 *
 * @param order A pointer to the Order object to add.
 */
void OrdersList::add(Order *order)
{
    ordersVector.push_back(order);
    notify(this);
}

/**
 * @brief Moves an Order within the list from one position to another.
 * The order positions are 1-based.
 *
 * @param orderPos The current position of the order to move (1-based index).
 * @param newOrderPos The new position for the order (1-based index).
 */
void OrdersList::move(int orderPos, int newOrderPos)
{
    // Convert from 1-based to 0-based indexing
    int currentIndex = orderPos - 1;
    int newIndex = newOrderPos - 1;

    // Check if the current and new positions are valid (within the bounds of the ordersVector).
    if (currentIndex >= 0 && currentIndex < ordersVector.size() &&
        newIndex >= 0 && newIndex < ordersVector.size())
    {
        // Get the order to move by accessing it using the currentIndex.
        Order *orderToMove = ordersVector.at(currentIndex);

        // Prevent moving orders of type "deploy".
        if (orderToMove->orderType == "deploy" || ordersVector.at(newIndex)->orderType == "deploy")
        {
            // If the order or the order at the target position is of type "deploy", print an error.
            std::cerr << "Cannot move a Deploy order.\n";
        }
        // Erase the order from its current position and insert it at the new position.
        else
        {
            // Remove the order at the current position (currentIndex).
            ordersVector.erase(ordersVector.begin() + currentIndex);
            // Insert the order at the new position (newIndex).
            ordersVector.insert(ordersVector.begin() + newIndex, orderToMove);
        }
    }
    // If the indices are out of range, print an error.
    else
    {
        std::cerr << "Invalid positions for move operation." << std::endl;
    }
}

/**
 * @brief Removes an Order from the list at the specified position.
 * The order position is 1-based. The memory allocated for the order is freed.
 *
 * @param orderPos The position of the order to remove (1-based index).
 */
void OrdersList::remove(int orderPos)
{
    // Convert from 1-based to 0-based indexing
    int index = orderPos - 1;

    // Check if the index is within the valid range of ordersVector.
    if (index >= 0 && index < ordersVector.size())
    {
        // Check if the order at the given index is of type "deploy".
        if (ordersVector.at(index)->orderType == "deploy")
        {
            // Print an error message if the order type is "deploy".
            std::cerr << "Cannot remove a Deploy order.\n";
        }
        // If the order is not of type "deploy", proceed to remove the order.
        else
        {
            // Free the memory allocated for the order at the specified index.
            delete ordersVector.at(index);
            // Erase the order from the vector at the specified index.
            ordersVector.erase(ordersVector.begin() + index);
        }
    }
    // Print an error message if the index is out of bounds.
    else
    {
        std::cerr << "Invalid position for remove operation." << std::endl;
    }
}

/**
 * @brief Stream insertion operator overload for the OrdersList class.
 * Outputs the list of orders with their positions to an output stream.
 *
 * @param COUT Output stream object.
 * @param ORDERSLIST The OrdersList object to display.
 * @return std::ostream& The modified output stream.
 */
std::ostream &operator<<(std::ostream &COUT, const OrdersList &ORDERSLIST)
{
    COUT << "Orders List:" << std::endl;
    for (int i = 0; i < ORDERSLIST.ordersVector.size(); i++)
    {
        COUT << i + 1 << ". " << *ORDERSLIST.ordersVector[i] << std::endl;
    }
    return COUT;
}

/**
 * @brief Destructor for the OrdersList class.
 * Frees the memory allocated for each Order in the list.
 */
OrdersList::~OrdersList()
{
    for (Order *order : ordersVector)
    { // Free up dynamically allocated Order instances in list.
        delete order;
    }
}

std::string OrdersList::stringToLog() const
{
    std::stringstream SS;
    SS << "Order Issued: " << *ordersVector.back();
    return SS.str();
}

// <<<< OrdersList Class Definitions >>>>
