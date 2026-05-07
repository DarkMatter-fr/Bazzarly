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
    std::string role; // Added for main.cpp integration
    std::string token; // Added for main.cpp integration


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
    std::string getPassword() const { return password; }
    std::string getRole() const { return role; }
    void setRole(const std::string& r) { role = r; }
    std::string getToken() const { return token; }
    void setToken(const std::string& t) { token = t; }

    // Virtual destructor is mandatory for Inheritance
    virtual ~User() = default; 
};

#endif