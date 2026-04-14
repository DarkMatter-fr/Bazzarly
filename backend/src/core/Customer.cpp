#include <iostream>
#include "Customer.h"
Customer::Customer(std::string username, std::string email, std::string password) 
    : User(username, email, password), accountBalance(0.0), loyaltyPoints(0), isPrimeMember(false) {
}

void Customer:: placeOrder(const Order& newOrder) {
    orderHistory.push_back(newOrder);
}

void Customer::addFunds(double amount) {
    accountBalance += amount;
}

bool Customer:: redeemLoyaltyPoints(int points){
    if (loyaltyPoints >= points && points > 0) {
        loyaltyPoints -= points;
        return true;
    }
    return false;
}

void Customer::upgradeToPrime() {
    isPrimeMember = true;
}

void Customer :: displayOrderHistory () const{
    std::cout << "Order History for "<< name << ":\n";
    for (const auto& order : orderHistory) {
        std::cout << "- " << order.toString() << "\n";
    }
}
