#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

class Book;

// Пункт 16: Дружня функція
void showPrivatePrice(const Book& b);

// ============================================================================
// ПУНКТ 1, 2, 4, 5, 8: Клас 1 — Book (Базовий клас)
// ============================================================================
class Book {
    friend void showPrivatePrice(const Book& b); // Пункт 16
    friend class Librarian;                       // Пункт 17

public:
    // Пункт 2: 3 public поля
    std::string title;
    std::string author;
    int year;

    // Пункт 18: Статичне поле та метод
    static int total_books;
    static void printTotal() {
        std::cout << "[Статичний метод] Всього книг: " << total_books << "\n";
    }

    // Пункт 4 & 5: Конструктори
    Book() : cat_code_(100), ref_tag_val_(year) {
        title = "Невідомо"; author = "Невідомо"; year = 0;
        pages_ = 0; isbn_ = "000"; price_ = 0.0; available_ = true; copies_ = 0;
        total_books++;
    }

    Book(std::string t, std::string a, int y, int p, std::string i, double pr, bool av, int c, int ref_val)
        : cat_code_(101), ref_tag_val_(ref_val) {
        title = t; author = a; year = y; pages_ = p; isbn_ = i;
        price_ = pr; available_ = av; copies_ = c;
        total_books++;
    }

    Book(const Book& o) : cat_code_(o.cat_code_), ref_tag_val_(o.ref_tag_val_) {
        title = o.title; author = o.author; year = o.year; pages_ = o.pages_;
        isbn_ = o.isbn_; price_ = o.price_; available_ = o.available_; copies_ = o.copies_;
        total_books++;
    }

    // Оператор присвоєння для усунення помилки C2280
    Book& operator=(const Book& o) {
        if (this != &o) {
            title = o.title; author = o.author; year = o.year; pages_ = o.pages_;
            isbn_ = o.isbn_; price_ = o.price_; available_ = o.available_; copies_ = o.copies_;
            ref_tag_val_ = o.ref_tag_val_;
        }
        return *this;
    }

    // Пункт 9, 13, 15: Деструктор
    virtual ~Book() {
        std::cout << "[Деструктор Book]: '" << title << "' вилучено.\n";
        total_books--;
    }

    // Пункт 2, 6: Методи опрацювання даних
    virtual void displayInfo() const {
        std::cout << "Книга: " << title << " | Автор: " << author << " | Рік: " << year
            << " | Стор: " << pages_ << " | ISBN: " << isbn_ << " | Ціна: " << price_
            << " грн | Копій: " << copies_ << "\n";
    }

    void markBorrowed() { if (copies_ > 0) { copies_--; if (copies_ == 0) available_ = false; } }
    void markReturned() { copies_++; available_ = true; }

    // Пункт 6: Перевантажені методи
    bool isSameAuthor(const Book& o) const { return (author == o.author); }
    Book createUpdatedEdition(int new_year) const { Book b = *this; b.year = new_year; return b; }

    // Пункт 7: Файлові методи
    void saveToFile(std::ofstream& f) const { f << title << "\n" << author << "\n" << year << "\n" << pages_ << "\n" << isbn_ << "\n" << price_ << "\n" << available_ << "\n" << copies_ << "\n"; }
    void loadFromFile(std::ifstream& f) { f >> title >> author >> year >> pages_ >> isbn_ >> price_ >> available_ >> copies_; }

    double getPrice() const { return price_; }

protected:
    // Пункт 8: protected елементи
    int pages_;
    std::string isbn_;

private:
    // Пункт 2: private елементи
    double price_;
    bool available_;
    int copies_;

    // Пункт 5: константа та значення посилання
    const int cat_code_;
    int ref_tag_val_;
};

int Book::total_books = 0;

// ============================================================================
// ПУНКТ 1, 8, 11, 12: Похідний Клас 2 — EBook (Відкрите наслідування)
// ============================================================================
class EBook : public Book {
public:
    double file_size;
    std::string format;
    std::string url;

    EBook() : Book() { file_size = 10.5; format = "PDF"; url = "http://link.ua"; }

    // Пункт 11: Передача параметрів базовому класу
    EBook(std::string t, std::string a, int y, int p, std::string i, double pr, bool av, int c, int ref_val, double size, std::string fmt, std::string u)
        : Book(t, a, y, p, i, pr, av, c, ref_val) {
        file_size = size; format = fmt; url = u;
    }

