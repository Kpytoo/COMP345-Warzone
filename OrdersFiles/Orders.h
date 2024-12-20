#ifndef ORDERS_H
#define ORDERS_H

#include <vector>
#include <string>
#include <iostream>
#include "LogFiles/LoggingObserver.h"
#include <unordered_map>
class Player;

/**
 * @brief The base class representing an Order in the game.
 * Defines a generic order with type and validation/execution mechanisms.
 */
class Order : public Subject, public ILoggable
{
public:
    /**
     * @brief The type of the order (e.g., deploy, advance, bomb, etc.).
     */
    std::string orderType;

    // A flag indicating whether the order has been validated and is ready for execution.
    bool validOrder;

    /**
     * @brief Validates whether the order is valid.
     * This method is purely virtual and must be implemented by derived classes.
     */
    virtual void validate() = 0;

    /**
     * @brief Executes the order, resulting in some game action.
     * This method is purely virtual and must be implemented by derived classes.
     */
    virtual void execute() = 0;

    /**
     * @brief Default constructor for the Order class.
     * Initializes the orderType to "unknown".
     */
    Order();

    /**
     * @brief Copy constructor for the Order class.
     * Copies the orderType from another Order instance.
     *
     * @param orderCopy The Order object to copy from.
     */
    Order(const Order &orderCopy);

    /**
     * @brief Assignment operator overload for the Order class.
     * Copies the orderType from another Order instance.
     *
     * @param order The Order object to assign from.
     */
    void operator=(const Order &order);

    /**
     * @brief Stream insertion operator overload for the Order class.
     * Outputs the order details to an output stream.
     *
     * @param COUT Output stream object.
     * @param ORDER The Order object to display.
     * @return std::ostream& The modified output stream.
     */
    friend std::ostream &operator<<(std::ostream &COUT, const Order &ORDER);

    std::string stringToLog() const override;
};

/**
 * @brief Represents a list of orders in the game.
 * Manages a collection of orders and provides methods to add, move, and remove orders.
 */
class OrdersList : public Subject, public ILoggable
{
public:
    /**
     * @brief Vector containing pointers to the list of orders.
     */
    std::vector<Order *> ordersVector;

    /**
     * @brief Adds an Order to the list of orders.
     * Orders are added sequentially to the end of the list.
     *
     * @param order A pointer to the Order object to add.
     */
    void add(Order *order);

    /**
     * @brief Moves an Order from one position to another within the list.
     * Both positions are 1-based indices.
     *
     * @param orderPos The current position of the order to move (1-based index).
     * @param newOrderPos The new position of the order (1-based index).
     */
    void move(int orderPos, int newOrderpos);

    /**
     * @brief Removes an Order from the list at the specified position.
     * The order position is 1-based. Frees the memory allocated for the order.
     *
     * @param orderPos The position of the order to remove (1-based index).
     */
    void remove(int orderPos);

    /**
     * @brief Default constructor for the OrdersList class.
     */
    OrdersList();

    /**
     * @brief Copy constructor for the OrdersList class.
     * Copies the list of orders from another OrdersList instance.
     *
     * @param ordersListCopy The OrdersList object to copy from.
     */
    OrdersList(const OrdersList &ordersListCopy);

    /**
     * @brief Assignment operator overload for the OrdersList class.
     * Copies the list of orders from another OrdersList instance.
     *
     * @param ordersList The OrdersList object to assign from.
     */
    void operator=(const OrdersList &ordersList);

    /**
     * @brief Stream insertion operator overload for the OrdersList class.
     * Outputs the list of orders to an output stream.
     *
     * @param COUT Output stream object.
     * @param ORDERSLIST The OrdersList object to display.
     * @return std::ostream& The modified output stream.
     */
    friend std::ostream &operator<<(std::ostream &COUT, const OrdersList &ORDERSLIST);

    /**
     * @brief Destructor for the OrdersList class.
     * Frees the memory allocated for each Order in the list.
     */
    virtual ~OrdersList();

    std::string stringToLog() const override;
};

/**
 * @brief A class representing a DeployOrder in the game.
 * Handles troop deployment actions.
 */
class DeployOrder : public Order
{
public:
    // The name of the territory where army units are to be deployed.
    std::string territoryDeployName;
    // The number of army units to be deployed to the specified territory.
    int army;
    // A pointer to the player who issued the deploy order.
    Player *player;

    /**
     * @brief Constructor for DeployOrder.
     * Initializes the orderType to "deploy".
     */
    DeployOrder();

    /**
     * Parameterized constructor for the DeployOrder class.
     *
     * @param p The player who issued the order.
     * @param tName The name of the territory where the army units are being deployed.
     * @param armyDeployed The number of army units being deployed to the territory.
     */
    DeployOrder(Player *p, const std::string tName, int armyUnits);

    /**
     * @brief Validates the DeployOrder.
     * Checks whether the deployment is valid.
     */
    void validate() override;

    /**
     * @brief Executes the DeployOrder.
     * Deploys troops if the order is valid.
     */
    void execute() override;
};

