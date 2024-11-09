#include <ctime>
#include <random>

#include "Orders.h"
#include "PlayerFiles/Player.h"

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
void Order::operator=(const Order &order) {
    if (this != &order) {
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
std::ostream& operator<<(std::ostream &COUT, const Order &ORDER) {
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
}

// ---------------------- Deploy Order ----------------------

/**
 * @brief Constructor for the DeployOrder class.
 * Initializes the orderType as "deploy".
 */
DeployOrder::DeployOrder() { orderType = "deploy"; validOrder = false;}

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
DeployOrder::DeployOrder(Player* p, const std::string tName, int armyDeployed) : player(p), territoryDeployName(tName), army(armyDeployed)
{
    // Set the order type to "deploy", indicating this is a deployment order
    orderType = "deploy";
    // Initialize the validOrder flag to false, meaning the order is not validated at creation
    validOrder = false;
}

std::ostream& operator<<(std::ostream &COUT, const DeployOrder &ORDER) {
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
    for(Territory* t : player->toDefend())
    {
        // If the player's owned territory matches the target territory name
        if(t->name == territoryDeployName)
        {
            // The target territory was found.
            tFound = true;

            // Check if the player has enough army units to deploy to the target territory.
            if(player->getNumArmies() >= army)
            {
                // Player has enough army units.
                enoughArmy = true;
            }

            // Exit the loop once the target territory is found.
            break;
        }
    }

    // If the target territory was not found in the player's owned territories, the order is invalid.
    if(!tFound)
    {
        // Mark the order as invalid.
        std::cout << "Order Invalid: Target territory not found in " << player->getPlayerName() << "'s owned territories.\n";
        validOrder = false;
    }
    // If the player does not have enough army units, the order is invalid.
    else if(!enoughArmy)
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
    // Validate the deploy order before execution.
    DeployOrder::validate();

    // If the order is valid, proceed with the execution.
    if (validOrder) 
    {
        // Call the base class execute method (Displaying the order type executing).
        Order::execute();

        for(Territory* t : player->toDefend())
        {
            // If the target territory is found in the player's owned territories
            if(t->name == territoryDeployName)
            {
                // Deploy the army units to the territory by adding the army units to the target territory.
                t->numberOfArmies += army;
                // Update the player's reinforcement pool to reflect the deployment.
                player->setNumArmies(player->getNumArmies() - army);
                // Notify that the deployment was successful.
                std::cout << "Successfully deployed " << army << " units to " << t->name << ".\n";
                // Exit the loop once the deployment is completed.
                break;
            }
        }
        
    } 
    // If the order is invalid, notify the player that the order will not be executed.
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
AdvanceOrder::AdvanceOrder(Player *p, const std::string sName, const std::string tName, int armyUnits) : 
player(p), territoryAdvanceSName(sName), territoryAdvanceTName(tName), army(armyUnits)
{
    // Sets the order type to "advance"
    orderType = "advance";
    // Initially invalid until validated
    validOrder = false;
}

std::ostream& operator<<(std::ostream &COUT, const AdvanceOrder &ORDER) {
    COUT << ORDER.orderType << ": " << "Move " << ORDER.army << " army units from territory " << ORDER.territoryAdvanceSName << " to territory " << ORDER.territoryAdvanceTName << ".\n";
    return COUT;
}

/**
 * @brief Validates the AdvanceOrder.
 * Displays a validation message.
 */
void AdvanceOrder::validate() 
{
    // Notify that the advance order is being validated.
    std::cout << "Validating advance order..." << std::endl;

    // Flag to track if source territory is found in player's owned territories
    bool sFound = false;
    // Flag to track if the target territory is adjacent to the source territory
    bool adjacent = false;
    // Pointer to source territory
    Territory* sourceT = nullptr;
    // Pointer to target territory
    Territory* targetT = nullptr;

    for(Territory* t : player->toDefend())
    {
        // Check if current territory matches the source territory name
        if(t->name == territoryAdvanceSName)
        {
            // Mark source territory as found
            sFound = true;
            // Set the source territory pointer to the found territory
            sourceT = t;
            // Exit the loop once the source territory is found
            break;
        }
    }

    // If the source territory is not found, mark the order as invalid and exit
    if(!sFound)
    {
        // Print an error message
        std::cout << "Order Invalid: Source territory not found in " << player->getPlayerName() << "'s owned territories.\n";
        // Set the order as invalid
        validOrder = false;
        // Exit the function early as the order cannot be validated further
        return;
    }

    // // Loop over adjacent territories of the source territory to check if the target territory is adjacent to the source territory
    for(const auto& pair : sourceT->adjacentTerritories)
    {
        // Check if current adjacent territory matches the target territory name
        if(pair.second->name == territoryAdvanceTName)
        {
            // Mark the target as adjacent if found
            adjacent = true;
            // Set the target territory pointer to the found territory
            targetT = pair.second;
            // Exit the loop once the target is found
            break;
        }
    }
    
    // If no adjacent territory matches the target, mark the order as invalid and exit
    if(!adjacent)
    {
        // Print an error message
        std::cout << "Order Invalid: Target territory is not found to source territory " << targetT->name << ".\n";
        // Set the order as invalid
        validOrder = false;
        // Exit the function early as the order cannot be validated further
        return;
    }

    // Check if the player has enough army units in the source territory to execute the order
    if(sourceT->numberOfArmies >= army)
    {
        // Mark the order as valid
        validOrder = true;
    }
    // If there are not enough armies in the source territory
    else
    {
        // Print an error message indicating insufficient armies
        std::cout << "Order Invalid: Number of army units to advance surpass the number of army units available in source territoty " << sourceT->name << ".\n";
        // Set the order as invalid
        validOrder = false;
    }
}

/**
 * @brief Executes the AdvanceOrder.
 * Validates and then performs the advancement if valid.
 */
void AdvanceOrder::execute() 
{
    AdvanceOrder::validate();

    if (validOrder) 
    {
        Order::execute();

        bool ownT = false;
        for(Territory* t : player->getToDefendTerritories())
        {
            if(t->name == territoryAdvanceSName)
            {
                t->numberOfArmies -= army;
            }

            if(t->name == territoryAdvanceTName)
            {
                ownT = true;
                t->numberOfArmies += army;
                std::cout << "Advancing troops from " << territoryAdvanceSName << " to defend " << territoryAdvanceTName << ".\n";
            }
        }

        if(!ownT)
        {
            for(Territory* t : player->getToAttackTerritories()) // TODO: Maybe change these vectors to maps to make easier to access
            {
                if(t->name == territoryAdvanceTName)
                {
                    std::cout << "Advancing troops from " << territoryAdvanceSName << " to attack " << territoryAdvanceTName << ".\n";

                    int attackingUnits = army;
                    int defendingUnits = t->numberOfArmies;

                    std::mt19937 rng(static_cast<unsigned>(std::time(0))); 
                    std::uniform_int_distribution<int> dist(1, 100);    

                    while(attackingUnits > 0 && defendingUnits > 0)
                    {
                        if(dist(rng) < 60)
                        {
                            defendingUnits--;
                        }

                        if(dist(rng) < 70)
                        {
                            attackingUnits--;
                        }
                    }

                    if(defendingUnits == 0)
                    {
                        t->numberOfArmies = attackingUnits;
                        // continue here    
                    }
                }
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

std::ostream& operator<<(std::ostream &COUT, const BombOrder &ORDER) {
    COUT << ORDER.orderType << std::endl;
    return COUT;
}

/**
 * @brief Validates the BombOrder.
 * Displays a validation message.
 */
void BombOrder::validate() {
    std::cout << "Validating bomb order..." << std::endl;
    // Assume validation logic here
}

/**
 * @brief Executes the BombOrder.
 * Validates and then performs the bombing if valid.
 */
void BombOrder::execute() {
    if (true /* assume valid */) {
        Order::execute();
        std::cout << "Bombing target." << std::endl;
    } else {
        std::cout << "Bomb order is invalid." << std::endl;
    }
}

// ---------------------- BlockadeOrder Class Implementation ----------------------

/**
 * @brief Constructor for the BlockadeOrder class.
 * Initializes the orderType as "blockade".
 */
BlockadeOrder::BlockadeOrder() {
    orderType = "blockade";
}

/**
 * Parameterized constructor for the BlockadeOrder class.
 * 
 * This constructor initializes the BlockadeOrder with the target territory for the blockade.
 * 
 * @param tBlockadeName The name of the territory to blockade.
 */
BlockadeOrder::BlockadeOrder(std::string tBlockadeName) : terriitoryBlockadeName(tBlockadeName)
{
    // Sets the order type to "blockade"
    orderType = "blockade";
    // Initially invalid until validated
    validOrder = false;
}

std::ostream& operator<<(std::ostream &COUT, const BlockadeOrder &ORDER) {
    COUT << ORDER.orderType << std::endl;
    return COUT;
}

/**
 * @brief Validates the BlockadeOrder.
 * Displays a validation message.
 */
void BlockadeOrder::validate() {
    std::cout << "Validating blockade order..." << std::endl;
    // Assume validation logic here
}

/**
 * @brief Executes the BlockadeOrder.
 * Validates and then initiates the blockade if valid.
 */
void BlockadeOrder::execute() {
    if (true /* assume valid */) {
        Order::execute();
        std::cout << "Initiating blockade." << std::endl;
    } else {
        std::cout << "Blockade order is invalid." << std::endl;
    }
}

// ---------------------- AirliftOrder Class Implementation ----------------------

/**
 * @brief Constructor for the AirliftOrder class.
 * Initializes the orderType as "airlift".
 */
AirliftOrder::AirliftOrder() {
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

std::ostream& operator<<(std::ostream &COUT, const AirliftOrder &ORDER) {
    COUT << ORDER.orderType << std::endl;
    return COUT;
}

/**
 * @brief Validates the AirliftOrder.
 * Displays a validation message.
 */
void AirliftOrder::validate() {
    std::cout << "Validating airlift order..." << std::endl;
    // Assume validation logic here
}

/**
 * @brief Executes the AirliftOrder.
 * Validates and then airlifts units if valid.
 */
void AirliftOrder::execute() {
    if (true /* assume valid */) {
        Order::execute();
        std::cout << "Airlifting units." << std::endl;
    } else {
        std::cout << "Airlift order is invalid." << std::endl;
    }
}

// ---------------------- NegotiateOrder Class Implementation ----------------------

/**
 * @brief Constructor for the NegotiateOrder class.
 * Initializes the orderType as "negotiate".
 */
NegotiateOrder::NegotiateOrder() {
    orderType = "negotiate";
}

/**
 * Parameterized constructor for the NegotiateOrder class.
 * 
 * This constructor initializes the NegotiateOrder with the target player for the negotiation.
 * 
 * @param pTargetName The name of the player to negotiate with.
 */
NegotiateOrder::NegotiateOrder(std::string pTargetNAme) : playerTargetName(pTargetNAme)
{
    // Sets the order type to "negotiate"
    orderType = "negotiate";
    // Initially invalid until validated
    validOrder = false;
}

std::ostream& operator<<(std::ostream &COUT, const NegotiateOrder &ORDER) {
    COUT << ORDER.orderType << std::endl;
    return COUT;
}

/**
 * @brief Validates the NegotiateOrder.
 * Displays a validation message.
 */
void NegotiateOrder::validate() {
    std::cout << "Validating negotiate order..." << std::endl;
    // Assume validation logic here
}

/**
 * @brief Executes the NegotiateOrder.
 * Validates and then negotiates peace if valid.
 */
void NegotiateOrder::execute() {
    if (true /* assume valid */) {
        Order::execute();
        std::cout << "Negotiating peace." << std::endl;
    } else {
        std::cout << "Negotiate order is invalid." << std::endl;
    }
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
OrdersList::OrdersList(const OrdersList &ordersListCopy) {
    ordersVector = ordersListCopy.ordersVector;
}

/**
 * @brief Assignment operator overload for the OrdersList class.
 * Assigns the ordersVector from another OrdersList instance.
 *
 * @param ordersList The OrdersList object to assign from.
 */
void OrdersList::operator=(const OrdersList &ordersList) {
    if (this != &ordersList) {
        ordersVector = ordersList.ordersVector;
    }
}

/**
 * @brief Adds an Order to the ordersVector.
 * Orders are added sequentially to the list.
 *
 * @param order A pointer to the Order object to add.
 */
void OrdersList::add(Order* order) {
    ordersVector.push_back(order);
}

/**
 * @brief Moves an Order within the list from one position to another.
 * The order positions are 1-based.
 *
 * @param orderPos The current position of the order to move (1-based index).
 * @param newOrderPos The new position for the order (1-based index).
 */
void OrdersList::move(int orderPos, int newOrderPos) {
    // Convert from 1-based to 0-based indexing
    int currentIndex = orderPos - 1;
    int newIndex = newOrderPos - 1;

    // Check if the current and new positions are valid (within the bounds of the ordersVector).
    if (currentIndex >= 0 && currentIndex < ordersVector.size() &&
        newIndex >= 0 && newIndex < ordersVector.size()) 
    {
        // Get the order to move by accessing it using the currentIndex.
        Order* orderToMove = ordersVector.at(currentIndex);

        // Prevent moving orders of type "deploy".
        if(orderToMove->orderType == "deploy" || ordersVector.at(newIndex)->orderType == "deploy")
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
void OrdersList::remove(int orderPos) {
    // Convert from 1-based to 0-based indexing
    int index = orderPos - 1;

    // Check if the index is within the valid range of ordersVector.
    if (index >= 0 && index < ordersVector.size()) 
    {
        // Check if the order at the given index is of type "deploy".
        if(ordersVector.at(index)->orderType == "deploy")
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
std::ostream& operator<<(std::ostream &COUT, const OrdersList &ORDERSLIST) {
    COUT << "Orders List:" << std::endl;
    for (int i = 0; i<ORDERSLIST.ordersVector.size(); i++) {
        COUT << i+1 << ". " << *ORDERSLIST.ordersVector[i] << std::endl;
    }
    return COUT;
}

/**
 * @brief Destructor for the OrdersList class.
 * Frees the memory allocated for each Order in the list.
 */
OrdersList::~OrdersList() {
    for (Order* order : ordersVector) { // Free up dynamically allocated Order instances in list.
        delete order;
    }
}

// <<<< OrdersList Class Definitions >>>>
