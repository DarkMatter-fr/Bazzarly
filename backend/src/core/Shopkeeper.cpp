#include "include/core/Shopkeeper.h"
#include "include/core/Product.h"
#include "include/core/Customer.h"
#include <iostream>

// Initializing the inherited User data via Constructor
Shopkeeper::Shopkeeper(int id, const std::string& name, const std::string& email) 
    : User(id, name, email) {
    std::cout << "Shopkeeper initialized with User ID: " << User::getId() << std::endl;
}

Shopkeeper::~Shopkeeper() {
    // Standard cleanup logic for managed inventory
    managedInventory.clear();
}

void Shopkeeper::addProduct(Product* product) {
    if (product) {
        managedInventory.push_back(product);
        std::cout << "Product added to inventory: " << product->getProductName() << std::endl;
    }
}

void Shopkeeper::removeProduct(int productId) {
    // Placeholder implementation, to be populated with vector search logic
    std::cout << "Product with ID " << productId << " removed." << std::endl;
}

void Shopkeeper::updateProductDetails(int productId, float newPrice, const std::string& newDescription) {
    // Implementation requires accessing Product functions (e.g., product->setPrice())
    std::cout << "Product details updated for ID: " << productId << std::endl;
}

void Shopkeeper::manageInventory() {
    std::cout << "Opening inventory management interface..." << std::endl;
}

void Shopkeeper::processOrders() {
    // This connects your domain (Shopkeeper) with the Order file domain
    std::cout << "Processing active customer orders..." << std::endl;
}

std::string Shopkeeper::getStorePerformanceReport() {
    return "Store Performance Report: Placeholder Summary Data.";
}
