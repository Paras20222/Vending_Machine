#ifndef BUYER_H
#define BUYER_H

#include "model.h"
#include <unordered_map>
#include <iostream>
#include <mutex>

using namespace std;

extern mutex mtx;

string showAllProducts(const vector<Product>& products) {
    stringstream ss;
    for (const auto& product : products) {
        if (product.getStock() > 0) {
            ss << product.displayName();
        }
    }
    return ss.str();
}

Product* chooseProduct(vector<Product>& products) {
    cout << "Available Products:\n" << showAllProducts(products);
    cout << "Select product ID: ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= products.size()) {
        return &products[choice - 1];
    }

    cout << "Invalid product.\n";
    return nullptr;
}

bool checkout(Cart& cart, vector<Product>& products) {
    if (cart.isEmpty()) {
        cout << "Cart is empty.\n";
        return false;
    }

    int total = cart.getTotal();
    cout << "Total: Rs. " << total << ". Pay: ";
    int cash;
    cin >> cash;

    if (cash >= total) {
        // Update stock in product file
        unordered_map<int, int> cartContents = cart.getCart();
        ifstream file("input.txt");
        ofstream tempFile("temp.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            int productId;
            ss >> productId;

            if (cartContents.find(productId) != cartContents.end()) {
                int newStock = stoi(line) - cartContents[productId];
                line.replace(line.find_last_of(" "), string::npos, to_string(newStock));
            }
            tempFile << line << endl;
        }
        file.close();
        tempFile.close();

        remove("input.txt");
        rename("temp.txt", "input.txt");

        ofstream profitFile("profit.txt", ios::app);
        profitFile << cart.viewCart() + "\n";
        profitFile.close();

        cout << "Change: Rs. " << cash - total << "\n";
        return true;
    } else {
        cout << "Insufficient funds.\n";
        return false;
    }
}

void buyer(vector<Product>& products) {
    Cart cart;
    while (true) {
        cout << "(1) Add Item\n(2) View Cart\n(3) Checkout\n";
        int action;
        cin >> action;

        if (action == 1) {
            Product* p = chooseProduct(products);
            if (p) {
                lock_guard<mutex> lock(mtx);
                cart.addProduct(*p);
                cout << "Added to cart.\n";
            }
        } else if (action == 2) {
            cout << cart.viewCart();
        } else if (action == 3) {
            if (checkout(cart, products)) break;
        }
    }
}

#endif
