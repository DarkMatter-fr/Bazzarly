#ifndef SHOPKEEPER_H
#define SHOPKEEPER_H

#include "User.h"
#include <vector>
#include <string>

// Forward declarations of existing classes to prevent circular dependencies
class Product;
class Customer;

class Shopkeeper : public User {
public:
    Shopkeeper(int id, const std::string& name, const std::string& email);
    ~Shopkeeper();

    // Core functionality from first image references
    void addProduct(Product* product);
    void removeProduct(int productId);
    void updateProductDetails(int productId, float newPrice, const std::string& newDescription);
    void manageInventory();

    // Order management referenced in program structure
    void processOrders();
    std::string getStorePerformanceReport();

private:
    std::vector<Product*> managedInventory;
};

#endif // SHOPKEEPER_H
