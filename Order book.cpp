#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

class Order
{
    friend std::ostream& operator<<(std::ostream& out, Order& order) {
        std::cout << "Order {User ID: " << order.user_id << ", amount: " << order.amount
            << " UAH, price: $" << order.price << ", total price: $" << order.getTotalPrice()
            << ", side: " << (order.side == 0 ? "buying" : "selling") << "}";
        return out;
    }
private:
    int user_id;
    int amount; //amount of currencies that user wants to trade
    int price; //price for 1 UAH
    int total_price; //price * amount
    bool side; //0 = buy, 1 = sell currency
public:
    Order(int user_id, double amount, double price, bool side) {
        this->user_id = user_id;
        this->amount = amount;
        this->price = price;
        this->side = side;
        total_price = price * amount;
    }
    Order() {
        user_id = 0;
        amount = 0;
        price = 0;
        total_price = 0;
        side = false;
    }
    //Getters
    int getUserID() { return user_id; }
    int getAmount() { return amount; }
    int getPrice() { return price; }
    int getTotalPrice() { return total_price; }
    bool getSide() { return side; }
};

class OrderBook
{
    friend std::ostream& operator<<(std::ostream& out, OrderBook& order_book) {
        int i;
        std::cout << "-----List of sellers:-----\n";
        if (order_book.selling_people.size() != 0) {
            for (i = 0; i < order_book.selling_people.size(); i++) {
                std::cout << "Seller #" << i + 1 << std::endl;
                std::cout << *order_book.selling_people[i] << "\n\n";
            }
        }
        else std::cout << "~There are no sellers for now~\n\n";

        std::cout << "-----List of buyers:-----\n";
        if (order_book.buying_people.size() != 0) {
            for (i = 0; i < order_book.buying_people.size(); i++) {
                std::cout << "Buyer #" << i + 1 << std::endl;
                std::cout << *order_book.buying_people[i] << "\n\n";
            }
        }
        else std::cout << "~There are no buyers for now~\n\n";
        return out;
    }
private:
    void sort_selling_people() { //From lowest to highest
        bool mustSort = true;
        while (mustSort) {
            mustSort = false;
            for (int i = 0; i < selling_people.size() - 1; i++) {
                if (selling_people[i]->getPrice() > selling_people[i + 1]->getPrice()) {
                    Order* temp = selling_people[i];
                    selling_people[i] = selling_people[i + 1];
                    selling_people[i + 1] = temp;
                    mustSort = true;
                }
            }
        }
    }
    void sort_buying_people() { //From highest to lowest
        bool mustSort = true;
        while (mustSort) {
            mustSort = false;
            for (int i = 0; i < buying_people.size() - 1; i++) {
                if (buying_people[i]->getPrice() < buying_people[i + 1]->getPrice()) {
                    Order* temp = buying_people[i];
                    buying_people[i] = buying_people[i + 1];
                    buying_people[i + 1] = temp;
                    mustSort = true;
                }
            }
        }
    }

public:
    std::vector<Order*> selling_people; //Vectors for orders
    std::vector<Order*> buying_people;

    void push_order_into_selling(Order* order) {
        selling_people.push_back(order);
        sort_selling_people();
    }
    void push_order_into_buying(Order* order) {
        buying_people.push_back(order);
        sort_buying_people();
    }
};

void user_input(int& user_id, int& amount, int& price, bool& side);
Order* find_best_match(OrderBook& order_book, Order& my_order);
void show_balance_changes(Order& myorder, Order* matched_order);

int main()
{
    const int num = 10; //Number of orders in selling_people and buying_people
    srand(time(0));
    int user_id, amount, price; bool side;

    user_input(user_id, amount, price, side);

    Order myorder(user_id, amount, price, side);
    std::cout << "\nYour order:\n" << myorder << std::endl;
    
    OrderBook orderbook;
    side == 0 ? orderbook.push_order_into_buying(&myorder)
        : orderbook.push_order_into_selling(&myorder);

    int i; //Filling sells and buys lists using random
    for (i = 0; i < num; i++) {
        orderbook.push_order_into_selling(
            new Order(rand() % 1000 + 1000, rand() % 500 + 2, rand() % 100 + 1, 1));
        orderbook.push_order_into_buying(
            new Order(rand() % 1000 + 1000, rand() % 500 + 2, rand() % 100 + 1, 0));
    }
    std::cout << "\nOrder book:\n" << orderbook << std::endl;

    std::cout << "Finding a matches:\n";
    Order* matched_order = find_best_match(orderbook, myorder);
    if (matched_order) {
        std::cout << "Matched order:\n" << *matched_order << std::endl;
    }
    else {
        std::cout << "~There are no matches for now. Try later~\n"; return 0;
    }

    char willTheTradeBe;
    std::cout << "Do you want to trade? (y/n): "; std::cin >> willTheTradeBe;
    if (willTheTradeBe == 'y' || willTheTradeBe == 'Y') {
        show_balance_changes(myorder, matched_order);
    }
    else if (willTheTradeBe == 'n' || willTheTradeBe == 'N') std::cout << "Trade has been cancelled=(\n";

    return 0;
}

