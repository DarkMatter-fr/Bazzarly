#include "../../inlcude/core/OrderService.h"
#include <iostream>

using namespace std;

OrderService::OrderService() {
    cout << "OrderService initialized." << endl;
}

OrderService::~OrderService() {
    activeOrders.clear();
    completedOrders.clear();
}

void OrderService::createOrder(Order* newOrder, Customer* customer) {
    if (newOrder && customer) {
        activeOrders.push_back(newOrder);
        cout << "New order created for customer." << endl;
    }
}

void OrderService::updateOrderStatus(int orderId, const string& newStatus) {
    cout << "Status for order ID " << orderId << " updated to: " << newStatus << endl;
}

void OrderService::processPayment(int orderId) {
    cout << "Processing payment for order ID: " << orderId << "..." << endl;
}

void OrderService::completeOrder(int orderId) {
    cout << "Order ID " << orderId << " marked as complete and archived." << endl;
}

void OrderService::displayActiveOrders() const {
    cout << "Currently tracking " << activeOrders.size() << " active orders." << endl;
}
