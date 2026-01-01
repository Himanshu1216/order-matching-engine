#include "order_book.hpp"
#include <iostream>

int main() {
    OrderBook book;

    book.add_order(Side::BUY, 100, 50);
    book.add_order(Side::BUY, 100, 20);
    book.add_order(Side::SELL, 99, 60);

    return 0;
}
