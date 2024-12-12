#ifndef MARKETPLACE_H
#define MARKETPLACE_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <limits>
#include <algorithm>

using namespace std;

// Абстрактный класс, определяющий интерфейс для всех стратегий оплаты
class PaymentStrategy {
public:
    // Виртуальный деструктор для корректного удаления объектов производного класса
    virtual ~PaymentStrategy() = default;

    // Метод, который должен быть реализован в производных классах
    // Он определяет способ оплаты определенной суммы
    virtual void pay(double amount) = 0;
};

// Класс, реализующий оплату наличными
class CashPayment : public PaymentStrategy {
public:
    // Переопределение метода pay для оплаты наличными
    void pay(double amount) override {
        cout << "Оплачено " << amount << " руб. наличными." << endl;
    }
};

// Класс, реализующий оплату картой
class CardPayment : public PaymentStrategy {
public:
    // Переопределение метода pay для оплаты картой
    void pay(double amount) override {
        cout << "Оплачено " << amount << " руб. картой." << endl;
    }
};

// Класс, реализующий оплату криптовалютой
class CryptoPayment : public PaymentStrategy {
public:
    // Переопределение метода pay для оплаты криптовалютой
    void pay(double amount) override {
        cout << "Оплачено " << amount << " руб. криптовалютой." << endl;
    }
};

// Класс Product описывает товар, включая: Название, цена,
// количество на складе, идентификатор продавца и хранится в списке товаров на
// площадке
class Product {
public:
    // Конструктор, инициализирующий товар
    Product(const string& name, double price, int quantity, int seller_id)
        : name(name), price(price), quantity(quantity), seller_id(seller_id) {}

    // Методы для получения информации о товаре
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getSellerId() const { return seller_id; }

    // Метод для уменьшения количества товара на складе
    void decreaseQuantity(int amount) {
        if (quantity >= amount) {
            quantity -= amount;
        } else {
            cout << "Недостаточно товара на складе." << endl;
        }
    }

private:
    string name; // Название товара
    double price; // Цена товара
    int quantity; // Количество товара на складе
    int seller_id; // ID продавца, который добавил этот товар
};

// Класс представляет продавца, который может добавлять товары на площадку
class Seller {
public:
    // Конструктор, инициализирующий продавца
    Seller(const string& name, int id) : name(name), id(id) {}

    // Метод для добавления товара на торговую площадку
    void addProduct(vector<Product>& products, const string& name, double price, int quantity) {
        products.emplace_back(name, price, quantity, id);
        cout << "Товар '" << name << "' добавлен на площадку." << endl;
    }

    // Методы для получения информации о продавце
    string getName() const { return name; }
    int getId() const { return id; }

private:
    string name; // Имя продавца
    int id; // Уникальный ID продавца
};

// Представляет собой покупателя, который может покупать товары на торговой площадке
class Customer {
public:
    // Конструктор, инициализирующий покупателя
    Customer(const string& name, double balance, int id) : name(name), balance(balance), id(id) {}

    // Метод для установки стратегии оплаты
    void setPaymentStrategy(unique_ptr<PaymentStrategy> strategy) {
        paymentStrategy = move(strategy);
    }

    // Метод для покупки товара
    bool buyProduct(Product& product, int quantity) {
        double totalCost = product.getPrice() * quantity;
        if (balance >= totalCost && product.getQuantity() >= quantity) {
            paymentStrategy->pay(totalCost);
            balance -= totalCost;
            product.decreaseQuantity(quantity);
            cout << "Покупка совершена. Остаток на балансе: " << balance << " руб." << endl;
            return true;
        } else {
            cout << "Недостаточно средств или товара на складе." << endl;
            return false;
        }
    }

    // Методы для получения информации о покупателе
    string getName() const { return name; }
    double getBalance() const { return balance; }
    int getId() const { return id; }

private:
    string name; // Имя покупателя
    double balance; // Баланс покупателя
    int id; // Уникальный ID покупателя
    unique_ptr<PaymentStrategy> paymentStrategy; // Стратегия оплаты, которую использует покупатель
};

// Базовый класс Marketplace, который
// управляет списком продавцов, покупателей и товаров. Обеспечивает интерфейс
// для добавления товаров, просмотра доступных товаров и управления сделками
class Marketplace {
public:
    // Метод для добавления продавца на торговую площадку
    void addSeller(const string& name) {
        sellers.emplace_back(name, nextSellerId++);
        cout << "Продавец '" << name << "' добавлен на площадку. ID: " << nextSellerId - 1 << endl;
    }

