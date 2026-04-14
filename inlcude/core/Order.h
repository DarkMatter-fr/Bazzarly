#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <cstdint>
#include "core/Product.h" // We need the Product shape


struct OrderItem {
    std::string productSku;
    std::string productName;
    double soldPrice; 
    uint32_t quantity;
};

enum class OrderStatus {
    Pending,
    Paid,
    Shipped,
    Delivered,
    Cancelled
};

class Order {
private:
    std::string m_orderId;
    std::string m_customerId;
    std::vector<OrderItem> m_items; // List of products purchased
    double m_totalAmount;
    OrderStatus m_status;
    std::string m_orderDate;

public:
    // Constructor
    Order(std::string orderId, std::string customerId);

    // --- Business Logic ---
    
    // Adds a product snapshot to the order
    void addItem(const Product& product, uint32_t quantity);
    
    // Finalizes the total and moves status to 'Paid'
    void processPayment();
    
    // Updates shipping status (Used by Shopkeepers/Admins)
    void updateStatus(OrderStatus newStatus);

    // --- Accessors (Const) ---
    double getTotalAmount() const;
    OrderStatus getStatus() const;
    void displayReceipt() const;
};

#endif // ORDER_H