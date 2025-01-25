#ifndef BUYER_H
#define BUYER_H

#include "model.h"   // Include necessary files
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <mutex>

using namespace std;

// Externally declared variables that are defined in Vending.cpp
extern vector<Product> products;
extern mutex mtx;

/**
 * Displays all products available for purchase by the owner.
 * Shows product details, including ID, name, price, and stock.
 */
string showAllProductsOwner() {
    stringstream ss;
    for (const auto& product : products) {
        ss << product.displayName();
    }
    return ss.str();
}

/**
 * Displays only the products that are available for purchase (i.e., those with stock > 0).
 */
string showAllProducts() {
    stringstream ss;
    for (const auto& product : products) {
        if (product.getStock() > 0) {
            ss << product.displayName();
        }
    }
    return ss.str();
}

/**
 * Prompts the user to choose a product and returns a pointer to the selected product.
 * Returns nullptr if the selected product is invalid.
 */
Product* chooseProduct() {
    cout << "Available Products:\n" << showAllProducts();
    cout << "Select the number of the Product: ";
    int choice;
    cin >> choice;
    if (choice > products.size()) {
        cout << "Product not found!\n";
        return nullptr;
    }
    return &products[choice - 1];
}

/**
 * Handles the checkout process. If the user has enough cash, it updates the stock
 * and records the transaction.
 */
bool checkout(Cart& cart) {
    if (cart.isEmpty()) {
        cout << "Cart is empty!\n";
        return false;
    }

    unordered_map<int, int> bought = cart.getCart();
    int total = cart.getTotal();
    cout << "Please pay a total of Rs. " << total << endl;
    int cash;
    cin >> cash;

    if (cash >= total) {
        ifstream file("input.txt");
        ofstream ofile("temp.txt");
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string newline, x;
            getline(ss, x, ' ');
            newline += x + " ";
            int id = stoi(x);
            getline(ss, x, ' ');
            newline += x + " ";
            getline(ss, x, ' ');
            newline += x + " ";
            getline(ss, x, ' ');
            newline += to_string(stoi(x) - bought[id]) + "\n";
            ofile << newline;
        }
        file.close();
        ofile.close();
        remove("input.txt");
        rename("temp.txt", "input.txt");

        ofstream outfile("profit.txt", ios::app);
        outfile << cart.viewCart() + "\n";
        outfile.close();
        cout << "Change: Rs. " << cash - total << "\n";
        cout << "Thank you for shopping :)\n";
        return true;
    } else {
        cout << "Insufficient funds\n";
        return false;
    }
}

/**
 * Main function for the buyer. Allows the buyer to add products to their cart,
 * view their cart, and proceed to checkout.
 */
void buyer() {
    int action;
    Cart cart;
    while (true) {
        // Display the options for the buyer
        cout << "(1) Add Item\n(2) View Cart\n(3) Checkout\n";
        cin >> action;

        if (action == 1) {
            // Add item to cart
            Product* p = chooseProduct();  // Get product to add
            if (p != nullptr) {
                lock_guard<std::mutex> lock(mtx);  // Ensure thread safety
                cart.addProduct(*p);  // Add product to cart
                cout << "Product added to cart.\n";
            }
        } else if (action == 2) {
            // View the current cart
            cout << cart.viewCart();
        } else if (action == 3) {
            // Proceed to checkout
            if (checkout(cart)) {
                break;  // Exit the loop after successful checkout
            }
        } else {
            cout << "Invalid action. Please select a valid option.\n";
        }
    }
}

#endif // BUYER_H
