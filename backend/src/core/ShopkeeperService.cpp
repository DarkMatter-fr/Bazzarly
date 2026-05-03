#include "../../include/core/ShopkeeperService.h"
#include <iostream>
#include <algorithm>

ShopkeeperService::ShopkeeperService() {
    std::cout << "ShopkeeperService initialized." << std::endl;
}

ShopkeeperService::~ShopkeeperService() {
    // Inventory cleanup logic if the service owns the pointers
    inventory.clear();
}

void ShopkeeperService::addProductToInventory(Product* product) {
    if (product != nullptr) {
        inventory.push_back(product);
        std::cout << "Product successfully added to inventory." << std::endl;
    }
}

void ShopkeeperService::removeProductFromInventory(int productId) {
    // Placeholder logic: In a real scenario, you'd match the productId against the Product objects
    std::cout << "Attempting to remove product ID: " << productId << " from inventory." << std::endl;
}

void ShopkeeperService::updateProductInventory(int productId, int additionalStock) {
    std::cout << "Stock updated for product ID: " << productId << " by " << additionalStock << " units." << std::endl;
}

void ShopkeeperService::generateInventoryReport() const {
    std::cout << "--- Inventory Report ---" << std::endl;
    std::cout << "Total unique products managed: " << inventory.size() << std::endl;
}
