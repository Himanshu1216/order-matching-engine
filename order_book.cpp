#include "order_book.hpp"
#include <iostream>
#include <algorithm>
#include <list>

Order OrderBook::create_order(Side side, double price, int quantity) {
    Order order {
        next_order_id++,
        side,
        price,
        quantity,
        current_time++
    };
    return order;
}

Order OrderBook::get_order(int order_id) {
    auto it = order_map.find(order_id);
    if (it == order_map.end()) {
        std::cerr << "Order ID " << order_id << " not found.\n";
    }
    auto &loc = it->second;
    return *(loc.it);
}

void OrderBook::add_order(Order& order) {
    if (order.side == Side::BUY) {
        match_buy(order);
        if (order.quantity > 0) {
            bids[order.price].push_back(order);
            order_map[order.order_id] = {order.side, order.price, std::prev(bids[order.price].end())};
        }
    } else {
        match_sell(order);
        if (order.quantity > 0) {
            asks[order.price].push_back(order);
            order_map[order.order_id] = {order.side, order.price, std::prev(asks[order.price].end())};
        }
    }
}

void OrderBook::match_buy(Order& buy) {
    while (buy.quantity > 0 && !asks.empty()) {
        auto best_ask_it = asks.begin();
        int ask_price = best_ask_it->first;

        if (ask_price > buy.price)
            break;

        auto& order_list = best_ask_it->second;
        Order& sell = order_list.front();

        int traded_qty = std::min(buy.quantity, sell.quantity);

        std::cout << "TRADE price=" << ask_price
                  << " qty=" << traded_qty
                  << " (buy_id=" << buy.order_id
                  << " sell_id=" << sell.order_id << ")\n";

        buy.quantity -= traded_qty;
        sell.quantity -= traded_qty;

        if (sell.quantity == 0) {
            order_map.erase(sell.order_id);
            order_list.pop_front();
            if (order_list.empty())
                asks.erase(best_ask_it);
        }
    }
}

void OrderBook::match_sell(Order& sell) {
    while (sell.quantity > 0 && !bids.empty()) {
        auto best_bid_it = bids.begin();
        int bid_price = best_bid_it->first;

        if (bid_price < sell.price)
            break;

        auto& order_list = best_bid_it->second;
        Order& buy = order_list.front();

        int traded_qty = std::min(sell.quantity, buy.quantity);

        std::cout << "TRADE price=" << bid_price
                  << " qty=" << traded_qty
                  << " (buy_id=" << buy.order_id
                  << " sell_id=" << sell.order_id << ")\n";

        sell.quantity -= traded_qty;
        buy.quantity -= traded_qty;

        if (buy.quantity == 0) {
            order_map.erase(buy.order_id);
            order_list.pop_front();
            if (order_list.empty())
                bids.erase(best_bid_it);
        }
    }
}

void OrderBook::cancel_order(int order_id) {
    auto it = order_map.find(order_id);
    if (it == order_map.end()) {
        std::cerr << "Order ID " << order_id << " not found for cancellation.\n";
        return;
    }
    auto &loc = it->second;
    auto &order_list = (loc.side == Side::BUY) ? bids[loc.price] : asks[loc.price];
    order_list.erase(loc.it);
    if (order_list.empty()) {
        if (loc.side == Side::BUY) {
            bids.erase(loc.price);
        } else {
            asks.erase(loc.price);  
        }
    }
    order_map.erase(it);
}

void OrderBook::modify_order(int order_id, int new_quantity, double new_price) {
    auto it = order_map.find(order_id);
    if (it == order_map.end()) {
        std::cerr << "Order ID " << order_id << " not found for modification.\n";
        return;
    }
    OrderLocation& loc = it->second;
    Order& old_order = *(loc.it);

    bool price_changed = (old_order.price != new_price);
    bool qty_increased = (new_quantity > old_order.quantity);

    if (!price_changed && !qty_increased) {
        old_order.quantity = new_quantity;
    } else {
        cancel_order(order_id);
        Order new_order{
            order_id,
            old_order.side,
            new_price,
            new_quantity,
            current_time++
        };
        add_order(new_order);
    }
    return;
}