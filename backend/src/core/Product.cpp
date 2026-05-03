#include "../../include/core/Product.h"
#include <iostream>

// Constructor implementation
Product::Product(int id, const std::string& name, float price, int stock)
    : productId(id), productName(name), productPrice(price), stockQuantity(stock) {}

Product::~Product() {}

// Getters
int Product::getId() const { return productId; }
std::string Product::getName() const { return productName; }
float Product::getPrice() const { return productPrice; }
int Product::getStock() const { return stockQuantity; }

// Setters
void Product::setName(const std::string& name) { productName = name; }
void Product::setPrice(float price) { productPrice = price; }
void Product::setStock(int stock) { stockQuantity = stock; }

// Inventory logic
void Product::reduceStock(int amount) {
    if (stockQuantity >= amount) {
        stockQuantity -= amount;
    } else {
        std::cout << "Insufficient stock for product: " << productName << std::endl;
    }
}

void Product::addStock(int amount) {
    stockQuantity += amount;
}

// Display utility
void Product::displayProductInfo() const {
    std::cout << "ID: " << productId 
              << " | Name: " << productName 
              << " | Price: $" << productPrice 
              << " | Stock: " << stockQuantity << std::endl;
}
