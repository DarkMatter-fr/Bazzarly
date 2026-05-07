#include "../../inlcude/core/User.h"
#include <ctime>
#include <iostream>

using namespace std;

User::User(const string& username, const string& userEmail, const string& userPassword) 
    : name(username), email(userEmail), password(userPassword), accountBalance(0.0), status(AccountStatus::ACTIVE) {
    
    // Simple ID Generation
    this->user_id = "USR-" + (name.length() >= 2 ? name.substr(0, 2) : name) + "99";

    // Set Creation Date
    time_t t = time(0);
    tm* now = localtime(&t);
    createdAt.day = now->tm_mday;
    createdAt.month = now->tm_mon + 1;
    createdAt.year = now->tm_year + 1900;
}

void User::depositFunds(double amount) {
    if (amount > 0) {
        accountBalance += amount;
        cout << "Deposited: " << amount << ". New Balance: " << accountBalance << endl;
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