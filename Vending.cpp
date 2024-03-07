#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <mutex>
#include "model.h"
#include "buyer.h"
#include "owner.h"

using namespace std;

mutex mtx;  // Mutex for thread-safe operations

// Function to read and display profit data
void seeProfit() {
    ifstream profitFile("profit.txt");  // Open the profit file
    string line;
    if (!profitFile) {
        cout << "Error: Could not open profit file.\n";
        return;
    }

    cout << "Profit Report:\n";
    while (getline(profitFile, line)) {  // Read each line of the file
        cout << line << endl;  // Display the profit
    }

    profitFile.close();  // Close the file after reading
}

// Function to add or update items
void addItems(vector<Product>& products) {
    cout << "(1) Add new product\n(2) Update Stock\n";
    int choice;
    cin >> choice;

    if (choice == 1) {
        int id = products.size() + 1;  // Generate new product ID based on existing products

        string name;
        int price, stock;

        cout << "Enter product name: ";
        cin.ignore();  // To ignore the newline character left by previous input
        getline(cin, name);
        cout << "Enter price: ";
        cin >> price;
        cout << "Enter stock quantity: ";
        cin >> stock;

        // Save the new product to the file and add it to the products vector
        ofstream outfile("input.txt", ios::app);
        outfile << id << " " << name << " " << price << " " << stock << "\n";
        products.push_back(Product(id, name, price, stock));
        cout << "Product added successfully.\n";
    } else if (choice == 2) {
        cout << "Enter product ID to update stock: ";
        int id;
        cin >> id;

        ifstream file("input.txt");
        ofstream tempFile("temp.txt");
        string line;
        bool found = false;

        while (getline(file, line)) {
            stringstream ss(line);
            int productId;
            ss >> productId;

            if (productId == id) {
                int newStock;
                cout << "Enter new stock quantity: ";
                cin >> newStock;
                line = to_string(id) + " " + to_string(newStock) + "\n";
                found = true;
            }
            tempFile << line << endl;
        }

        file.close();
        tempFile.close();

        if (found) {
            remove("input.txt");
            rename("temp.txt", "input.txt");
            cout << "Stock updated.\n";
        } else {
            cout << "Product not found.\n";
        }
    }
}

// Function for Owner to either Register or Sign In
void owner(vector<Product>& products) {
    string username, password;

    // Ask if user is a new user or an existing user
    cout << "(1) New User (Register)\n(2) Sign In (Existing User)\n";
    int choice;
    cin >> choice;

    if (choice == 1) {
        // New user registration
        cout << "No owner data found. Please register.\n";
        cout << "Enter a username: ";
        cin >> username;
        cout << "Enter a password: ";
        cin >> password;

        // Save the new user data to the file
        ofstream file("owner_data.txt");
        if (file) {
            file << username << " " << password << endl;
            cout << "Registration successful. You can now sign in.\n";
        } else {
            cout << "Error during registration.\n";
        }
    } else if (choice == 2) {
        // Try to load existing owner data from the file
        ifstream file("owner_data.txt");
        if (!file) {
            cout << "Error: No owner data found. Please register first.\n";
            return;
        }

        file >> username >> password;

        // Sign in with username and password
        string enteredUsername, enteredPassword;
        int attempts = 3;
        while (attempts-- > 0) {
            cout << "Enter username: ";
            cin >> enteredUsername;
            cout << "Enter password: ";
            cin >> enteredPassword;

            // Check if entered username and password match
            if (enteredUsername == username && enteredPassword == password) {
                cout << "Sign-in successful.\n";
                break;
            } else {
                cout << "Incorrect username or password. " << attempts << " attempts left.\n";
            }
        }

        if (attempts == 0) {
            cout << "Too many failed attempts. Exiting...\n";
            return;
        }
    } else {
        cout << "Invalid option. Exiting...\n";
        return;
    }

    // After sign-in, owner can choose to add items, view profit, or exit
    string action;
    cout << "(1) Add Items\n(2) View Profit\n(3) Exit\n";
    int actionChoice;
    cin >> actionChoice;
    switch (actionChoice) {
        case 1:
            addItems(products);  // Add or update items
            break;
        case 2:
            seeProfit();  // View profit
            break;
        case 3:
            return;  // Exit
        default:
            cout << "Invalid choice.\n";
            break;
    }
}

// Enum for user roles
enum class UserRole {
    BUYER = 1,
    OWNER = 2
};

// Display main menu to the user
UserRole getMainMenuChoice() {
    int choice;
    cout << "--------------------------------------\n";
    cout << "            VENDING MACHINE           \n";
    cout << "--------------------------------------\n";
    cout << "Please choose an option:\n";
    cout << "[1] Buyer\n[2] Owner\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case static_cast<int>(UserRole::BUYER): return UserRole::BUYER;
        case static_cast<int>(UserRole::OWNER): return UserRole::OWNER;
        default: 
            cout << "Invalid choice. Try again.\n";
            return static_cast<UserRole>(0);
    }
}

int main() {
    vector<Product> products;

    // Load products
    if (!getProducts(products)) {
        cerr << "Error: Failed to load products.\n";
        return 1;
    }

    // Get user choice
    UserRole choice = getMainMenuChoice();
    switch (choice) {
        case UserRole::BUYER: buyer(products); break;
        case UserRole::OWNER: owner(products); break;
        default: return 1;
    }

    return 0;
}
