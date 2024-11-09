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

DeployOrder::DeployOrder(Player* p, const std::string tName, int armyDeployed) : player(p), territoryDeployName(tName), army(armyDeployed)
{
    orderType = "deploy";
    validOrder = false;
}

//void DeployOrder::print(size_t index) const
//{
//    std::cout << "Order " <<  index + 1 << " - Deploy Order: Deploy " << army << " army units to territory " << territoryDeployName << ".\n";
//}

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
    std::cout << "Validating deploy order..." << std::endl;
    
    bool tFound = false;
    bool enoughArmy = false;
    for(Territory* t : player->getToDefendTerritories())
    {
        if(t->name == territoryDeployName)
        {
            tFound = true;

            if(player->getNumArmies() >= army)
            {
                enoughArmy = true;
            }

            break;
        }
    }

    if(!tFound)
    {
        std::cout << "Order Invalid: Target territory not found in " << player->getPlayerName() << "'s owned territories.\n";
        validOrder = false;
    }
    else if(!enoughArmy)
    {
        std::cout << "Order Invalid: Not enough army units in " << player->getPlayerName() << "'s reinforcement pool to deploy.\n";
        validOrder = false;
    }
    else
    {
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

        for(Territory* t : player->getToDefendTerritories())
        {
            if(t->name == territoryDeployName)
            {
                t->numberOfArmies += army;
                player->setNumArmies(player->getNumArmies() - army);
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

AdvanceOrder::AdvanceOrder(Player *p, const std::string sName, const std::string tName, int armyUnits) : 
player(p), territoryAdvanceSName(sName), territoryAdvanceTName(tName), army(armyUnits)
{
    orderType = "advance";
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
    std::cout << "Validating advance order..." << std::endl;
    bool sFound = false;
    bool adjacent = false;
    Territory* sourceT = nullptr;
    Territory* targetT = nullptr;

    for(Territory* t : player->getToDefendTerritories())
    {
        if(t->name == territoryAdvanceSName)
        {
            sFound = true;
            sourceT = t;
            break;
        }
    }

    if(!sFound)
    {
        std::cout << "Order Invalid: Source territory not found in " << player->getPlayerName() << "'s owned territories.\n";
        validOrder = false;
        return;
    }

    for(const auto& pair : sourceT->adjacentTerritories)
    {
        if(pair.second->name == territoryAdvanceTName)
        {
            adjacent = true;
            targetT = pair.second;
            break;
        }
    }

    if(!adjacent)
    {
        std::cout << "Order Invalid: Target territory is not found to source territory " << targetT->name << ".\n";
        validOrder = false;
        return;
    }

    if(sourceT->numberOfArmies >= army)
    {
        validOrder = true;
    }
    else
    {
        std::cout << "Order Invalid: Number of army units to advance surpass the number of army units available in source territoty" << sourceT->name << ".\n";
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

    if (currentIndex >= 0 && currentIndex < ordersVector.size() &&
        newIndex >= 0 && newIndex < ordersVector.size()) 
    {
        Order* orderToMove = ordersVector.at(currentIndex);
        if(orderToMove->orderType == "deploy" || ordersVector.at(newIndex)->orderType == "deploy")
        {
            std::cerr << "Cannot move a Deploy order.\n";
        }
        else
        {
            ordersVector.erase(ordersVector.begin() + currentIndex);
            ordersVector.insert(ordersVector.begin() + newIndex, orderToMove);
        }
    } 
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

    if (index >= 0 && index < ordersVector.size()) 
    {
        if(ordersVector.at(index)->orderType == "deploy")
        {
            std::cerr << "Cannot remove a Deploy order.\n";
        }
        else
        {
            delete ordersVector.at(index); // Free the memory
            ordersVector.erase(ordersVector.begin() + index);
        }
    } 
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
