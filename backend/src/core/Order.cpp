#include "../../include/core/Order.h"
#include "../../include/core/Product.h" // Needed to calculate totals
#include <iostream>

// Constructor implementation
Order::Order(int id, int custId)
    : orderId(id), customerId(custId), totalAmount(0.0f), status("Pending") {}

Order::~Order() {
    // Note: We don't delete products here as they are managed by the Shopkeeper/Inventory
    orderItems.clear(); 
}

// Getters
int Order::getOrderId() const { return orderId; }
int Order::getCustomerId() const { return customerId; }
float Order::getTotalAmount() const { return totalAmount; }
std::string Order::getStatus() const { return status; }

// Setters and Logic
void Order::setStatus(const std::string& newStatus) {
    status = newStatus;
}

void Order::addProductToOrder(Product* product, int quantity) {
    if (product != nullptr && product->getStock() >= quantity) {
        // In a real system, you might need an OrderItem struct to hold product + quantity.
        // For simplicity based on headers, we add the product pointer.
        orderItems.push_back(product);
        totalAmount += (product->getPrice() * quantity);
        product->reduceStock(quantity);
        std::cout << "Added " << quantity << "x " << product->getName() << " to order." << std::endl;
    } else {
         std::cout << "Failed to add product. Out of stock or invalid product." << std::endl;
    }
}

void Order::calculateTotal() {
    totalAmount = 0.0f;
    for (const auto& product : orderItems) {
        totalAmount += product->getPrice();
    }
}

void Order::displayOrderSummary() const {
    std::cout << "--- Order Summary ---" << std::endl;
    std::cout << "Order ID: " << orderId << " | Status: " << status << std::endl;
    std::cout << "Customer ID: " << customerId << std::endl;
    std::cout << "Items in order: " << orderItems.size() << std::endl;
    std::cout << "Total Amount: $" << totalAmount << std::endl;
}
