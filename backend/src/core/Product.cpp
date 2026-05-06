#include "../../inlcude/core/Product.h"
#include <iostream>

Product::Product(std::string sku, std::string name, double price, uint32_t stock, std::string owner)
    : m_sku(sku), m_name(name), m_price(price >= 0.01 ? price : 0.01), m_stockQuantity(stock), m_owner(owner) {}

void Product::setPrice(double newPrice) {
    if (newPrice >= 0.01) {
        m_price = newPrice;
    }
}

bool Product::reduceStock(uint32_t amount) {
    if (m_stockQuantity >= amount) {
        m_stockQuantity -= amount;
        return true;
    }
    std::cout << "Insufficient stock for product: " << m_name << std::endl;
    return false;
}

void Product::restock(uint32_t amount) {
    m_stockQuantity += amount;
}

std::string Product::getSku() const { return m_sku; }
std::string Product::getName() const { return m_name; }
double Product::getPrice() const { return m_price; }
uint32_t Product::getStock() const { return m_stockQuantity; }
std::string Product::getOwner() const { return m_owner; }

void Product::getInfo() const {
    std::cout << "SKU: " << m_sku 
              << " | Name: " << m_name 
              << " | Price: $" << m_price 
              << " | Stock: " << m_stockQuantity << std::endl;
}