/**
 * @brief A class representing an AdvanceOrder in the game.
 * Handles troop advancement actions.
 */
class AdvanceOrder : public Order
{
public:
    // The name of the territory from which the army will advance.
    std::string territoryAdvanceSName;
    // The name of the territory to which the army will advance.
    std::string territoryAdvanceTName;
    // The number of army units to advance.
    int army;
    // A pointer to the player who issued the advance order.
    Player *player;
    Player *enemyPlayer;

    /**
     * @brief Constructor for AdvanceOrder.
     * Initializes the orderType to "advance".
     */
    AdvanceOrder();

    /**
     * Parameterized constructor for the AdvanceOrder class.
     *
     * @param p A pointer to the player who is making the advance order.
     * @param sName The name of the source territory from which the army units will advance.
     * @param tName The name of the target territory to which the army units will advance.
     * @param armyUnits The number of army units that are being advanced.
     */
    AdvanceOrder(Player *p, Player *enemyP, const std::string sName, const std::string tName, int armyUnits);

    /**
     * @brief Validates the AdvanceOrder.
     * Checks whether advancing troops is valid.
     */
    void validate() override;

    /**
     * @brief Executes the AdvanceOrder.
     * Advances troops if the order is valid.
     */
    void execute() override;
};

/**
 * @brief A class representing a BombOrder in the game.
 * Handles bomb actions.
 */
class BombOrder : public Order
{
public:
    void setPlayer(Player *p);

    std::string territoryBombName;
    Player *player;

    /**
     * @brief Constructor for BombOrder.
     * Initializes the orderType to "bomb".
     */
    BombOrder();

    /**
     * Parameterized constructor for the BombOrder class.
     *
     * @param tBombName The name of the territory to bomb.
     */
    BombOrder(std::string tBombName);

    BombOrder(Player *player, const std::string &territoryBombName);

    /**
     * @brief Validates the BombOrder.
     * Checks whether bombing is valid.
     */
    void validate() override;

    /**
     * @brief Executes the BombOrder.
     * Bombs the target if the order is valid.
     */
    void execute() override;
};

/**
 * @brief A class representing a BlockadeOrder in the game.
 * Handles blockade actions.
 */
class BlockadeOrder : public Order
{
public:
    // The name of the territory to block.
    std::string territoryBlockadeName;
    Player *player;
    Player *neutralPlayer = nullptr;

    /**
     * @brief Constructor for BlockadeOrder.
     * Initializes the orderType to "blockade".
     */
    BlockadeOrder();

    void setPlayer(Player *p);

    /**
     * Parameterized constructor for the BlockadeOrder class.
     *
     * @param tBlockadeName The name of the territory to blockade.
     */
    BlockadeOrder(std::string tBlockadeName);

    /**
     * @brief Validates the BlockadeOrder.
     * Checks whether the blockade is valid.
     */
    void validate() override;

    /**
     * @brief Executes the BlockadeOrder.
     * Initiates a blockade if the order is valid.
     */
    void execute() override;
};

/**
 * @brief A class representing an AirliftOrder in the game.
 * Handles airlift actions.
 */
class AirliftOrder : public Order
{
public:
    void setPlayer(Player *p);

    // The name of the territory from which the army will be airlifted.
    std::string territoryAirliftSName;
    // The name of the territory to which the army will be airlifted.
    std::string territoryAirliftTName;
    // The number of army units to airlift.
    int army;
    Player *player;

    /**
     * @brief Constructor for AirliftOrder.
     * Initializes the orderType to "airlift".
     */
    AirliftOrder();

    /**
     * @brief Parameterized constructor for the AirliftOrder class.
     *
     * @param airSName The name of the source territory from which the army units will be airlifted.
     * @param airTName The name of the target territory to which the army units will be airlifted.
     * @param units The number of army units to airlift.
     */
    AirliftOrder(std::string airSName, std::string airTName, int units);

    /**
     * @brief Validates the AirliftOrder.
     * Checks whether airlifting is valid.
     */
    void validate() override;

    /**
     * @brief Executes the AirliftOrder.
     * Airlifts units if the order is valid.
     */
    void execute() override;
};

/**
 * @brief A class representing a NegotiateOrder in the game.
 * Handles negotiation actions.
 */
class NegotiateOrder : public Order
{
public:
    // The name of the player to negotiate with.
    std::string playerTargetName;
    Player *player;
    static std::unordered_map<std::string, std::string> negotiatedPlayers;
    void setPlayer(Player *p);

    /**
     * @brief Constructor for NegotiateOrder.
     * Initializes the orderType to "negotiate".
     */
    NegotiateOrder();

    /**
     * Parameterized constructor for the NegotiateOrder class.
     *
     * @param pTargetName The name of the player to negotiate with.
     */
    NegotiateOrder(std::string pTargetNAme);

    NegotiateOrder(Player *p, std::string pTargetName);

    /**
     * @brief Validates the NegotiateOrder.
     * Checks whether the negotiation is valid.
     */
    void validate() override;

    /**
     * @brief Executes the NegotiateOrder.
     * Negotiates peace if the order is valid.
     */
    void execute() override;
};

#endif
