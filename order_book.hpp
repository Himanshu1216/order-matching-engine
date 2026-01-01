#pragma once
#include "order.hpp"
#include <map>
#include <queue>

class OrderBook {
public:
    void add_order(Side side, double price, int quantity);

private:
    void match_buy(Order& buy);
    void match_sell(Order& sell);

    int next_order_id = 1;
    int current_time = 0;

    // Buy: highest price first
    std::map<int, std::queue<Order>, std::greater<int>> bids;

    // Sell: lowest price first
    std::map<int, std::queue<Order>> asks;
};
