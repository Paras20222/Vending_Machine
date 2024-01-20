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

Product* chooseProduct(vector<Product>& products, int& quantity) {
    cout << "Available Products:\n" << showAllProducts(products);
    cout << "Select product ID: ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= products.size()) {
        Product* selectedProduct = &products[choice - 1];

        cout << "Enter quantity: ";
        cin >> quantity;

        if (quantity > 0 && quantity <= selectedProduct->getStock()) {
            return selectedProduct;
        } else {
            cout << "Insufficient stock available. Only " << selectedProduct->getStock() << " in stock.\n";
            return nullptr;
        }
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
    long long cash;  // Use long long to handle large inputs
    cin >> cash;

    if (cash >= total) {
        unordered_map<int, int> cartContents = cart.getCart();
        ifstream file("input.txt");
        ofstream tempFile("temp.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            int productId, stock;
            string name;
            
            ss >> productId >> name >> stock;  // Extract product ID, name, and stock properly

            if (cartContents.find(productId) != cartContents.end()) {
                stock -= cartContents[productId];  // Deduct purchased quantity
            }

            tempFile << productId << " " << name << " " << stock << endl;
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
            int quantity;
            Product* p = chooseProduct(products, quantity);

            if (p) {
                lock_guard<mutex> lock(mtx);
                cart.addProduct(*p, quantity);  
                cout << "Added " << quantity << " units to cart.\n";
            }
        } else if (action == 2) {
            cout << cart.viewCart();
        } else if (action == 3) {
            if (checkout(cart, products)) break;
        }
    }
}

#endif
