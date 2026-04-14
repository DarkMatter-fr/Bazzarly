//GAURD FOR DEFINING CLASS 
#ifndef USER_H
#define USER_H

#include <string>

enum class AccountStatus { ACTIVE, SUSPENDED, BANNED };
typedef struct Date {
    int day;
    int month;
    int year;
};

//USER CLASS
class User{
    private :
        //USER'S PERSONAL DETAILS
        std:: string name ;
        std:: string email;
        std:: string password;
        std:: string user_id;
        //BUSINESS LOGIC ATTRIBUTES 
        double accountBalance ;
        int loyaltyPoints ;
        bool isPrimeMember; 
        //STATUS AND METADATA
        Date creatdAt;
        public:
    // Constructor
         User(std::string username, std::string email, std::string password);

    // Getters and Setters (Encapsulation)
        void depositFunds(double amount);
        bool withdrawFunds(double amount);
        void updateStatus(AccountStatus newStatus);
    
    // We only provide a getter for ID, no setter. 
    // An ID should never change once assigned!
    std::string getUserId() const { return user_id; }
   
};

#endif //USER_H