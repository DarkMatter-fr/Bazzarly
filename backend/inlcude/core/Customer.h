#ifndef Customer_H
#define Customer_H

#include "User.h" // Customer is a type of User, so we include the User header
#include <vector>
#include <string>   
#include "Order.h" // To keep track of customer's orders

class Customer : public User{
    protected :
        std::vector<Order> orderHistory; // List of past orders
        double accountBalance; // For storing customer's wallet balance
        int loyaltyPoints; // For tracking loyalty points
        bool isPrimeMember; // To check if the customer is a prime member

    public:
        // Constructor
        Customer(std::string username, std::string email, std::string password);

        // Business Logic Methods
        void placeOrder(const Order& newOrder); // To add a new order to the history        
        void addFunds(double amount); // To add money to the customer's account
        bool redeemLoyaltyPoints(int points); // To redeem loyalty points for discounts     
        void upgradeToPrime(); // To upgrade the customer to prime membership
        void displayOrderHistory() const; // To display past orders
};

#endif //Customer_H
