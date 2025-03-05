#include <iostream>
#include <limits> 
using namespace std;

// Product Class
class Product {
public:
    int productId;
    const char* name;
    double price;
    int stockQuantity;

    Product(int id, const char* n, double p, int stock) : productId(id), name(n), price(p), stockQuantity(stock) {}
};

// ShoppingCart Class
class ShoppingCart {
private:
    Product* items[10];
    int quantities[10];
    int itemCount;

public:
    ShoppingCart() : itemCount(0) {}

    bool addProduct(Product* product, int quantity) {
        if (product->stockQuantity < quantity) {
            return false;
        }
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->productId == product->productId) {
                quantities[i] += quantity;
                cout << "\n[+] Product quantity updated successfully!\n";
                return true;
            }
        }
        items[itemCount] = product;
        quantities[itemCount++] = quantity;
        cout << "\n[+] Product added to cart successfully!\n";
        return true;
    }

    void displayCart() const {
        if (itemCount == 0) {
            cout << "\n[!] Shopping cart is empty.\n";
            return;
        }
        cout << "\nShopping Cart:\nID| Name          | Price | Quantity\n------------------------------------\n";
        for (int i = 0; i < itemCount; i++) {
            cout << items[i]->productId << " | " << items[i]->name << " | $"
                 << items[i]->price << "   | " << quantities[i] << "\n";
        }
    }

    double calculateTotal() const {
        double total = 0;
        for (int i = 0; i < itemCount; i++) {
            total += items[i]->price * quantities[i];
        }
        return total;
    }

    void clearCart() { itemCount = 0; }

    int getItemCount() const { return itemCount; }
};

// Order Class
class Order {
private:
    static int orderCounter;
    int orderId;
    double totalAmount;
    ShoppingCart cart;

public:
    Order() : orderId(0), totalAmount(0) {}

    Order(const ShoppingCart& c) : orderId(++orderCounter), totalAmount(c.calculateTotal()), cart(c) {}

    void displayOrderDetails() const {
        cout << "\n=====================================";
        cout << "\n Order Summary ";
        cout << "\n=====================================";
        cout << "\nOrder ID: " << orderId;
        cout << "\nTotal Amount: $" << totalAmount << "\n";
        cart.displayCart();
        cout << "=====================================\n";
    }
};
int Order::orderCounter = 0;

// Display Products
void displayProducts(Product* products[], int size) {
    cout << "\nAvailable Products:\nID| Name          | Price\n";
    cout << "------------------------------\n";
    for (int i = 0; i < size; i++) {
        cout << products[i]->productId << " | " << products[i]->name << " | $" << products[i]->price << "\n";
    }
}

int main() {
    Product p1(1, "Hammer       ", 25, 10);
    Product p2(2, "Screwdriver  ", 15, 12);
    Product p3(3, "Wrench       ", 30, 8);
    Product p4(4, "Pliers       ", 20, 10);
    Product p5(5, "Drill        ", 50, 5);
    Product* products[] = { &p1, &p2, &p3, &p4, &p5 };
    int productCount = 5;

    ShoppingCart cart;
    Order orderHistory[10];
    int orderCount = 0;

    while (true) {
        cout << "\n===================";
        cout << "\n Main Menu ";
        cout << "\n===================";
        cout << "\n1. View Products\n2. View Shopping Cart\n3. View Orders\n4. Exit\nEnter choice: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n[!] Invalid input! Please enter a valid number.\n";
            continue;
        }

        if (choice == 1) {
            displayProducts(products, productCount);
            int id, qty;
            while (true) {
                cout << "\nEnter Product ID to add to the Shopping Cart (press 0 to go back): ";
                cin >> id;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\n[!] Invalid input! Please enter a valid Product ID or 0 to go back.\n";
                    continue;
                }

                if (id == 0) break;

                Product* selectedProduct = nullptr;
                for (int i = 0; i < productCount; i++) {
                    if (products[i]->productId == id) {
                        selectedProduct = products[i];
                        break;
                    }
                }

                if (!selectedProduct) {
                    cout << "\n[!] Invalid Product ID!\n";
                    continue;
                }

                cout << "Enter quantity: ";
                cin >> qty;

                if (cin.fail() || qty <= 0 || selectedProduct->stockQuantity < qty) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\n[!] Invalid quantity!\n";
                    continue;
                }

                cart.addProduct(selectedProduct, qty);
            }
        }
        else if (choice == 2) {
            cart.displayCart();
            if (cart.getItemCount() > 0) {
                string confirm;
                while (true) {
                    cout << "\nProceed to checkout? (Y/N): ";
                    cin >> confirm;

                    if (confirm == "Y" || confirm == "y" || confirm == "N" || confirm == "n") {
                        break;
                    }

                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "[!] Invalid input! Please enter 'Y' or 'N'.\n";
                }

                if (confirm == "Y" || confirm == "y") {
                    orderHistory[orderCount++] = Order(cart);
                    cart.clearCart();
                    cout << "\n[+] Checkout successful!\n";
                }
            }
        }
        else if (choice == 3) {
            if (orderCount == 0) cout << "\n[!] No orders placed yet.\n";
            else for (int i = 0; i < orderCount; i++) orderHistory[i].displayOrderDetails();
        }
        else if (choice == 4) {
            cout << "\nThank you for shopping! Goodbye!\n";
            break;
        }
        else {
            cout << "\n[!] Invalid choice!\n";
        }
    }
    return 0;
}
