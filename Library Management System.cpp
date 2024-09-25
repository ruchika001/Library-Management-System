#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Book class to represent each book
class Book {
private:
    string title;
    string author;
    bool isAvailable;

public:
    Book(string title, string author) : title(title), author(author), isAvailable(true) {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool getAvailability() const { return isAvailable; }

    void borrow() { isAvailable = false; }
    void returnBook() { isAvailable = true; }

    void display() const {
        cout << left << setw(30) << title
             << left << setw(30) << author
             << (isAvailable ? "Available" : "Not Available") << endl;
    }
};

// Library class to manage books
class Library {
private:
    vector<Book> books;

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    void displayBooks() const {
        cout << left << setw(30) << "Title"
             << left << setw(30) << "Author"
             << "Availability" << endl;
        cout << string(90, '-') << endl;

        for (const auto& book : books) {
            book.display();
        }
    }

    void borrowBook(const string& title) {
        for (auto& book : books) {
            if (book.getTitle() == title) {
                if (book.getAvailability()) {
                    book.borrow();
                    cout << "You have borrowed: " << title << endl;
                    return;
                } else {
                    cout << "Sorry, this book is not available." << endl;
                    return;
                }
            }
        }
        cout << "Book not found." << endl;
    }

    void returnBook(const string& title) {
        for (auto& book : books) {
            if (book.getTitle() == title) {
                book.returnBook();
                cout << "You have returned: " << title << endl;
                return;
            }
        }
        cout << "Book not found." << endl;
    }
};

// Function to save library data to a file
void saveLibraryData(const Library& library) {
    ofstream outFile("library_data.txt");
    if (outFile.is_open()) {
        for (const auto& book : library.books) {
            outFile << book.getTitle() << ","
                    << book.getAuthor() << ","
                    << book.getAvailability() << endl;
        }
        outFile.close();
    } else {
        cout << "Unable to open file." << endl;
    }
}

// Function to load library data from a file
void loadLibraryData(Library& library) {
    ifstream inFile("library_data.txt");
    string title, author;
    bool availability;
    string line;

    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            size_t pos = line.find(',');
            title = line.substr(0, pos);
            size_t nextPos = line.find(',', pos + 1);
            author = line.substr(pos + 1, nextPos - pos - 1);
            availability = line.substr(nextPos + 1) == "1";

            Book book(title, author);
            if (!availability) {
                book.borrow();  // Set as not available if borrowed
            }
            library.addBook(book);
        }
        inFile.close();
    } else {
        cout << "Unable to open file." << endl;
    }
}

int main() {
    Library library;
    loadLibraryData(library);

    int choice;
    string title, author;

    do {
        cout << "\nLibrary Management System" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. View Books" << endl;
        cout << "3. Borrow Book" << endl;
        cout << "4. Return Book" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                library.addBook(Book(title, author));
                saveLibraryData(library);  // Save after adding a book
                break;

            case 2:
                library.displayBooks();
                break;

            case 3:
                cout << "Enter the title of the book to borrow: ";
                cin.ignore();
                getline(cin, title);
                library.borrowBook(title);
                saveLibraryData(library);  // Save after borrowing a book
                break;

            case 4:
                cout << "Enter the title of the book to return: ";
                cin.ignore();
                getline(cin, title);
                library.returnBook(title);
                saveLibraryData(library);  // Save after returning a book
                break;

            case 5:
                cout << "Exiting..." << endl;
                break;

            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 5);

    return 0;
}
