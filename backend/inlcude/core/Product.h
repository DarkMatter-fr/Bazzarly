#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <cstdint> // For uint32_t

class Product{
    private :
    std::string m_sku;           // Unique ID
    std::string m_name;
    double m_price;              // Price (Minimum 0.01)
    uint32_t m_stockQuantity;    // Unsigned to prevent negative stock

public:
    // Constructor using Member Initializer List (Implementation will be in .cpp)
    Product(std::string sku, std::string name, double price, uint32_t stock);

    // --- Business Logic (The Setters) ---
    
    // Ensures price never drops below 0.01
    void setPrice(double newPrice);
    
    // Checks if enough stock exists before deducting
    bool reduceStock(uint32_t amount);
    
    // For shopkeepers to add inventory
    void restock(uint32_t amount);

    // --- Accessors (The Getters) ---
    // All marked 'const' because they don't change the object's state
    std::string getSku() const;
    std::string getName() const;
    double getPrice() const;
    uint32_t getStock() const;

    // Display info to console
    void displayInfo() const;

        
};

#endif //PRODUCT_H