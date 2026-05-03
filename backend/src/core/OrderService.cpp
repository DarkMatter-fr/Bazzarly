#include "../../include/core/OrderService.h"
#include <iostream>

OrderService::OrderService() {
    std::cout << "OrderService initialized." << std::endl;
}

OrderService::~OrderService() {
    activeOrders.clear();
    completedOrders.clear();
}

void OrderService::createOrder(Order* newOrder, Customer* customer) {
    if (newOrder && customer) {
        activeOrders.push_back(newOrder);
        std::cout << "New order created for customer." << std::endl;
    }
}

void OrderService::updateOrderStatus(int orderId, const std::string& newStatus) {
    // Requires your existing Order.h to have a method like setStatus()
    std::cout << "Status for order ID " << orderId << " updated to: " << newStatus << std::endl;
}

void OrderService::processPayment(int orderId) {
    std::cout << "Processing payment for order ID: " << orderId << "..." << std::endl;
    // Payment logic goes here
}

void OrderService::completeOrder(int orderId) {
    // Logic to move an order from activeOrders to completedOrders
    std::cout << "Order ID " << orderId << " marked as complete and archived." << std::endl;
}

void OrderService::displayActiveOrders() const {
    std::cout << "Currently tracking " << activeOrders.size() << " active orders." << std::endl;
}
