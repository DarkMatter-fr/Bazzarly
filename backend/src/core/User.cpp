#include "User.h"
#include <ctime>

User::User(const std::string& username, const std::string& userEmail, const std::string& userPassword) 
    : name(username), email(userEmail), password(userPassword), accountBalance(0.0), status(AccountStatus::ACTIVE) {
    
    // Simple ID Generation
    this->user_id = "USR-" + (name.length() >= 2 ? name.substr(0, 2) : name) + "99";

    // Set Creation Date
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    createdAt.day = now->tm_mday;
    createdAt.month = now->tm_mon + 1;
    createdAt.year = now->tm_year + 1900;
}

void User::depositFunds(double amount) {
    if (amount > 0) {
        accountBalance += amount;
        std::cout << "Deposited: " << amount << ". New Balance: " << accountBalance << std::endl;
    }
}

bool User::withdrawFunds(double amount) {
    if (amount > 0 && amount <= accountBalance) {
        accountBalance -= amount;
        return true;
    }
    return false;
}

void User::updateStatus(AccountStatus newStatus) {
    this->status = newStatus;
}