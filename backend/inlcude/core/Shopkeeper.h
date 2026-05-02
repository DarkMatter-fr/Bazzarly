#ifndef SHOPKEEPER_SERVICE_H
#define SHOPKEEPER_SERVICE_H

#include "../models/Product.h"
#include <vector>

class ShopkeeperService {
private:
    vector<Product> inventory;

public:
    void addProduct(const Product& p);
    void removeProduct(int id);
    void updateStock(int id, int newStock);
    void displayProducts() const;
    Product getProduct(int id) const;
    vector<Product>& getInventory();
};

#endif
