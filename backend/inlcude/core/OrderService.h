#ifndef ORDER_SERVICE_H
#define ORDER_SERVICE_H

#include "Order.h"
#include "Customer.h"
#include <vector>

class OrderService {
private:
    std::vector<Order*> activeOrders;
    std::vector<Order*> completedOrders;

public:
    OrderService();
    ~OrderService();

    // Order Processing Logic
    void createOrder(Order* newOrder, Customer* customer);
    void updateOrderStatus(int orderId, const std::string& newStatus);
    void processPayment(int orderId);
    void completeOrder(int orderId);
    
    void displayActiveOrders() const;
};

#endif // ORDER_SERVICE_H
