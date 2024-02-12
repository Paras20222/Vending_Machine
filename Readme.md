# Vending Machine System

## Overview
This project is a **multi-user Vending Machine system** that allows customers to add products to a cart, view the cart, and proceed to checkout, while an **owner** can restock products. The system is implemented in C++ with **file-based inventory management** and supports concurrency for safe transactions.

## Features
- **Customer Mode (Buyer)**
  - View available products with price and stock
  - Add products to cart with a specified quantity
  - Prevents adding more items than available stock
  - View cart with total price breakdown
  - Secure checkout with cash payment and automatic stock update
  
- **Owner Mode**
  - View all products and their stock levels
  - Restock products dynamically
  - Update product pricing if needed
  
- **File-based Inventory Management**
  - `input.txt` stores product details (ID, Name, Price, Stock)
  - `profit.txt` maintains transaction history
  - Updates stock levels dynamically after purchases

## Installation & Setup
### Prerequisites
- C++ compiler (GCC or Clang)
- Make (optional for build automation)
- Git (for version control)

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/Paras20222/Vending_Machine
   cd Vending_Machine
   ```
2. Compile the project:
   ```bash
   g++ -o vending Vending.cpp -pthread
   ```
3. Run the program:
   ```bash
   ./vending
   ```

## File Structure
```
Vending_Machine/
├── model.h       # Defines Product, Item, and Cart classes
├── buyer.h       # Implements Buyer functionalities
├── owner.h       # Implements Owner functionalities
├── Vending.cpp   # Main execution file
├── input.txt     # Stores product inventory
├── profit.txt    # Transaction records
├── README.md     # Project documentation
```

## How It Works
### Customer Workflow
1. Start the vending machine.
2. Select **Buyer Mode**.
3. View available products.
4. Choose a product and specify quantity.
5. Add to cart (checks stock availability automatically).
6. Checkout with payment (updates stock in `input.txt`).

### Owner Workflow
1. Start the vending machine.
2. Select **Owner Mode**.
3. View all products and stock levels.
4. Restock or update product prices.
5. Changes reflect in `input.txt`.

## Error Handling & Edge Cases
- **Invalid product selection**: Ensures input is within the available product list.
- **Quantity validation**: Prevents exceeding stock limits.
- **File I/O safety**: Uses temporary files for atomic updates.
- **Concurrency Handling**: Uses `mutex` for safe multi-threaded operations.

## Future Enhancements
- **GUI Interface** using Qt or GTK for better user interaction.
- **Database Integration** instead of file storage for scalability.
- **Card Payment Simulation** for a more realistic experience.
- **Admin Panel** with analytics and sales tracking.

## Contributing
Feel free to fork and contribute! Submit a pull request with detailed descriptions of changes.

## License
This project is open-source under the MIT License.

---
Developed by **Paras Kumar Sharma** 🚀

