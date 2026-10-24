import subprocess
import tkinter as tk
from tkinter import messagebox, simpledialog

class MarketplaceGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Marketplace")

        # Главное меню
        self.menu_frame = tk.Frame(self.root)
        self.menu_frame.pack(pady=20)

        tk.Button(self.menu_frame, text="Регистрация продавца", command=self.register_seller).pack(pady=5)
        tk.Button(self.menu_frame, text="Регистрация покупателя", command=self.register_customer).pack(pady=5)
        tk.Button(self.menu_frame, text="Вход как продавец", command=self.login_seller).pack(pady=5)
        tk.Button(self.menu_frame, text="Вход как покупатель", command=self.login_customer).pack(pady=5)
        tk.Button(self.menu_frame, text="Выход", command=self.root.quit).pack(pady=5)

    def run_cpp_command(self, command):
        """Запуск C++ исполняемого файла с аргументами."""
        try:
            result = subprocess.run(["./marketplace"] + command, capture_output=True, text=True)
            if result.returncode == 0:
                messagebox.showinfo("Результат", result.stdout)
            else:
                messagebox.showerror("Ошибка", result.stderr)
        except Exception as e:
            messagebox.showerror("Ошибка", str(e))

    def register_seller(self):
        """Регистрация продавца."""
        name = simpledialog.askstring("Регистрация продавца", "Введите имя продавца:")
        if name:
            self.run_cpp_command(["addSeller", name])

    def register_customer(self):
        """Регистрация покупателя."""
        name = simpledialog.askstring("Регистрация покупателя", "Введите имя покупателя:")
        balance = simpledialog.askstring("Регистрация покупателя", "Введите баланс покупателя:")
        if name and balance:
            self.run_cpp_command(["addCustomer", name, balance])

    def login_seller(self):
        """Вход как продавец."""
        seller_id = simpledialog.askstring("Вход продавца", "Введите ID продавца:")
        if seller_id:
            self.run_cpp_command(["loginSeller", seller_id])

    def login_customer(self):
        """Вход как покупатель."""
        customer_id = simpledialog.askstring("Вход покупателя", "Введите ID покупателя:")
        if customer_id:
            self.run_cpp_command(["loginCustomer", customer_id])

# Запуск приложения
if __name__ == "__main__":
    root = tk.Tk()
    app = MarketplaceGUI(root)
    root.mainloop()