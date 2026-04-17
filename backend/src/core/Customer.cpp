#include <iostream>
#include "core/Customer.h"

// Constructor
Customer::Customer(std::string username, std::string email, std::string password) 
    : User(username, email, password), 
      loyaltyPoints(0), 
      isPrimeMember(false) 
{
    // Set base class members here instead
    this->accountBalance = 0.0; 
}

void Customer::placeOrder(const Order& newOrder) {
    orderHistory.push_back(newOrder);
}

void Customer::addFunds(double amount) {
    if (amount > 0) {
        accountBalance += amount;
    }
}

bool Customer::redeemLoyaltyPoints(int points) {
    if (loyaltyPoints >= points && points > 0) {
        loyaltyPoints -= points;
        return true;
    }
    return false;
}

void Customer::upgradeToPrime() {
    isPrimeMember = true;
}

void Customer::displayOrderHistory() const {
    std::cout << "Order History for " << name << ":\n";
    
    if (orderHistory.empty()) {
    
        std::cout << "No orders found." << std::endl;
        return;
    }

    for (const Order& order : orderHistory) {

        std::cout << "- ";
        order.displayReceipt(); 
    }
}