#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

enum class AccountStatus { ACTIVE, SUSPENDED, BANNED };

struct Date {
    int day;
    int month;
    int year;
}; 

class User {
protected:
    std::string name;
    std::string email;
    std::string password;
    std::string user_id;
    double accountBalance; // Added for logic
    AccountStatus status;  // Added for logic
    Date createdAt;

public:
    // Using Const Reference (The 'Move' Alternative)
    User(const std::string& username, const std::string& email, const std::string& password);

    // Business Logic
    void depositFunds(double amount);
    bool withdrawFunds(double amount);
    void updateStatus(AccountStatus newStatus);

    // Getters
    std::string getUserId() const { return user_id; }
    std::string getName() const { return name; }
    double getBalance() const { return accountBalance; }

    // Virtual destructor is mandatory for Inheritance
    virtual ~User() = default; 
};

#endif