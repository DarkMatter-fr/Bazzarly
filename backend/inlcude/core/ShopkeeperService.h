#ifndef SHOPKEEPER_SERVICE_H
#define SHOPKEEPER_SERVICE_H

#include "Product.h"
#include "Order.h"
#include <vector>
#include <string>

class ShopkeeperService {
private:
    std::vector<Product*> inventory;

public:
    ShopkeeperService();
    ~ShopkeeperService();

    // Inventory Management
    void addProductToInventory(Product* product);
    void removeProductFromInventory(int productId);
    void updateProductInventory(int productId, int additionalStock);
    void updateProductPrice(const std::string& sku, double newPrice);
    const std::vector<Product*>& getInventory() const { return inventory; }
    
    // Shopkeeper specific metrics
    void generateInventoryReport() const;
};

#endif // SHOPKEEPER_SERVICE_H
