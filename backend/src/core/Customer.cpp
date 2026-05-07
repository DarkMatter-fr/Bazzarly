#include <iostream>
#include "../../inlcude/core/Customer.h"

using namespace std;

// Constructor
Customer::Customer(string username, string email, string password) 
    : User(username, email, password), 
      loyaltyPoints(0), 
      isPrimeMember(false) 
{
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
    cout << "Order History for " << name << ":\n";
    
    if (orderHistory.empty()) {
        cout << "No orders found." << endl;
        return;
    }

    for (const Order& order : orderHistory) {
        cout << "- ";
        order.displayReceipt(); 
    }
}