void user_input(int& user_id, int& amount, int& price, bool& side) {
    try {
        std::cout << "Enter your ID: "; std::cin >> user_id;
        if (std::cin.fail() || user_id <= 0) { throw std::runtime_error("Error: incorrect ID (less than 0 or not a num)"); }

        std::cout << "Enter 0 to buy UAH, 1 to sell UAH: "; std::cin >> side;
        if (std::cin.fail()) { throw std::runtime_error("Error: incorrect side (not 0 / 1 or not a num"); }

        std::cout << "Enter amount of UAH: "; std::cin >> amount;
        if (std::cin.fail() || amount <= 0) { throw std::runtime_error("Error: incorrect amount (less than 0 or not a num)"); }

        std::cout << "And the last, enter price for each UAH: "; std::cin >> price;
        if (std::cin.fail() || price <= 0) { throw std::runtime_error("Error: incorrect price (less than 0 or not a num)"); }
    }
    catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        std::exit(-1);
    }
}
Order* find_best_match(OrderBook& order_book, Order& my_order) {
    if (my_order.getSide() == 0) { // If we want to buy
        for (Order* item : order_book.selling_people) {
            if (item->getPrice() <= my_order.getPrice()) {
                return item;
            }
        }
    }
    else { // If we want to sell
        for (Order* item : order_book.buying_people) {
            if (item->getPrice() >= my_order.getPrice()) {
                return item;
            }
        }
    }
    return nullptr;
}
void show_balance_changes(Order& myorder, Order* matched_order) {
    std::cout << "\nYour balance {user ID: " << myorder.getUserID() << ", value: $";
    if (myorder.getSide() == 0) {
        if (myorder.getAmount() >= matched_order->getAmount()) {
            std::cout << -matched_order->getTotalPrice()
                << ", bought currency: " << matched_order->getAmount();
        }
        else {
            if (myorder.getTotalPrice() <= matched_order->getTotalPrice()) {
                std::cout << -myorder.getTotalPrice()
                    << ", bought currency: " << myorder.getTotalPrice() / matched_order->getPrice();
            }
            else {
                std::cout << -matched_order->getTotalPrice()
                    << ", bought currency: " << matched_order->getAmount();
            }
        }
    }
    else {
        if (myorder.getAmount() <= matched_order->getAmount()) {
            std::cout << "+" << myorder.getTotalPrice()
                << ", sold currency: " << myorder.getAmount();
        }
        else {
            if (myorder.getTotalPrice() >= matched_order->getTotalPrice()) {
                std::cout << "+" << matched_order->getTotalPrice()
                    << ", sold currency: " << matched_order->getTotalPrice() / myorder.getPrice();
            }
            else {
                std::cout << "+" << myorder.getTotalPrice() << ", sold currency: " << myorder.getAmount();
            }
        }
    }
    std::cout << " UAH}\n";

    std::cout << "User balance {user ID: " << matched_order->getUserID() << ", value: $";
    if (matched_order->getSide() == 0) {
        if (matched_order->getAmount() >= myorder.getAmount()) {
            std::cout << -myorder.getTotalPrice()
                << ", bought currency: " << myorder.getAmount();
        }
        else {
            if (matched_order->getTotalPrice() <= myorder.getTotalPrice()) {
                std::cout << -matched_order->getTotalPrice()
                    << ", bought currency: " << matched_order->getTotalPrice() / myorder.getPrice();
            }
            else {
                std::cout << -myorder.getTotalPrice() << ", bought currency: " << myorder.getAmount();
            }
        }
    }
    else {
        if (matched_order->getAmount() <= myorder.getAmount()) {
            std::cout << "+" << matched_order->getTotalPrice() << ", sold currency: "
                << matched_order->getAmount();
        }
        else {
            if (matched_order->getTotalPrice() >= myorder.getTotalPrice()) {
                std::cout << "+" << myorder.getTotalPrice()
                    << ", sold currency: " << myorder.getTotalPrice() / matched_order->getPrice();
            }
            else {
                std::cout << "+" << matched_order->getTotalPrice()
                    << ", sold currency: " << matched_order->getAmount();
            }
        }
    }
    std::cout << " UAH}\n";
}
