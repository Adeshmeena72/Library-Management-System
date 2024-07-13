#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>

using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    bool isIssued;
    time_t dueDate;
    string issuedToUsername;
    string issuedToID;

    Book(int id, string title, string author) : id(id), title(title), author(author), isIssued(false), dueDate(0) {}
};

class User {
public:
    string username;
    string password;
    string userType; // "Student", "Teacher", or "Staff"
    string id; // Roll number for students, teacher ID for teachers, staff ID for staff

    User(string username, string password, string userType, string id) : username(username), password(password), userType(userType), id(id) {}
};

class Library {
private:
    vector<Book> books;
    unordered_map<int, string> issuedBooks; // Maps book ID to username

public:
    void addBook(int id, string title, string author) {
        books.push_back(Book(id, title, author));
        cout << "Book added successfully!" << endl;
    }

    void removeBook(int id, string userType) {
        if (userType == "Teacher" || userType == "Staff") {
            for (auto it = books.begin(); it != books.end(); ++it) {
                if (it->id == id) {
                    books.erase(it);
                    cout << "Book removed successfully!" << endl;
                    return;
                }
            }
            cout << "Book not found!" << endl;
        } else {
            cout << "Only teachers and staff can remove books!" << endl;
        }
    }

    void displayBooks() {
        if (books.empty()) {
            cout << "No books available!" << endl;
            return;
        }
        for (const auto& book : books) {
            cout << "ID: " << book.id << ", Title: " << book.title << ", Author: " << book.author << ", Issued: " << (book.isIssued ? "Yes" : "No");
            if (book.isIssued) {
                cout << ", Issued to: " << book.issuedToUsername << " (ID: " << book.issuedToID << ")";
            }
            cout << endl;
        }
    }

    void displaySpecificBook(int id) {
        for (const auto& book : books) {
            if (book.id == id) {
                cout << "ID: " << book.id << ", Title: " << book.title << ", Author: " << book.author << ", Issued: " << (book.isIssued ? "Yes" : "No");
                if (book.isIssued) {
                    cout << ", Issued to: " << book.issuedToUsername << " (ID: " << book.issuedToID << ")";
                }
                cout << endl;
                return;
            }
        }
        cout << "Book not found!" << endl;
    }

    void issueBook(int id, string username, string userID, int days, string userType) {
        if (userType == "Student" || userType == "Teacher"|| userType == "Staff") {
            for (auto& book : books) {
                if (book.id == id) {
                    if (!book.isIssued) {
                        book.isIssued = true;
                        book.issuedToUsername = username;
                        book.issuedToID = userID;
                        issuedBooks[id] = username;
                        time_t now = time(0);
                        book.dueDate = now + days * 24 * 60 * 60; // Due date in seconds
                        cout << "Book issued successfully to " << username << " (ID: " << userID << ") for " << days << " days!" << endl;
                    } else {
                        cout << "Book is already issued!" << endl;
                    }
                    return;
                }
            }
            cout << "Book not found!" << endl;
        } else {
            cout << "Only students and teachers can issue books!" << endl;
        }
    }

    void returnBook(int id, string username) {
        for (auto& book : books) {
            if (book.id == id && book.isIssued && issuedBooks[id] == username) {
                book.isIssued = false;
                issuedBooks.erase(id);
                time_t now = time(0);
                if (now > book.dueDate) {
                    double extraDays = difftime(now, book.dueDate) / (60 * 60 * 24);
                    double fine = extraDays * 10; // Assuming a fine of 10 units per day
                    cout << "Book returned late! Fine: " << fine << " units." << endl;
                } else {
                    cout << "Book returned on time!" << endl;
                }
                return;
            }
        }
        cout << "Book not found or not issued to this user!" << endl;
    }

    void displayIssuedBooks() {
        if (issuedBooks.empty()) {
            cout << "No books are currently issued." << endl;
            return;
        }
        for (const auto& entry : issuedBooks) {
            for (const auto& book : books) {
                if (book.id == entry.first) {
                    cout << "Book ID: " << book.id << ", Title: " << book.title << ", Issued to: " << book.issuedToUsername << " (ID: " << book.issuedToID << ")" << endl;
                }
            }
        }
    }

    int numberOfBooks() {
        return books.size();
    }

    void displayUserIssuedBooks(string username) {
        bool found = false;
        for (const auto& book : books) {
            if (book.issuedToUsername == username) {
                cout << "Book ID: " << book.id << ", Title: " << book.title << ", Due Date: " << ctime(&book.dueDate);
                found = true;
            }
        }
        if (!found) {
            cout << "No books issued to this user." << endl;
        }
    }
};

class LibrarySystem {
private:
    Library library;
    vector<User> users;
    User* loggedInUser = nullptr;

public:
    void registerUser(string username, string password, string userType, string id) {
        users.push_back(User(username, password, userType, id));
        cout << "User registered successfully!" << endl;
    }

    bool loginUser(string username, string password) {
        for (auto& user : users) {
            if (user.username == username && user.password == password) {
                loggedInUser = &user;
                cout << "Login successful!" << endl;
                return true;
            }
        }
        cout << "Invalid username or password!" << endl;
        return false;
    }

