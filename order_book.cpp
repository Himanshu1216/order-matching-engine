#include "order_book.hpp"
#include <iostream>
#include <algorithm>

void OrderBook::add_order(Side side, double price, int quantity) {
    Order order {
        next_order_id++,
        side,
        price,
        quantity,
        current_time++
    };

    if (side == Side::BUY) {
        match_buy(order);
        if (order.quantity > 0) {
            bids[order.price].push(order);
        }
    } else {
        match_sell(order);
        if (order.quantity > 0) {
            asks[order.price].push(order);
        }
    }
}

void OrderBook::match_buy(Order& buy) {
    while (buy.quantity > 0 && !asks.empty()) {
        auto best_ask_it = asks.begin();
        int ask_price = best_ask_it->first;

        if (ask_price > buy.price)
            break;

        auto& queue = best_ask_it->second;
        Order& sell = queue.front();

        int traded_qty = std::min(buy.quantity, sell.quantity);

        std::cout << "TRADE price=" << ask_price
                  << " qty=" << traded_qty
                  << " (buy_id=" << buy.order_id
                  << " sell_id=" << sell.order_id << ")\n";

        buy.quantity -= traded_qty;
        sell.quantity -= traded_qty;

        if (sell.quantity == 0) {
            queue.pop();
            if (queue.empty())
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

        auto& queue = best_bid_it->second;
        Order& buy = queue.front();

        int traded_qty = std::min(sell.quantity, buy.quantity);

        std::cout << "TRADE price=" << bid_price
                  << " qty=" << traded_qty
                  << " (buy_id=" << buy.order_id
                  << " sell_id=" << sell.order_id << ")\n";

        sell.quantity -= traded_qty;
        buy.quantity -= traded_qty;

        if (buy.quantity == 0) {
            queue.pop();
            if (queue.empty())
                bids.erase(best_bid_it);
        }
    }
}
