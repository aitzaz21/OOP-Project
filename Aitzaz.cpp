#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <memory>
#include <cctype>

using namespace std;

// Base class representing a generic item
class Item {
protected:
    string itemId;  // Unique identifier for the item
    string name;    // Name of the item
    double price;   // Price of the item
    int stock;      // Stock quantity of the item

public:
    Item(const string& id, const string& name, double price, int stock)
        : itemId(id), name(name), price(price), stock(stock) {}

    virtual void printInfo() const {
        cout << "Item ID: " << itemId << "\n"
            << "Name: " << name << "\n"
            << "Price: $" << fixed << setprecision(2) << price << "\n"
            << "Stock: " << stock << "\n";
    }

    string getItemId() const { return itemId; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
    void setStock(int newStock) { stock = newStock; }
    void setName(const string& newName) { name = newName; }
    void setPrice(double newPrice) { price = newPrice; }

    virtual ~Item() {}
};

// Derived class for products with additional attributes
class Product : public Item {
private:
    string description;
    string supplier;

public:
    Product(const string& id, const string& name, double price, const string& description, const string& supplier, int stock)
        : Item(id, name, price, stock), description(description), supplier(supplier) {}

    void printInfo() const override {
        Item::printInfo();
        cout << "Description: " << description << "\n"
            << "Supplier: " << supplier << "\n";
    }
};

// Inventory class to manage items
class Inventory {
private:
    vector<unique_ptr<Item>> items; // Use smart pointers

public:
    void addItem(unique_ptr<Item> item) {
        items.push_back(move(item));
    }

    void generateReport() const {
        if (items.empty()) {
            cout << "No items in inventory.\n";
        }
        else {
            cout << "\n=== Inventory Report ===\n";
            for (const auto& item : items) {
                item->printInfo();
                cout << "---------------------\n";
            }
        }
    }

    void removeItem(const string& itemId) {
        auto it = find_if(items.begin(), items.end(),
            [&itemId](const unique_ptr<Item>& item) { return item->getItemId() == itemId; });
        if (it != items.end()) {
            items.erase(it);
            cout << "Item with ID " << itemId << " has been successfully removed from the inventory.\n";
        }
        else {
            cout << "Error: Item with ID " << itemId << " not found in the inventory.\n";
        }
    }

    Item* findItem(const string& itemId) {
        for (const auto& item : items) {
            if (item->getItemId() == itemId) {
                return item.get();
            }
        }
        return nullptr;
    }

    void updateItemStock(const string& itemId, int newStock) {
        Item* item = findItem(itemId);
        if (item) {
            item->setStock(newStock);
            cout << "Stock for item ID " << itemId << " has been updated to " << newStock << ".\n";
        }
        else {
            cout << "Error: Item with ID " << itemId << " not found in the inventory.\n";
        }
    }

    bool isIdUnique(const string& itemId) const {
        return find_if(items.begin(), items.end(),
            [&itemId](const unique_ptr<Item>& item) { return item->getItemId() == itemId; }) == items.end();
    }
};

// Function declarations
int getValidatedInt(const string& prompt);
double getValidatedDouble(const string& prompt);
string getValidatedId();
string getValidatedString(const string& prompt);

void addProduct(Inventory& inventory) {
    string itemId = getValidatedId();
    if (!inventory.isIdUnique(itemId)) {
        cout << "Error: Item ID " << itemId << " already exists. Please use a unique ID.\n";
        return;
    }

    string name = getValidatedString("Enter product name (alphabetic characters only): ");
    double price = getValidatedDouble("Enter product price (non-negative integer): ");
    string description = getValidatedString("Enter product description (alphabetic characters only): ");
    string supplier = getValidatedString("Enter supplier name (alphabetic characters only): ");
    int stock = getValidatedInt("Enter initial stock quantity (non-negative integer): ");

    unique_ptr<Item> newProduct = make_unique<Product>(itemId, name, price, description, supplier, stock);
    inventory.addItem(move(newProduct));
    cout << "Product added successfully!\n";
}

string getValidatedId() {
    string itemId;
    while (true) {
        cout << "Enter item ID (4 characters): ";
        cin >> itemId;
        if (itemId.length() == 4) {
            return itemId;
        }
        else {
            cout << "Invalid input. Item ID must be exactly 4 characters long.\n";
        }
    }
}

string getValidatedString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        cin.ignore(); // Clear the input buffer
        getline(cin, input);
        if (all_of(input.begin(), input.end(), [](char c) { return isalpha(c); })) {
            return input;
        }
        else {
            cout << "Invalid input. Please enter alphabetic characters only.\n";
        }
    }
}

int getValidatedInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < 0) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a non-negative integer.\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            return value;
        }
    }
}

double getValidatedDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < 0) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a non-negative number.\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            return value;
        }
    }
}

void displayMenu() {
    cout << "=== Inventory Management System Menu ===\n"
        << "1. Add Product\n"
        << "2. Remove Item\n"
        << "3. Generate Inventory Report\n"
        << "4. View Item by ID\n"
        << "5. Update Item Stock\n"
        << "6. Exit\n";
}

int main() {
    Inventory inventory;
    int choice;

    do {
        displayMenu();
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addProduct(inventory);
            break;
        case 2: {
            string itemId;
            cout << "Enter item ID to remove: ";
            cin >> itemId;
            inventory.removeItem(itemId);
            break;
        }
        case 3:
            inventory.generateReport();
            break;
        case 4: {
            string itemId;
            cout << "Enter item ID to view: ";
            cin >> itemId;
            Item* item = inventory.findItem(itemId);
            if (item) {
                item->printInfo();
            }
            else {
                cout << "Error: Item with ID " << itemId << " not found.\n";
            }
            break;
        }
        case 5: {
            string itemId;
            int newStock;
            cout << "Enter item ID to update stock: ";
            cin >> itemId;
            newStock = getValidatedInt("Enter new stock quantity: ");
            inventory.updateItemStock(itemId, newStock);
            break;
        }
        case 6:
            cout << "Exiting the program.\n";
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}