    void deleteUser(string username) {
        if (loggedInUser && (loggedInUser->userType == "Teacher" || loggedInUser->userType == "Staff")) {
            for (auto it = users.begin(); it != users.end(); ++it) {
                if (it->username == username) {
                    users.erase(it);
                    cout << "User deleted successfully!" << endl;
                    return;
                }
            }
            cout << "User not found!" << endl;
        } else {
            cout << "Only teachers and staff can delete users!" << endl;
        }
    }

    void displaySpecificUser(string id) {
        for (const auto& user : users) {
            if (user.id == id) {
                cout << "Username: " << user.username << ", UserType: " << user.userType << ", ID: " << user.id << endl;
                library.displayUserIssuedBooks(id);
                return;
            }
        }
        cout << "User not found!" << endl;
    }

    bool isUserRegistered(string username) {
        for (const auto& user : users) {
            if (user.username == username) {
                return true;
            }
        }
        return false;
    }

    bool isIDRegistered(string id) {
        for (const auto& user : users) {
            if (user.id == id) {
                return true;
            }
        }
        return false;
    }

    void menu() {
        int choice;
        do {
            cout << "\nLibrary Management System\n";
            cout << "1. Add Book\n";
            cout << "2. Remove Book\n";
            cout << "3. Display Books\n";
            cout << "4. Register User\n";
            cout << "5. Login User\n";
            cout << "6. Issue Book\n";
            cout << "7. Return Book\n";
            cout << "8. Delete User\n";
            cout << "9. Number of Books\n";
            cout << "10. Display Specific Book\n";
            cout << "11. Display Specific User\n";
            cout << "12. Display Issued Books\n";
            cout << "13. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    if (loggedInUser && (loggedInUser->userType == "Teacher" || loggedInUser->userType == "Staff")) {
                        int id;
                        string title, author;
                        cout << "Enter book ID: ";
                        cin >> id;
                        cout << "Enter book title: ";
                        cin.ignore();
                        getline(cin, title);
                        cout << "Enter book author: ";
                        getline(cin, author);
                                                library.addBook(id, title, author);
                    } else {
                        cout << "Only teachers and staff can add books!" << endl;
                    }
                    break;
                }
                case 2: {
                    if (loggedInUser && (loggedInUser->userType == "Teacher" || loggedInUser->userType == "Staff")) {
                        int id;
                        cout << "Enter book ID to remove: ";
                        cin >> id;
                        library.removeBook(id, loggedInUser->userType);
                    } else {
                        cout << "Only teachers and staff can remove books!" << endl;
                    }
                    break;
                }
                case 3:
                    library.displayBooks();
                    break;
                case 4: {
                    string username, password, userType, id;
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                    cout << "Enter user type (Student/Teacher/Staff): ";
                    cin >> userType;
                    cout << "Enter ID (Roll number for students, Teacher ID for teachers, Staff ID for staff): ";
                    cin >> id;
                    registerUser(username, password, userType, id);
                    break;
                }
                case 5: {
                    string username, password;
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                    loginUser(username, password);
                    break;
                }
                case 6: {
                    int id, days;
                    string username, userID;
                    cout << "Enter book ID to issue: ";
                    cin >> id;
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter user ID (Roll number for students, Teacher ID for teachers): ";
                    cin >> userID;
                    if (!isUserRegistered(username) || !isIDRegistered(userID)) {
                        cout << "User not registered. Please register the user first." << endl;
                        break;
                    }
                    cout << "Enter number of days: ";
                    cin >> days;
                    library.issueBook(id, username, userID, days, loggedInUser->userType);
                    break;
                }
                case 7: {
                    int id;
                    string username;
                    cout << "Enter book ID to return: ";
                    cin >> id;
                    cout << "Enter username: ";
                    cin >> username;
                    library.returnBook(id, username);
                    break;
                }
                case 8: {
                    if (loggedInUser && (loggedInUser->userType == "Teacher" || loggedInUser->userType == "Staff")) {
                        string username;
                        cout << "Enter username to delete: ";
                        cin >> username;
                        deleteUser(username);
                    } else {
                        cout << "Only teachers and staff can delete users!" << endl;
                    }
                    break;
                }
                case 9:
                    cout << "Number of books: " << library.numberOfBooks() << endl;
                    break;
                case 10: {
                    int id;
                    cout << "Enter book ID to display: ";
                    cin >> id;
                    library.displaySpecificBook(id);
                    break;
                }
                case 11: {
                    string username;
                    cout << "Enter userID to display: ";
                    cin >> username;
                    displaySpecificUser(username);
                    break;
                }
                case 12:
                    library.displayIssuedBooks();
                    break;
                case 13:
                    cout << "Exiting..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Returning to main menu." << endl;
            }
        } while (choice != 13);
    }
};

int main() {
    LibrarySystem system;
    string username, password;
    bool loggedIn = false;

    while (!loggedIn) {
        cout << "Library Management System\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                string username, password, userType, id;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                cout << "Enter user type (Student/Teacher/Staff): ";
                cin >> userType;
                cout << "Enter ID (Roll number for students, Teacher ID for teachers, Staff ID for staff): ";
                cin >> id;
                system.registerUser(username, password, userType, id);
                break;
            }
            case 2: {
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                loggedIn = system.loginUser(username, password);
                break;
            }
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
        }
    }

    system.menu();
    return 0;
}
