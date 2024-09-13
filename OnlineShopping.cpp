#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm> // For transform and toupper
#include <limits>    // For numeric limits

using namespace std;

// Class for Product
class Product {
public:
    string productID;  // ID for product
    string productName; // Name of Product
    float price;        // Price of the Product  

    Product(string id, string name, float price) : productID(id), productName(name), price(price) {}
};

// Class for Shopping Cart
// Holds product and quantities for each customer
class ShoppingCart {
public:
    vector<pair<Product, int>> products;

    void addProduct(const Product& product, int quantity) {
        for (auto& item : products) {
            if (item.first.productID == product.productID) {
                item.second += quantity;
                cout << "Product quantity updated!" << endl;
                return;
            }
        }
        products.push_back(make_pair(product, quantity));
        cout << "Product added successfully!" << endl;
    }

    void displayCart() const {
        if (products.empty()) {
            cout << "Your cart is empty." << endl;
            return;
        }

        cout << left << setw(12) << "Product ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << endl;
        for (const auto& item : products) {
            cout << left << setw(12) << item.first.productID << setw(20) << item.first.productName << setw(10) << item.first.price << setw(10) << item.second << endl;
        }
    }

    bool isEmpty() const {
        return products.empty();
    }

    float calculateTotalAmount() const {
        float total = 0;
        for (const auto& item : products) {
            total += item.first.price * item.second;
        }
        return total;
    }
};

// Class for Order
class Order {
public:
    int orderID;
    ShoppingCart cart;
    float totalAmount;

    Order(int id, const ShoppingCart& cart) : orderID(id), cart(cart), totalAmount(cart.calculateTotalAmount()) {}

    void displayOrder() const {
        cout << "Order ID: " << orderID << endl;
        cout << "Total Amount: " << totalAmount << endl;
        cout << "Order Details:" << endl;
        cart.displayCart();
    }
};

// Class for Customer
class Customer {
public:
    int customerID;
    string name;
    string email;
    ShoppingCart cart;
    vector<Order> orders;

    Customer(int id, string name, string email) : customerID(id), name(name), email(email) {}

    void checkout() {
        if (cart.isEmpty()) {
            cout << "Your cart is empty. Cannot checkout." << endl;
            return;
        }

        int newOrderID = orders.size() + 1;
        Order newOrder(newOrderID, cart);
        orders.push_back(newOrder);

        cart.products.clear();
        cout << "Checkout successful!" << endl;
    }

    void displayOrders() const {
        if (orders.empty()) {
            cout << "No orders found." << endl;
            return;
        }

        for (const auto& order : orders) {
            cout << "Order ID: " << order.orderID << endl;
            cout << "Total Amount: " << order.totalAmount << endl;
            cout << "Order Details:" << endl;
            cout << left << setw(12) << "Product ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << endl;
            for (const auto& item : order.cart.products) {
                cout << left << setw(12) << item.first.productID << setw(20) << item.first.productName << setw(10) << item.first.price << setw(10) << item.second << endl;
            }
            cout << endl;
        }
    }
};

// Utility function to convert a string to uppercase 
string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Function to display all available products in the system
void displayProducts(const vector<Product>& productList) {
    cout << left << setw(12) << "Product ID" << setw(20) << "Name" << setw(10) << "Price" << endl;

    for (const auto& product : productList) {
        cout << left << setw(12) << product.productID << setw(20) << product.productName << setw(10) << product.price << endl;
    }
}

// Function to get a valid integer input within a specified range
int getValidIntInput(int min, int max) {
    int input;
    while (true) {
        cin >> input;
        if (cin.fail() || input < min || input > max) {
            cin.clear(); // clear the error flag on cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
            cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear any additional input
            return input;
        }
    }
}

// Main Function
int main() {
    int customerID;
    string name, email;

    cout << "Register a new customer" << endl;
    cout << "Enter Customer ID: ";
    customerID = getValidIntInput(1, 99);
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Email: ";
    getline(cin, email);

    Customer customer(customerID, name, email);

    vector<Product> productList = {
        Product("ABC", "Paper", 20.00),
        Product("CDE", "Pencil", 10.00),
        Product("QWE", "Pen", 15.00),
        Product("RTY", "Eraser", 5.00),
        Product("UIO", "Notebook", 50.0),
        Product("PAS", "Ruler", 12.0),
        Product("DFG", "Marker", 25.0),
        Product("HJK", "Glue Stick", 8.0),
        Product("LZX", "Scissors", 30.0),
        Product("CVB", "Highlighter", 18.0),
        Product("FEK", "Cutter", 7.00)
    };

    int choice;
    string productID;
    int quantity;

    do {
        cout << "\nMenu" << endl;
        cout << "1 - View Products" << endl;
        cout << "2 - View Shopping Cart" << endl;
        cout << "3 - View Orders" << endl;
        cout << "4 - Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                displayProducts(productList);

                cout << "Enter the Product ID to add to cart (or 'exit' to return to menu): ";
                getline(cin, productID);

                if (productID == "exit") {
                    break;
                }

                productID = toUpperCase(productID);

                cout << "Enter quantity: ";
                quantity = getValidIntInput(1, 99);

                {
                    bool productFound = false;
                    for (const auto& product : productList) {
                        if (product.productID == productID) {
                            customer.cart.addProduct(product, quantity);
                            productFound = true;
                            break;
                        }
                    }

                    if (!productFound) {
                        cout << "Invalid Product ID!" << endl;
                    }
                }
                break;

            case 2:
                customer.cart.displayCart();
                if (!customer.cart.isEmpty()) {
                    char checkoutChoice;
                    cout << "Do you want to checkout all the products? (Y/N): ";
                    cin >> checkoutChoice;
                    cin.ignore();

                    if (checkoutChoice == 'Y' || checkoutChoice == 'y') {
                        customer.checkout();
                    }
                }
                break;

            case 3:
                customer.displayOrders();
                break;

            case 4:
                cout << "Logging out..." << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 4);

    return 0;
}
