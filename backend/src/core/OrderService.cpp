#include "OrderService.h"
#include <iostream>

using namespace std;

Order OrderService::createOrder(int id) {
    return Order(id);
}

void OrderService::addItemToOrder(Order &order, const Product& p) {
    order.addItem(p);
}

void OrderService::checkout(Order &order) const {
    order.calculateTotal();

    cout << "\nOrder ID: " << order.orderId << endl;
    cout << "Items:\n";

    for (const auto &p : order.items)
        cout << p.name << " - " << p.price << endl;

    cout << "Total: " << order.total << endl;
}
