#pragma once
#include "order.hpp"
#include <map>
#include <unordered_map>
#include <queue>

class OrderBook {
public:
    Order create_order(Side side, double price, int quantity);
    void add_order(Order& order);
    void cancel_order(int order_id);
    void modify_order(int order_id, int new_quantity, double new_price);
    Order get_order(int order_id);

private:
    void match_buy(Order& buy);
    void match_sell(Order& sell);   

    int next_order_id = 1;
    int current_time = 0;

    // Buy: highest price first
    std::map<int, std::list<Order>, std::greater<int>> bids;

    // Sell: lowest price first
    std::map<int, std::list<Order>> asks;

    std::unordered_map<int, OrderLocation> order_map;
};
