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

/**
 * Class representing a product in the vending machine.
 * Each product has an ID, name, price, and stock count.
 */
class Product {
private:
    int id;         // Unique identifier for the product
    string name;    // Name of the product
    int price;      // Price of the product in INR
    int stock;      // Available stock for the product

public:
    // Default constructor and parameterized constructor for initialization
    Product() = default;
    Product(int id, const string& name, int price, int stock) 
        : id(id), name(name), price(price), stock(stock) {}

    // Returns a string displaying the product's details
    string displayName() const {
        return to_string(id) + " -- " + name + "  Rs." + to_string(price) + "   Stock - " + to_string(stock) + "\n";
    }

    // Getter for stock
    int getStock() const { return stock; }

    friend class Item;  // Allow Item to access private members of Product
    friend class Cart;  // Allow Cart to access private members of Product
};

/**
 * Class representing a single item in the shopping cart.
 * Contains a product and its quantity in the cart.
 */
class Item {
private:
    Product product;  // The product in the cart
    int quantity;     // Quantity of the product in the cart

public:
    // Default constructor and parameterized constructor
    Item() = default;
    Item(const Product& product, int quantity) : product(product), quantity(quantity) {}

    // Returns the total price of the item (product price * quantity)
    int getItemPrice() const { return quantity * product.price; }

    // Returns a string displaying the item's details
    string getItemInfo() const {
        return " " + product.name + " x" + to_string(quantity) + "     Rs. " + to_string(quantity * product.price) + "\n";
    }

    friend class Cart;  // Allow Cart to access private members of Item
};

/**
 * Class representing a shopping cart that holds items added by the buyer.
 */
class Cart {
private:
    unordered_map<int, Item> items;  // Map of product ID to item (product + quantity)

public:
    // Adds a product to the cart (or increases its quantity if already present)
    void addProduct(const Product& product) {
        if (items.count(product.id) == 0) {
            items[product.id] = Item(product, 1);
        } else {
            items[product.id].quantity++;
        }
    }

    // Calculates and returns the total price of all items in the cart
    int getTotal() const {
        int total = 0;
        for (const auto& item : items) {
            total += item.second.getItemPrice();
        }
        return total;
    }

    // Displays all items in the cart
    string viewCart() const {
        if (items.empty()) return "Cart is empty!";
        stringstream ss;
        for (const auto& item : items) {
            ss << item.second.getItemInfo();
        }
        ss << " Total Price = Rs. " << getTotal() << "\n";
        return ss.str();
    }

    // Checks if the cart is empty
    bool isEmpty() const { return items.empty(); }

    // Returns a map of product ID to quantity for all items in the cart
    unordered_map<int, int> getCart() const {
        unordered_map<int, int> cart;
        for (const auto& item : items) {
            cart[item.first] = item.second.quantity;
        }
        return cart;
    }
};

/**
 * Thread-safe function to load products from the input file.
 * The products vector will be populated with the products from the file.
 */
void getProducts(vector<Product>& products) {
    products.clear();
    ifstream infile("input.txt");
    if (!infile) {
        cerr << "Error: Unable to open product file.\n";
        return;
    }

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string x;
        getline(ss, x, ' ');
        int id = stoi(x);
        string name;
        getline(ss, name, ' ');
        getline(ss, x, ' ');
        int price = stoi(x);
        getline(ss, x, ' ');
        int stock = stoi(x);
        products.push_back(Product(id, name, price, stock));
    }
    infile.close();
}

#endif // MODEL_H
