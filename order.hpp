#pragma once

#include <list>

enum class Side {
    BUY,
    SELL
};

struct Order {
    int order_id;
    Side side;
    double price;
    int quantity;
    int timestamp;
};

struct OrderLocation {
    Side side;
    double price;
    std::list<Order>::iterator it;
};
