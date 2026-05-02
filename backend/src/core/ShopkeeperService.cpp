#include "ShopkeeperService.h"
#include <iostream>

using namespace std;

void ShopkeeperService::addProduct(const Product& p) {
    inventory.push_back(p);
}

void ShopkeeperService::removeProduct(int id) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->id == id) {
            inventory.erase(it);
            break;
        }
    }
}

void ShopkeeperService::updateStock(int id, int newStock) {
    for (auto &p : inventory) {
        if (p.id == id) {
            p.stock = newStock;
        }
    }
}

void ShopkeeperService::displayProducts() const {
    for (const auto &p : inventory) {
        cout << p.id << " | "
             << p.name << " | "
             << p.price << " | "
             << p.stock << endl;
    }
}

Product ShopkeeperService::getProduct(int id) const {
    for (const auto &p : inventory)
        if (p.id == id)
            return p;

    return Product();
}

vector<Product>& ShopkeeperService::getInventory() {
    return inventory;
}
