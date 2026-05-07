#include "../../inlcude/core/Order.h"
#include <iostream>

using namespace std;

Order::Order(string orderId, string customerId)
    : m_orderId(orderId), m_customerId(customerId), m_totalAmount(0.0), m_status(OrderStatus::Pending) {}

void Order::addItem(const Product& product, uint32_t quantity) {
    OrderItem item{product.getSku(), product.getName(), product.getPrice(), quantity};
    m_items.push_back(item);
    m_totalAmount += (product.getPrice() * quantity);
}

void Order::processPayment() {
    m_status = OrderStatus::Paid;
}

void Order::updateStatus(OrderStatus newStatus) {
    m_status = newStatus;
}

double Order::getTotalAmount() const {
    return m_totalAmount;
}

OrderStatus Order::getStatus() const {
    return m_status;
}

void Order::displayReceipt() const {
    cout << "Receipt for Order: " << m_orderId << "\n";
    cout << "Total: $" << m_totalAmount << "\n";
}
