#include "order_book.hpp"
#include <iostream>

int main() {
    OrderBook book;

    Order order1 = book.create_order(Side::BUY, 100, 50);
    book.add_order(order1);
    
    Order order2 = book.create_order(Side::BUY, 100, 20);
    book.add_order(order2);
    
    Order order3 = book.create_order(Side::SELL, 99, 60);
    book.add_order(order3);

    return 0;
}
