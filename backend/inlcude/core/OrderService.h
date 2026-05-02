#ifndef ORDER_SERVICE_H
#define ORDER_SERVICE_H

#include "../models/Order.h"

class OrderService {
public:
    Order createOrder(int id);
    void addItemToOrder(Order &order, const Product& p);
    void checkout(Order &order) const;
};

#endif
