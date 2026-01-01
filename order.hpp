#pragma once

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