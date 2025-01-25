#ifndef OWNER_H
#define OWNER_H

#include "buyer.h"
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

// Function to load owner data from file
bool loadOwnerData(string& password, string& mobileNumber) {
    ifstream file("owner_data.txt");
    if (!file) {
        cout << "No owner data found. You must sign up first.\n";
        return false;
    }

    string line;
    getline(file, line);
    stringstream ss(line);
    ss >> mobileNumber >> password;  // Read mobile number and password
    file.close();
    return true;
}

// Function to store owner data to file
void storeOwnerData(const string& password, const string& mobileNumber) {
    ofstream file("owner_data.txt");
    file << mobileNumber << " " << password << endl;  // Save mobile number and password
    file.close();
}

// Function to allow the owner to reset their password after verification
void resetPassword(string& password, string& mobileNumber) {
    cout << "Enter mobile number for verification: ";
    string enteredMobile;
    cin >> enteredMobile;

    // Verify if the mobile number matches
    if (enteredMobile == mobileNumber) {
        cout << "Enter new password: ";
        string newPassword;
        cin >> newPassword;
        password = newPassword;  // Update the password
        storeOwnerData(password, mobileNumber);  // Save the new password
        cout << "Your password has been reset successfully.\n";
    } else {
        cout << "Mobile number does not match. Password reset failed.\n";
    }
}

void addItems() {
    cout << "(1) Add new product\n(2) Update Stock\n";
    int choice;
    cin >> choice;

    if (choice == 1) {
        int id = products.size() + 1; // New product ID based on existing product count

        // Prompt the owner to enter new product details
        cout << "Enter product name: ";
        string name;
        cin.ignore();  // To clear the input buffer
        getline(cin, name);

        cout << "Enter price: ";
        int price;
        cin >> price;

        cout << "Enter stock quantity: ";
        int stock;
        cin >> stock;

        // Add the new product to the file and to the product list
        ofstream outfile("input.txt", ios::app);
        outfile << id << " " << name << " " << price << " " << stock << "\n";
        outfile.close();

        // Update the products list in memory
        products.push_back(Product(id, name, price, stock));

        cout << "Product added successfully.\n";
    } 
    else if (choice == 2) {
        // Update stock functionality
        cout << "Enter product ID to update stock: ";
        int id;
        cin >> id;

        ifstream file("input.txt");
        ofstream tempFile("temp.txt");
        string line;

        bool found = false;
        while (getline(file, line)) {
            stringstream ss(line);
            string x;
            getline(ss, x, ' ');
            int cid = stoi(x);

            // If the product ID matches, update the stock
            if (cid == id) {
                cout << "Enter new stock quantity: ";
                int newStock;
                cin >> newStock;
                string updatedLine = x; // ID
                getline(ss, x, ' ');
                updatedLine += " " + x; // Name
                getline(ss, x, ' ');
                updatedLine += " " + x; // Price
                updatedLine += " " + to_string(newStock) + "\n"; // New stock

                tempFile << updatedLine;
                found = true;
            } else {
                tempFile << line << "\n"; // Copy unchanged lines
            }
        }

        file.close();
        tempFile.close();

        if (found) {
            // Replace the old file with the updated one
            remove("input.txt");
            rename("temp.txt", "input.txt");
            cout << "Stock updated successfully.\n";
        } else {
            cout << "Product ID not found.\n";
        }
    } 
    else {
        cout << "Invalid option.\n";
    }
}

/**
 * Function for the owner to sign in or reset the password.
 * Owner must provide the correct password or reset it.
 */
void owner() {
    string password;
    string mobileNumber;
    
    // Load owner data from file (if available)
    if (!loadOwnerData(password, mobileNumber)) {
        cout << "No owner data found. Please sign up.\n";
        
        // Sign up process
        cout << "Enter mobile number to sign up: ";
        cin >> mobileNumber;
        cout << "Enter a password: ";
        cin >> password;
        storeOwnerData(password, mobileNumber);  // Store owner data
        cout << "Owner signed up successfully.\n";
    }

    // Now that the owner is signed up or loaded, handle login
    string ipass;
    int attempts = 3;  // Allow 3 attempts

    while (true) {
        cout << "Enter password or type 'forgot' to reset password: ";
        cin >> ipass;

        if (ipass == password) {
            // Successful login, show options to the owner
            cout << "(1) Add Items\n(2) See Profit\n(3) Reset Password\n(4) Done\n";
            int action;
            cin >> action;

            if (action == 1) {
                addItems();  // Add new product or update stock
            } else if (action == 2) {
                seeProfit();  // Display total profit
            } else if (action == 3) {
                resetPassword(password, mobileNumber);  // Reset password after mobile verification
            } else if (action == 4) {
                break;  // Exit owner mode
            } else {
                cout << "Invalid action. Please select a valid option.\n";
            }
            break;  // Exit after successful login
        } 
        else if (ipass == "forgot") {
            // Forgot password option
            resetPassword(password, mobileNumber);  // Reset the password after verification
            break;  // Exit after resetting password
        } 
        else {
            // Wrong password case
            attempts--;  // Decrease attempt count on wrong password
            cout << "Wrong Password! You have " << attempts << " attempts left.\n";
        }

        if (attempts == 0) {
            cout << "Maximum attempts reached. Exiting...\n";
            return;  // Exit the owner mode after too many failed attempts
        }
    }
}

#endif // OWNER_H
