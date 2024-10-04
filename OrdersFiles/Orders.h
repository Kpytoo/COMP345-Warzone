#ifndef ORDERS_H
#define ORDERS_H

#include <vector>
#include <string>
#include <iostream>

class Order
{
    public:
        //Possible types are deploy, advance, bomb, blockade, airlift, and negotiate
        std::string orderType;
        //validate() method that verifies if the order is valid
        virtual void validate() = 0;
        //execute() method that will result in some game action being implemented (see the project description document)
        virtual void execute() = 0;
        //Default constructor
        Order();
        //Copy constructor
        Order(const Order &orderCopy);
        //Assignment operator overload
        void operator=(const Order &order);
        //Stream insertion operator overload
        friend std::ostream& operator<<(std::ostream &COUT, const Order &ORDER);
};

class OrdersList 
{   
    public:
        //Vector that contains a list of orders
        std::vector<Order*> ordersVector;
        //add() method to add orders sequentially to list of orders
        void add(Order* order);
        //move() method to move an order in the list of orders
        void move(int orderPos, int newOrderpos);
        //remove() method that deletes an order from the list
        void remove(int orderPos);
        //Default constructor
        OrdersList();
        //Copy constructor
        OrdersList(const OrdersList &ordersListCopy);
        //Assignment operator overload
        void operator=(const OrdersList &ordersList);
        //Stream insertion operator overload
        friend std::ostream& operator<<(std::ostream &COUT, const OrdersList &ORDERSLIST);

    virtual ~OrdersList();

};

class DeployOrder : public Order {
public:
    DeployOrder();

    void validate() override;

    void execute() override;
};

class AdvanceOrder : public Order {
public:
    AdvanceOrder();

    void validate() override;

    void execute() override;
};

class BombOrder : public Order {
public:
    BombOrder();

    void validate() override;

    void execute() override;
};

class BlockadeOrder : public Order {
public:
    BlockadeOrder();

    void validate() override;

    void execute() override;
};

class AirliftOrder : public Order {
public:
    AirliftOrder();

    void validate() override;

    void execute() override;
};

class NegotiateOrder : public Order {
public:
    NegotiateOrder();

    void validate() override;

    void execute() override;
};


#endif