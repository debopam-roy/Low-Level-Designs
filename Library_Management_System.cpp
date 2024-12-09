#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

class Book {
private:
    string name, author, isbn;
    int publicationYear;
    bool availabilityStatus;

public:
    // Default constructor
    Book() : name(""), author(""), isbn(""), publicationYear(0), availabilityStatus(true) {}

    // Parameterized constructor
    Book(const string &name, int publicationYear, const string &author, const string &isbn)
        : name(name), publicationYear(publicationYear), author(author), isbn(isbn), availabilityStatus(true) {}

    string getISBN() const { return isbn; }
    string getName() const { return name; }
    string getAuthor() const { return author; }
    bool isAvailable() const { return availabilityStatus; }

    void setAvailability(bool status) { availabilityStatus = status; }
};

class Member {
private:
    static int idCounter;
    int id;
    string email, name, memberId;
    int numberOfBooksIssued;
    vector<string> booksIdList;

public:
    // Default constructor
    Member() : name(""), email(""), id(++idCounter), numberOfBooksIssued(0) {
        memberId = "M" + to_string(id);
    }

    // Parameterized constructor
    Member(const string &name, const string &email)
        : name(name), email(email), id(++idCounter), numberOfBooksIssued(0) {
        memberId = "M" + to_string(id);
    }

    string getMemberId() const { return memberId; }
    int getNumberOfBooksIssued() const { return numberOfBooksIssued; }
    string getName() const { return name; }

    void issueBook(const string &isbn) {
        booksIdList.push_back(isbn);
        numberOfBooksIssued++;
    }

    void returnBook(const string &isbn) {
        auto it = find(booksIdList.begin(), booksIdList.end(), isbn);
        if (it != booksIdList.end()) {
            booksIdList.erase(it);
            numberOfBooksIssued--;
        }
    }

    bool hasBook(const string &isbn) const {
        return find(booksIdList.begin(), booksIdList.end(), isbn) != booksIdList.end();
    }
};

class Library {
private:
    unordered_map<string, Book> books;   // Key: ISBN
    unordered_map<string, Member> members; // Key: Member ID

public:
    // Getter methods for books and members
    unordered_map<string, Book>& getBooks() { return books; }
    unordered_map<string, Member>& getMembers() { return members; }

    void addBook(const Book &book) {
        books[book.getISBN()] = book;
    }

    void removeBook(const string &isbn) {
        if (books.erase(isbn)) {
            cout << "Book with ISBN " << isbn << " removed successfully." << endl;
        } else {
            cout << "Book with ISBN " << isbn << " not found!" << endl;
        }
    }

    void addMember(const Member &member) {
        members[member.getMemberId()] = member;
    }

    void removeMember(const string &memberId) {
        if (members.erase(memberId)) {
            cout << "Member with ID " << memberId << " removed successfully." << endl;
        } else {
            cout << "Member with ID " << memberId << " not found!" << endl;
        }
    }

    void issueBook(const string &isbn, const string &memberId) {
        if (books.find(isbn) != books.end() && books[isbn].isAvailable()) {
            if (members.find(memberId) != members.end()) {
                Member &member = members[memberId];
                if (member.getNumberOfBooksIssued() < 5) {
                    books[isbn].setAvailability(false);
                    member.issueBook(isbn);
                    cout << "Book issued successfully to Member ID: " << memberId << endl;
                } else {
                    cout << "Member has reached the maximum book limit!" << endl;
                }
            } else {
                cout << "Member with ID " << memberId << " not found!" << endl;
            }
        } else {
            cout << "Book with ISBN " << isbn << " is unavailable or does not exist!" << endl;
        }
    }

    void returnBook(const string &isbn, const string &memberId) {
        if (members.find(memberId) != members.end()) {
            Member &member = members[memberId];
            if (member.hasBook(isbn)) {
                member.returnBook(isbn);
                if (books.find(isbn) != books.end()) {
                    books[isbn].setAvailability(true);
                }
                cout << "Book returned successfully by Member ID: " << memberId << endl;
            } else {
                cout << "Member does not have the book with ISBN " << isbn << "!" << endl;
            }
        } else {
            cout << "Member with ID " << memberId << " not found!" << endl;
        }
    }
};

int Member::idCounter = 0;

void displayMenu() {
    cout << "\nLibrary System Menu:" << endl;
    cout << "1. Add Book" << endl;
    cout << "2. Remove Book" << endl;
    cout << "3. Add Member" << endl;
    cout << "4. Remove Member" << endl;
    cout << "5. Issue Book" << endl;
    cout << "6. Return Book" << endl;
    cout << "7. View All Books" << endl;
    cout << "8. View All Members" << endl;
    cout << "9. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    Library library;

    // Adding books and members to demonstrate functionality
    library.addBook(Book("The Great Gatsby", 1925, "F. Scott Fitzgerald", "9780743273565"));
    library.addBook(Book("1984", 1949, "George Orwell", "9780451524935"));
    library.addMember(Member("John Doe", "john@example.com"));
    library.addMember(Member("Jane Smith", "jane@example.com"));

    // Direct function calls instead of threads
    library.issueBook("9780743273565", "M1");  // John issues "The Great Gatsby"
    library.returnBook("9780743273565", "M1"); // John returns "The Great Gatsby"
    library.issueBook("9780451524935", "M2");  // Jane issues "1984"

    // Simulate menu-driven user interaction for more operations
    int choice;
    bool exitProgram = false;

    while (!exitProgram) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, author, isbn;
                int publicationYear;

                cout << "Enter book name: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter author: ";
                getline(cin, author);

                cout << "Enter ISBN: ";
                getline(cin, isbn);

                cout << "Enter publication year: ";
                cin >> publicationYear;

                library.addBook(Book(name, publicationYear, author, isbn));
                cout << "Book added successfully!" << endl;
                break;
            }
            case 2: {
                string isbn;
                cout << "Enter ISBN of the book to remove: ";
                cin >> isbn;
                library.removeBook(isbn);
                break;
            }
            case 3: {
                string name, email;
                cout << "Enter member name: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter member email: ";
                getline(cin, email);

                library.addMember(Member(name, email));
                cout << "Member added successfully!" << endl;
                break;
            }
            case 4: {
                string memberId;
                cout << "Enter member ID to remove: ";
                cin >> memberId;
                library.removeMember(memberId);
                break;
            }
            case 5: {
                string isbn, memberId;
                cout << "Enter ISBN of the book to issue: ";
                cin >> isbn;

                cout << "Enter member ID to issue book: ";
                cin >> memberId;

                library.issueBook(isbn, memberId);
                break;
            }
            case 6: {
                string isbn, memberId;
                cout << "Enter ISBN of the book to return: ";
                cin >> isbn;

                cout << "Enter member ID to return book: ";
                cin >> memberId;

                library.returnBook(isbn, memberId);
                break;
            }
            case 7:
                cout << "Books in Library:" << endl;
                for (const auto &pair : library.getBooks()) {
                    const Book &book = pair.second;
                    cout << "Name: " << book.getName() << ", Author: " << book.getAuthor() << ", ISBN: " << book.getISBN() << endl;
                }
                break;
            case 8:
                cout << "Members in Library:" << endl;
                for (const auto &pair : library.getMembers()) {
                    const Member &member = pair.second;
                    cout << "ID: " << member.getMemberId() << ", Name: " << member.getName() << ", Books Issued: " << member.getNumberOfBooksIssued() << endl;
                }
                break;
            case 9:
                exitProgram = true;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
