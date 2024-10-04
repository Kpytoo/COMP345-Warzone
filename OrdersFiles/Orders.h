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
        void validate();
        //execute() method that will result in some game action being implemented (see the project description document)
        void execute();
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
        //move() method to move an order in the list of orders
        void move();
        //remove() method that deletes an order from the list
        void remove();
        //Default constructor
        OrdersList();
        //Copy constructor
        OrdersList(const OrdersList &ordersListCopy);
        //Assignment operator overload
        void operator=(const OrdersList &ordersList);
        //Stream insertion operator overload
        friend std::ostream& operator<<(std::ostream &COUT, const OrdersList &ORDERSLIST);

};

#endif