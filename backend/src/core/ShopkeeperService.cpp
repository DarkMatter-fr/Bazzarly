#include "../../inlcude/core/ShopkeeperService.h"
#include <iostream>
#include <algorithm>

using namespace std;

ShopkeeperService::ShopkeeperService() {
    cout << "ShopkeeperService initialized." << endl;
}

ShopkeeperService::~ShopkeeperService() {
    inventory.clear();
}

void ShopkeeperService::addProductToInventory(Product* product) {
    if (product != nullptr) {
        inventory.push_back(product);
        cout << "Product successfully added to inventory." << endl;
    }
}

void ShopkeeperService::removeProductFromInventory(int productId) {
    cout << "Attempting to remove product ID: " << productId << " from inventory." << endl;
}

void ShopkeeperService::updateProductInventory(int productId, int additionalStock) {
    cout << "Stock updated for product ID: " << productId << " by " << additionalStock << " units." << endl;
}

void ShopkeeperService::updateProductPrice(const string& sku, double newPrice) {
    for (Product* product : inventory) {
        if (product != nullptr && product->getSku() == sku) {
            product->setPrice(newPrice);
            cout << "Successfully updated price for " << product->getName() 
                      << " (SKU: " << sku << ") to $" << newPrice << endl;
            return;
        }
    }
    cout << "Error: Product with SKU " << sku << " not found in inventory." << endl;
}

void ShopkeeperService::generateInventoryReport() const {
    cout << "--- Inventory Report ---" << endl;
    cout << "Total unique products managed: " << inventory.size() << endl;
}
