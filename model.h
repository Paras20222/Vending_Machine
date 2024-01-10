#ifndef MODEL_H
#define MODEL_H

#include <unordered_map>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <mutex>
#include <vector>

using namespace std;

class Product {
private:
    int id;
    string name;
    int price;
    int stock;

public:
    Product() = default;
    Product(int id, const string& name, int price, int stock)
        : id(id), name(name), price(price), stock(stock) {}

    string displayName() const {
        return to_string(id) + " -- " + name + "  Rs." + to_string(price) + "   Stock - " + to_string(stock) + "\n";
    }

    int getStock() const { return stock; }
    int getPrice() const { return price; }

    friend class Item;
    friend class Cart;
};

class Item {
private:
    Product product;
    int quantity;

public:
    Item() = default;
    Item(const Product& product, int quantity) : product(product), quantity(quantity) {}

    int getItemPrice() const { return quantity * product.price; }

    string getItemInfo() const {
        return " " + product.name + " x" + to_string(quantity) + "     Rs. " + to_string(quantity * product.price) + "\n";
    }

    friend class Cart;
};

class Cart {
private:
    unordered_map<int, Item> items;

public:
    void addProduct(const Product& product) {
        items[product.id] = Item(product, items.count(product.id) ? items[product.id].quantity + 1 : 1);
    }

    int getTotal() const {
        int total = 0;
        for (const auto& item : items) {
            total += item.second.getItemPrice();
        }
        return total;
    }

    string viewCart() const {
        stringstream ss;
        for (const auto& item : items) {
            ss << item.second.getItemInfo();
        }
        ss << " Total Price = Rs. " << getTotal() << "\n";
        return ss.str();
    }

    bool isEmpty() const { return items.empty(); }

    unordered_map<int, int> getCart() const {
        unordered_map<int, int> cart;
        for (const auto& item : items) {
            cart[item.first] = item.second.quantity;
        }
        return cart;
    }
};

bool getProducts(vector<Product>& products) {
    ifstream infile("input.txt");
    if (!infile) return false;

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        int id, price, stock;
        string name;
        ss >> id >> name >> price >> stock;
        products.push_back(Product(id, name, price, stock));
    }

    return true;
}

#endif
