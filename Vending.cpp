#include <iostream>
#include <string>
#include "model.h"
#include "buyer.h"
#include "owner.h"

using namespace std;

// Global variables
vector<Product> products;  // Stores all products available for purchase
mutex mtx;                 // Mutex for thread-safe operations on shared resources

/**
 * Main function for the Vending Machine application.
 * Allows the user to choose between Buyer or Owner functionalities.
 */
int main() {
    // Display options to the user
    cout << "Choose: \n(1) Buyer\n(2) Owner\n";
    int choice;
    cin >> choice;

    // Load product data from input file
    getProducts(products);

    // Perform actions based on user choice
    if (choice == 1) {
        buyer();  // Buyer functionality
    } else if (choice == 2) {
        owner();  // Owner functionality
    } else {
        cout << "Invalid choice. Please try again.\n";
    }

    return 0;
}