    ~EBook() override { std::cout << "[Деструктор EBook]: Е-книгу вилучено.\n"; }

    void displayInfo() const override {
        Book::displayInfo();
        std::cout << "  -> [EBook] Розмір: " << file_size << " МБ, Формат: " << format << "\n";
    }

    using Book::markBorrowed;
};

// ============================================================================
// ПУНКТ 9: Клас із Закритим наслідуванням (SecretBook)
// ============================================================================
class SecretBook : private Book {
public:
    int secret_level;
    SecretBook(std::string t, std::string a, int y, int ref_val)
        : Book(t, a, y, 100, "SECRET-01", 0.0, false, 1, ref_val) {
        secret_level = 5;
    }

    void printSecret() {
        std::cout << "[SecretBook] Секретний рівень " << secret_level << ": ";
        Book::displayInfo();
    }
};

// ============================================================================
// ПУНКТ 10: Класи для Множинного Наслідування
// ============================================================================
class Printer {
public:
    int dpi;
    Printer() : dpi(600) {}
    virtual ~Printer() { std::cout << "[Деструктор Printer]\n"; }
    void print() const { std::cout << "Друк з роздільністю " << dpi << " DPI\n"; }
};

class Digital {
public:
    std::string server;
    Digital() : server("AWS Cloud") {}
    virtual ~Digital() { std::cout << "[Деструктор Digital]\n"; }
    void sync() const { std::cout << "Синхронізація з сервером " << server << "\n"; }
};

class AudioBook : public Book, public Printer, public Digital {
public:
    int duration_min;
    AudioBook(std::string t, std::string a, int y, int ref_val, int dur)
        : Book(t, a, y, 0, "AUDIO-123", 150.0, true, 3, ref_val) {
        duration_min = dur;
    }

    ~AudioBook() override { std::cout << "[Деструктор AudioBook]\n"; }

    void displayInfo() const override {
        Book::displayInfo();
        std::cout << "  -> Тривалість аудіо: " << duration_min << " хв.\n";
        print(); sync();
    }
};

// ============================================================================
// ПУНКТ 1, 3, 20: Клас Reader (Композиція)
// ============================================================================
class Reader {
public:
    std::string full_name;
    int ticket_number;
    std::string reg_date;

    Book borrowed_book;

    Reader() { full_name = "Невідомо"; ticket_number = 0; reg_date = "01.01.2024"; blacklisted_ = false; }
    Reader(std::string name, int ticket, std::string date, bool bl, Book b) {
        full_name = name; ticket_number = ticket; reg_date = date; blacklisted_ = bl; borrowed_book = b;
    }
    ~Reader() { std::cout << "[Деструктор Reader]: Читач " << full_name << "\n"; }

    void displayReaderInfo() const {
        std::cout << "Читач: " << full_name << " | Квиток: " << ticket_number << " | В чорному списку: " << (blacklisted_ ? "Так" : "Ні") << "\n";
    }

    void setBlacklist(bool status) { blacklisted_ = status; }
    bool isBlacklisted() const { return blacklisted_; }

private:
    bool blacklisted_;
};

// ============================================================================
// ПУНКТ 17: Дружній клас Librarian
// ============================================================================
class Librarian {
public:
    std::string name;
    Librarian(std::string n) : name(n) {}
    void updatePrice(Book& b, double new_price) {
        b.price_ = new_price;
        std::cout << "[Дружній клас Librarian] " << name << " змінив ціну на " << b.price_ << " грн.\n";
    }
};

// ============================================================================
// ПУНКТ 16, 19, 20, 21: Функції поза межами класів
// ============================================================================
void showPrivatePrice(const Book& b) {
    std::cout << "[Дружня функція showPrivatePrice] Приватна ціна = " << b.price_ << " грн.\n";
}

void calculateTax(Book b) {
    std::cout << "[Пункт 19] ПДВ (20%) для '" << b.title << "': " << (b.getPrice() * 0.20) << " грн.\n";
}

void calculateTax(const Book* b_ptr) {
    if (b_ptr) std::cout << "[Пункт 20 - за адресою] ПДВ для '" << b_ptr->title << "': " << (b_ptr->getPrice() * 0.20) << " грн.\n";
}