    // Метод для добавления покупателя на торговую площадку
    void addCustomer(const string& name, double balance) {
        customers.emplace_back(name, balance, nextCustomerId++);
        cout << "Покупатель '" << name << "' добавлен на площадку. ID: " << nextCustomerId - 1 << endl;
    }

    // Метод для добавления товара на торговую площадку от имени продавца
    void addProduct(int sellerId, const string& name, double price, int quantity) {
        for (auto& seller : sellers) {
            if (seller.getId() == sellerId) {
                seller.addProduct(products, name, price, quantity);
                return;
            }
        }
        cout << "Продавец с ID " << sellerId << " не найден." << endl;
    }

    // Метод для вывода списка всех доступных товаров на торговой площадке
    void listProducts() const {
        cout << "Доступные товары:" << endl;
        for (const auto& product : products) {
            cout << "Название: " << product.getName()
                      << ", Цена: " << product.getPrice()
                      << ", Количество: " << product.getQuantity()
                      << ", Продавец ID: " << product.getSellerId()
                      << ", Продавец: " << getSellerNameById(product.getSellerId()) << endl;
        }
    }

    // Метод для покупки товара покупателем
    void buyProduct(int customerId, const string& productName, int quantity) {
        auto customerIt = find_if(customers.begin(), customers.end(), [customerId](const Customer& c) { return c.getId() == customerId; });
        if (customerIt == customers.end()) {
            cout << "Покупатель с ID " << customerId << " не найден." << endl;
            return;
        }

        auto productIt = find_if(products.begin(), products.end(), [&productName](const Product& p) { return p.getName() == productName; });
        if (productIt == products.end()) {
            cout << "Товар с названием '" << productName << "' не найден." << endl;
            return;
        }

        if (customerIt->buyProduct(*productIt, quantity)) {
            generateReceipt(*customerIt, *productIt, quantity);
        }
    }

    // Метод для вывода списка всех зарегистрированных продавцов
    void listSellers() const {
        cout << "Зарегистрированные продавцы:" << endl;
        for (const auto& seller : sellers) {
            cout << "Имя: " << seller.getName() << ", ID: " << seller.getId() << endl;
        }
    }

    // Метод для вывода списка всех зарегистрированных покупателей
    void listCustomers() const {
        cout << "Зарегистрированные покупатели:" << endl;
        for (const auto& customer : customers) {
            cout << "Имя: " << customer.getName() << ", ID: " << customer.getId() << ", Баланс: " << customer.getBalance() << " руб." << endl;
        }
    }

    // Метод для генерации чека после покупки
    void generateReceipt(const Customer& customer, const Product& product, int quantity) const {
        double totalCost = product.getPrice() * quantity;
        cout << "Чек:" << endl;
        cout << "Покупатель: " << customer.getName() << ", ID: " << customer.getId() << endl;
        cout << "Товар: " << product.getName() << endl;
        cout << "Количество: " << quantity << endl;
        cout << "Цена за единицу: " << product.getPrice() << " руб." << endl;
        cout << "Общая стоимость: " << totalCost << " руб." << endl;
        cout << "Остаток на балансе: " << customer.getBalance() << " руб." << endl;
    }

    // Метод для получения имени продавца по его ID
    string getSellerNameById(int sellerId) const {
        auto sellerIt = find_if(sellers.begin(), sellers.end(), [sellerId](const Seller& s) { return s.getId() == sellerId; });
        if (sellerIt != sellers.end()) {
            return sellerIt->getName();
        }
        return "Неизвестный продавец";
    }

    // Метод для проверки существования продавца по его ID
    bool isValidSellerId(int sellerId) const {
        return find_if(sellers.begin(), sellers.end(), [sellerId](const Seller& s) { return s.getId() == sellerId; }) != sellers.end();
    }

    // Метод для проверки существования покупателя по его ID
    bool isValidCustomerId(int customerId) const {
        return find_if(customers.begin(), customers.end(), [customerId](const Customer& c) { return c.getId() == customerId; }) != customers.end();
    }

    // Методы для получения списков продавцов, покупателей и товаров
    vector<Seller>& getSellers() { return sellers; }
    vector<Customer>& getCustomers() { return customers; }
    vector<Product>& getProducts() { return products; }

private:
    vector<Seller> sellers; // Список продавцов на торговой площадке
    vector<Customer> customers; // Список покупателей на торговой площадке
    vector<Product> products; // Список товаров на торговой площадке
    int nextSellerId = 1; // Следующий доступный ID для продавца
    int nextCustomerId = 1; // Следующий доступный ID для покупателя
};

#endif