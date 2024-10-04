#include "Orders.h"

/*
Note that the orders’ actions do not need to be implemented at this point. Invalid
orders can be created and put in the list, but their execution will not result in any action.
*/


// <<<< Order Class Definitions >>>> 

#include "Orders.h"

// ---------------------- Order Class Implementation ----------------------

Order::Order() : orderType("unknown") {}

Order::Order(const Order &orderCopy) : orderType(orderCopy.orderType) {}

void Order::operator=(const Order &order) {
    if (this != &order) {
        orderType = order.orderType;
    }
}

std::ostream& operator<<(std::ostream &COUT, const Order &ORDER) {
    COUT << "Order: " << ORDER.orderType;
    return COUT;
}

void Order::execute() {
    validate();  // Validate first
    std::cout << "Executing " << orderType << " order." << std::endl;
}

// ---------------------- Deploy Order ----------------------

DeployOrder::DeployOrder() { orderType = "deploy"; }

void DeployOrder::validate() {
    std::cout << "Validating deploy order..." << std::endl;
    // Assume validation logic here
}

void DeployOrder::execute() {
    if (true /* assume valid */) {
        Order::execute();
        std::cout << "Deploying troops." << std::endl;
    } else {
        std::cout << "Deploy order is invalid." << std::endl;
    }
}

// ---------------------- Advance Order ----------------------


AdvanceOrder::AdvanceOrder() { orderType = "advance"; }

void AdvanceOrder::validate() {
    std::cout << "Validating advance order..." << std::endl;
    // Assume validation logic here
}

void AdvanceOrder::execute() {
    if (true /* assume valid */) {
        Order::execute();
        std::cout << "Advancing troops." << std::endl;
    } else {
        std::cout << "Advance order is invalid." << std::endl;
    }
}

// ---------------------- Bomb Order ----------------------

BombOrder::BombOrder() { orderType = "bomb"; }

void BombOrder::validate() {
    std::cout << "Validating bomb order..." << std::endl;
    // Assume validation logic here
}

void BombOrder::execute() {
    if (true /* assume valid */) {
        Order::execute();
        std::cout << "Bombing target." << std::endl;
    } else {
        std::cout << "Bomb order is invalid." << std::endl;
    }
}

// ---------------------- BlockadeOrder Class Implementation ----------------------

BlockadeOrder::BlockadeOrder() {
    orderType = "blockade";
}

void BlockadeOrder::validate() {
    std::cout << "Validating blockade order..." << std::endl;
    // Assume validation logic here
}

void BlockadeOrder::execute() {
    if (true /* assume valid */) {
        Order::execute();
        std::cout << "Initiating blockade." << std::endl;
    } else {
        std::cout << "Blockade order is invalid." << std::endl;
    }
}

// ---------------------- AirliftOrder Class Implementation ----------------------

AirliftOrder::AirliftOrder() {
    orderType = "airlift";
}

void AirliftOrder::validate() {
    std::cout << "Validating airlift order..." << std::endl;
    // Assume validation logic here
}

void AirliftOrder::execute() {
    if (true /* assume valid */) {
        Order::execute();
        std::cout << "Airlifting units." << std::endl;
    } else {
        std::cout << "Airlift order is invalid." << std::endl;
    }
}

// ---------------------- NegotiateOrder Class Implementation ----------------------

NegotiateOrder::NegotiateOrder() {
    orderType = "negotiate";
}

void NegotiateOrder::validate() {
    std::cout << "Validating negotiate order..." << std::endl;
    // Assume validation logic here
}

void NegotiateOrder::execute() {
    if (true /* assume valid */) {
        Order::execute();
        std::cout << "Negotiating peace." << std::endl;
    } else {
        std::cout << "Negotiate order is invalid." << std::endl;
    }
}

// ---------------------- OrdersList Class Implementation ----------------------

OrdersList::OrdersList() {}

OrdersList::OrdersList(const OrdersList &ordersListCopy) {
    ordersVector = ordersListCopy.ordersVector;
}

void OrdersList::operator=(const OrdersList &ordersList) {
    if (this != &ordersList) {
        ordersVector = ordersList.ordersVector;
    }
}

void OrdersList::add(Order* order) {
    ordersVector.push_back(order);
}

void OrdersList::move(int orderPos, int newOrderPos) {
    // Convert from 1-based to 0-based indexing
    int currentIndex = orderPos - 1;
    int newIndex = newOrderPos - 1;

    if (currentIndex >= 0 && currentIndex < ordersVector.size() &&
        newIndex >= 0 && newIndex < ordersVector.size()) {
        Order* orderToMove = ordersVector.at(currentIndex);
        ordersVector.erase(ordersVector.begin() + currentIndex);
        ordersVector.insert(ordersVector.begin() + newIndex, orderToMove);
    } else {
        std::cerr << "Invalid positions for move operation." << std::endl;
    }
}

void OrdersList::remove(int orderPos) {
    // Convert from 1-based to 0-based indexing
    int index = orderPos - 1;

    if (index >= 0 && index < ordersVector.size()) {
        delete ordersVector.at(index); // Free the memory
        ordersVector.erase(ordersVector.begin() + index);
    } else {
        std::cerr << "Invalid position for remove operation." << std::endl;
    }
}

std::ostream& operator<<(std::ostream &COUT, const OrdersList &ORDERSLIST) {
    COUT << "Orders List:" << std::endl;
    for (int i = 0; i<ORDERSLIST.ordersVector.size(); i++) {
        COUT << i+1 << ". " << *ORDERSLIST.ordersVector[i] << std::endl;
    }
    return COUT;
}

OrdersList::~OrdersList() {
    for (Order* order : ordersVector) { // Free up dynamically allocated Order instances in list.
        delete order;
    }
}

// <<<< OrdersList Class Definitions >>>> 