Book makeSpecialEdition(const Book& base, std::string prefix) {
    Book temp = base;
    temp.title = prefix + " " + base.title;
    return temp;
}

// ============================================================================
// ГОЛОВНА ПРОГРАМА
// ============================================================================
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int dummy = 123;

    std::cout << "--- Пункт 22: 3 способи створення об'єктів ---\n";
    Book b1;
    Book b2 = Book("Кобзар", "Т.Шевченко", 1840, 350, "978-1", 250.0, true, 3, dummy);
    Book b3{ "Місто", "В.Підмогильний", 1928, 300, "978-3", 210.0, true, 5, dummy };

    std::cout << "\n--- Пункт 14, 23, 24: 5 об'єктів у статиці та 5 у динаміці ---\n";
    Book static_books[5] = {
        Book("Кобзар", "Т.Шевченко", 1840, 350, "978-1", 250.0, true, 3, dummy),
        Book("Тіні забутих предків", "М.Коцюбинський", 1911, 200, "978-2", 180.0, true, 2, dummy),
        Book("Місто", "В.Підмогильний", 1928, 300, "978-3", 210.0, true, 5, dummy),
        Book("Захар Беркут", "І.Франко", 1883, 240, "978-4", 190.0, true, 1, dummy),
        Book("Кайдашева сім'я", "І.Нечуй-Левицький", 1878, 180, "978-5", 150.0, true, 4, dummy)
    };

    Book* dynamic_books[5];
    for (int i = 0; i < 5; ++i) {
        dynamic_books[i] = new Book("Динамічна_Книга_" + std::to_string(i + 1), "Автор", 2020 + i, 100, "978-D", 100.0, true, 2, dummy);
    }

    for (int i = 0; i < 5; ++i) {
        static_books[i].displayInfo();
    }

    std::cout << "\n--- Пункт 8, 11, 12: Похідний клас EBook ---\n";
    EBook ebook("Енеїда", "І.Котляревський", 1798, 280, "978-E", 120.0, true, 10, dummy, 12.5, "EPUB", "http://link.ua");
    ebook.displayInfo();

    std::cout << "\n--- Пункт 9: Закрите наслідування (SecretBook) ---\n";
    SecretBook secret("Секретні архіви", "Невідомо", 1700, dummy);
    secret.printSecret();

    std::cout << "\n--- Пункт 10, 13: Множинне наслідування (AudioBook) ---\n";
    AudioBook audio("Аудіо-Кобзар", "Т.Шевченко", 1840, dummy, 120);
    audio.displayInfo();

    std::cout << "\n--- Пункт 16 & 17: Дружня функція та Дружній клас ---\n";
    showPrivatePrice(static_books[0]);
    Librarian lib("Ольга Іванівна");
    lib.updatePrice(static_books[0], 300.0);

    std::cout << "\n--- Пункт 18: Статичні елементи ---\n";
    Book::printTotal();

    std::cout << "\n--- Пункт 19, 20, 21: Позакласові функції ---\n";
    calculateTax(static_books[0]);
    calculateTax(&static_books[1]);
    Book spec = makeSpecialEdition(static_books[0], "Подарункове");
    spec.displayInfo();

    std::cout << "\n--- Пункт 26, 27: Сценарій функціонування ---\n";
    Reader reader("Іван Петренко", 101, "01.10.2025", false, static_books[1]);
    reader.displayReaderInfo();

    std::cout << "Читач запізнився із поверненням книги!\n";
    reader.setBlacklist(true);
    std::cout << "Статус чорного списку: " << (reader.isBlacklisted() ? "Внесено" : "Ні") << "\n";

    Book* ptr = &static_books[2];
    ptr->displayInfo();

    std::cout << "\n--- Пункт 7: Запис та читання з файлу ---\n";
    std::ofstream fout("data.txt");
    if (fout.is_open()) {
        static_books[0].saveToFile(fout);
        fout.close();
        std::cout << "Дані записано у файл data.txt\n";
    }

    std::ifstream fin("data.txt");
    if (fin.is_open()) {
        Book loaded;
        loaded.loadFromFile(fin);
        fin.close();
        std::cout << "Прочитано з файлу: ";
        loaded.displayInfo();
    }

    std::cout << "\n--- Очищення динамічної пам'яті ---\n";
    for (int i = 0; i < 5; ++i) {
        delete dynamic_books[i];
    }

    return 0;
}