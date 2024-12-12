#include "marketplace.h"
#include <iostream>
#include <memory>
#include <limits>

// Функция для отображения главного меню
void displayMainMenu() {
    std::cout << "1. Регистрация продавца" << std::endl;
    std::cout << "2. Регистрация покупателя" << std::endl;
    std::cout << "3. Вход как продавец" << std::endl;
    std::cout << "4. Вход как покупатель" << std::endl;
    std::cout << "5. Выход" << std::endl;
}

// Функция для отображения меню продавца
void displaySellerMenu() {
    std::cout << "1. Добавить товар" << std::endl;
    std::cout << "2. Просмотреть свои товары" << std::endl;
    std::cout << "3. Назад" << std::endl;
}

// Функция для отображения меню покупателя
void displayCustomerMenu() {
    std::cout << "1. Просмотреть товары" << std::endl;
    std::cout << "2. Купить товар" << std::endl;
    std::cout << "3. Назад" << std::endl;
}

// Функция для действий продавца
void sellerActions(Marketplace& marketplace, int sellerId) {
    int sellerChoice;
    do {
        displaySellerMenu();
        std::cout << "Выберите действие: ";
        std::cin >> sellerChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода

        switch (sellerChoice) {
            case 1: {
                std::string productName;
                double price;
                int quantity;
                std::cout << "Введите название товара: ";
                std::getline(std::cin, productName);
                std::cout << "Введите цену товара: ";
                std::cin >> price;
                std::cout << "Введите количество товара: ";
                std::cin >> quantity;
                marketplace.addProduct(sellerId, productName, price, quantity);
                break;
            }
            case 2: {
                std::cout << "Ваши товары:" << std::endl;
                for (const auto& product : marketplace.getProducts()) {
                    if (product.getSellerId() == sellerId) {
                        std::cout << "Название: " << product.getName()
                                  << ", Цена: " << product.getPrice()
                                  << ", Количество: " << product.getQuantity() << std::endl;
                    }
                }
                break;
            }
            case 3:
                std::cout << "Возврат в главное меню." << std::endl;
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
        }
    } while (sellerChoice != 3);
}

// Функция для действий покупателя
void customerActions(Marketplace& marketplace, int customerId) {
    int customerChoice;
    do {
        displayCustomerMenu();
        std::cout << "Выберите действие: ";
        std::cin >> customerChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода

        switch (customerChoice) {
            case 1:
                marketplace.listProducts();
                break;
            case 2: {
                std::string productName;
                int quantity;
                std::cout << "Введите название товара: ";
                std::getline(std::cin, productName);
                std::cout << "Введите количество товара: ";
                std::cin >> quantity;

                int paymentChoice;
                std::cout << "Выберите метод оплаты:" << std::endl;
                std::cout << "1. Наличные" << std::endl;
                std::cout << "2. Карта" << std::endl;
                std::cout << "3. Криптовалюта" << std::endl;
                std::cin >> paymentChoice;

                std::unique_ptr<PaymentStrategy> paymentStrategy;
                switch (paymentChoice) {
                    case 1:
                        paymentStrategy = std::make_unique<CashPayment>();
                        break;
                    case 2:
                        paymentStrategy = std::make_unique<CardPayment>();
                        break;
                    case 3:
                        paymentStrategy = std::make_unique<CryptoPayment>();
                        break;
                    default:
                        std::cout << "Неверный выбор метода оплаты." << std::endl;
                        continue;
                }

                auto customerIt = std::find_if(marketplace.getCustomers().begin(), marketplace.getCustomers().end(), [customerId](const Customer& c) { return c.getId() == customerId; });
                if (customerIt != marketplace.getCustomers().end()) {
                    customerIt->setPaymentStrategy(std::move(paymentStrategy));
                    marketplace.buyProduct(customerId, productName, quantity);
                } else {
                    std::cout << "Покупатель с ID " << customerId << " не найден." << std::endl;
                }
                break;
            }
            case 3:
                std::cout << "Возврат в главное меню." << std::endl;
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
        }
    } while (customerChoice != 3);
}

// Основная функция программы
int main() {
    Marketplace marketplace;
    int mainChoice;

    do {
        displayMainMenu();
        std::cout << "Выберите действие: ";
        std::cin >> mainChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода

        switch (mainChoice) {
            case 1: {
                std::string sellerName;
                std::cout << "Введите имя продавца: ";
                std::getline(std::cin, sellerName);
                marketplace.addSeller(sellerName);
                break;
            }
            case 2: {
                std::string customerName;
                double balance;
                std::cout << "Введите имя покупателя: ";
                std::getline(std::cin, customerName);
                std::cout << "Введите баланс покупателя: ";
                std::cin >> balance;
                marketplace.addCustomer(customerName, balance);
                break;
            }
            case 3: {
                int sellerId;
                do {
                    marketplace.listSellers();
                    std::cout << "Введите ID продавца: ";
                    std::cin >> sellerId;
                    if (!marketplace.isValidSellerId(sellerId)) {
                        std::cout << "Продавец с ID " << sellerId << " не найден. Попробуйте снова." << std::endl;
                    }
                } while (!marketplace.isValidSellerId(sellerId));
                sellerActions(marketplace, sellerId);
                break;
            }
            case 4: {
                int customerId;
                do {
                    marketplace.listCustomers();
                    std::cout << "Введите ID покупателя: ";
                    std::cin >> customerId;
                    if (!marketplace.isValidCustomerId(customerId)) {
                        std::cout << "Покупатель с ID " << customerId << " не найден. Попробуйте снова." << std::endl;
                    }
                } while (!marketplace.isValidCustomerId(customerId));
                customerActions(marketplace, customerId);
                break;
            }
            case 5:
                std::cout << "Выход из программы." << std::endl;
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
        }
    } while (mainChoice != 5);

    return 0;
}