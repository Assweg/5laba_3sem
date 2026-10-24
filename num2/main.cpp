#include "marketplace.h"
#include <iostream>
#include <memory>
#include <limits>

// Функция для отображения главного меню
void displayMainMenu() {
    cout << "1. Регистрация продавца" << endl;
    cout << "2. Регистрация покупателя" << endl;
    cout << "3. Вход как продавец" << endl;
    cout << "4. Вход как покупатель" << endl;
    cout << "5. Выход" << endl;
}

// Функция для отображения меню продавца
void displaySellerMenu() {
    cout << "1. Добавить товар" << endl;
    cout << "2. Просмотреть свои товары" << endl;
    cout << "3. Назад" << endl;
}

// Функция для отображения меню покупателя
void displayCustomerMenu() {
    cout << "1. Просмотреть товары" << endl;
    cout << "2. Купить товар" << endl;
    cout << "3. Назад" << endl;
}

// Функция для действий продавца
void sellerActions(Marketplace& marketplace, int sellerId) {
    int sellerChoice;
    do {
        displaySellerMenu();
        cout << "Выберите действие: ";
        cin >> sellerChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера ввода

        switch (sellerChoice) {
            case 1: {
                string productName;
                double price;
                int quantity;
                cout << "Введите название товара: ";
                getline(cin, productName);
                cout << "Введите цену товара: ";
                cin >> price;
                cout << "Введите количество товара: ";
                cin >> quantity;
                marketplace.addProduct(sellerId, productName, price, quantity);
                break;
            }
            case 2: {
                cout << "Ваши товары:" << endl;
                for (const auto& product : marketplace.getProducts()) {
                    if (product.getSellerId() == sellerId) {
                        cout << "Название: " << product.getName()
                                  << ", Цена: " << product.getPrice()
                                  << ", Количество: " << product.getQuantity() << endl;
                    }
                }
                break;
            }
            case 3:
                cout << "Возврат в главное меню." << endl;
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    } while (sellerChoice != 3);
}

// Функция для действий покупателя
void customerActions(Marketplace& marketplace, int customerId) {
    int customerChoice;
    do {
        displayCustomerMenu();
        cout << "Выберите действие: ";
        cin >> customerChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера ввода

        switch (customerChoice) {
            case 1:
                marketplace.listProducts();
                break;
            case 2: {
                string productName;
                int quantity;
                cout << "Введите название товара: ";
                getline(cin, productName);
                cout << "Введите количество товара: ";
                cin >> quantity;

                int paymentChoice;
                cout << "Выберите метод оплаты:" << endl;
                cout << "1. Наличные" << endl;
                cout << "2. Карта" << endl;
                cout << "3. Криптовалюта" << endl;
                cin >> paymentChoice;

                unique_ptr<PaymentStrategy> paymentStrategy;
                switch (paymentChoice) {
                    case 1:
                        paymentStrategy = make_unique<CashPayment>();
                        break;
                    case 2:
                        paymentStrategy = make_unique<CardPayment>();
                        break;
                    case 3:
                        paymentStrategy = make_unique<CryptoPayment>();
                        break;
                    default:
                        cout << "Неверный выбор метода оплаты." << endl;
                        continue;
                }

                auto customerIt = find_if(marketplace.getCustomers().begin(), marketplace.getCustomers().end(), [customerId](const Customer& c) { return c.getId() == customerId; });
                if (customerIt != marketplace.getCustomers().end()) {
                    customerIt->setPaymentStrategy(move(paymentStrategy));
                    marketplace.buyProduct(customerId, productName, quantity);
                } else {
                    cout << "Покупатель с ID " << customerId << " не найден." << endl;
                }
                break;
            }
            case 3:
                cout << "Возврат в главное меню." << endl;
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    } while (customerChoice != 3);
}

// Основная функция программы
int main() {
    Marketplace marketplace;
    int mainChoice;

    do {
        displayMainMenu();
        cout << "Выберите действие: ";
        cin >> mainChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера ввода

        switch (mainChoice) {
            case 1: {
                string sellerName;
                cout << "Введите имя продавца: ";
                getline(cin, sellerName);
                marketplace.addSeller(sellerName);
                break;
            }
            case 2: {
                string customerName;
                double balance;
                cout << "Введите имя покупателя: ";
                getline(cin, customerName);
                cout << "Введите баланс покупателя: ";
                cin >> balance;
                marketplace.addCustomer(customerName, balance);
                break;
            }
            case 3: {
                int sellerId;
                do {
                    marketplace.listSellers();
                    cout << "Введите ID продавца: ";
                    cin >> sellerId;
                    if (!marketplace.isValidSellerId(sellerId)) {
                        cout << "Продавец с ID " << sellerId << " не найден. Попробуйте снова." << endl;
                    }
                } while (!marketplace.isValidSellerId(sellerId));
                sellerActions(marketplace, sellerId);
                break;
            }
            case 4: {
                int customerId;
                do {
                    marketplace.listCustomers();
                    cout << "Введите ID покупателя: ";
                    cin >> customerId;
                    if (!marketplace.isValidCustomerId(customerId)) {
                        cout << "Покупатель с ID " << customerId << " не найден. Попробуйте снова." << endl;
                    }
                } while (!marketplace.isValidCustomerId(customerId));
                customerActions(marketplace, customerId);
                break;
            }
            case 5:
                cout << "Выход из программы." << endl;
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    } while (mainChoice != 5);

    return 0;
}