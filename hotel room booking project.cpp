#include<iostream>
#include<fstream>
#include<iomanip>
#include <string>
using namespace std;

// Base Class → Room
class Room {
protected:
    int roomNo;
    string roomType;
    float pricePerDay;

public:
    Room(int r = 0, string type = "", float price = 0.0) {
        roomNo = r;
        roomType = type;
        pricePerDay = price;
    }

    void displayRoom() {
        cout << setw(5) << roomNo << setw(15) << roomType
             << setw(12) << pricePerDay << "/day" << endl;
    }

    int getRoomNo() { return roomNo; }
    float getPrice() { return pricePerDay; }
    string getType() { return roomType; }
};

// Derived Class → Booking
class Booking : public Room {
    string customerName;
    int days;

public:
    Booking(int r = 0, string type = "", float price = 0.0,
            string cname = "", int d = 0)
        : Room(r, type, price), customerName(cname), days(d) {}

    float calculateBill() {
        return pricePerDay * days;
    }

    void displayBooking() {
        cout << setw(5) << roomNo << setw(15) << roomType
             << setw(15) << customerName << setw(8) << days
             << setw(10) << calculateBill() << endl;
    }

    void saveBookingToFile() {
        ofstream file("bookings.txt", ios::app); // append mode
        if (file.is_open()) {
            file << roomNo << " | " << roomType << " | "
                 << customerName << " | " << days << " | "
                 << calculateBill() << endl;
            file.close();
        }
    }
};

// Function to cancel booking (remove record from file)
void cancelBooking(int roomNo, string customerName) {
    ifstream inFile("bookings.txt");
    ofstream outFile("temp.txt");
    string line;
    bool found = false;

    if (!inFile.is_open()) {
        cout << "No bookings available to cancel.\n";
        return;
    }

    while (getline(inFile, line)) {
        if (line.find(to_string(roomNo)) != string::npos &&
            line.find(customerName) != string::npos) {
            cout << "Booking found and cancelled ✅\n";
            found = true;
            continue; // skip writing this line
        }
        outFile << line << endl;
    }

    inFile.close();
    outFile.close();

    remove("bookings.txt");
    rename("temp.txt", "bookings.txt");

    if (!found) {
        cout << "No matching booking found ❌\n";
    }
}

// Function to search booking by Customer Name or Room No
void searchBooking() {
    ifstream file("bookings.txt");
    if (!file.is_open()) {
        cout << "No bookings found.\n";
        return;
    }

    int choice;
    cout << "\nSearch Booking By:\n";
    cout << "1. Customer Name\n";
    cout << "2. Room Number\n";
    cout << "Enter choice: ";
    cin >> choice;

    cin.ignore();
    string query;
    if (choice == 1) {
        cout << "Enter Customer Name: ";
        getline(cin, query);
    } else if (choice == 2) {
        cout << "Enter Room Number: ";
        getline(cin, query);
    } else {
        cout << "Invalid choice!\n";
        return;
    }

    string line;
    bool found = false;
    cout << "\n===== Search Results =====\n";
    while (getline(file, line)) {
        if (line.find(query) != string::npos) {
            cout << line << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No matching booking found ❌\n";
    }
    file.close();
}

int main() {
    // Predefined rooms
    Room rooms[3] = {
        Room(101, "Single", 1000),
        Room(102, "Double", 1800),
        Room(103, "Deluxe", 2500)
    };

    int choice;
    do {
        cout << "\n===== HOTEL ROOM BOOKING SYSTEM =====\n";
        cout << "1. Show Available Rooms\n";
        cout << "2. Book a Room\n";
        cout << "3. View All Bookings (from file)\n";
        cout << "4. Cancel Booking\n";
        cout << "5. Search Booking\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "\nAvailable Rooms:\n";
            cout << setw(5) << "No" << setw(15) << "Type"
                 << setw(15) << "Price" << endl;
            for (int i = 0; i < 3; i++) {
                rooms[i].displayRoom();
            }
        }

        else if (choice == 2) {
            int rno, days;
            string cname;
            bool found = false;

            cout << "Enter Room No: ";
            cin >> rno;
            cout << "Enter Customer Name: ";
            cin.ignore();
            getline(cin, cname);
            cout << "Enter No. of Days: ";
            cin >> days;

            for (int i = 0; i < 3; i++) {
                if (rooms[i].getRoomNo() == rno) {
                    Booking b(rooms[i].getRoomNo(),
                              rooms[i].getType(),
                              rooms[i].getPrice(),
                              cname, days);

                    cout << "\nBooking Confirmed ✅\n";
                    cout << setw(5) << "Room" << setw(15) << "Type"
                         << setw(15) << "Customer" << setw(8) << "Days"
                         << setw(10) << "Total" << endl;
                    b.displayBooking();

                    b.saveBookingToFile();
                    cout << "\nBooking saved to 'bookings.txt'\n";
                    found = true;
                }
            }

            if (!found) {
                cout << "Invalid Room Number!\n";
            }
        }

        else if (choice == 3) {
            ifstream file("bookings.txt");
            string line;
            cout << "\n===== All Bookings =====\n";
            if (file.is_open()) {
                while (getline(file, line)) {
                    cout << line << endl;
                }
                file.close();
            } else {
                cout << "No bookings found.\n";
            }
        }

        else if (choice == 4) {
            int rno;
            string cname;
            cout << "Enter Room No to cancel: ";
            cin >> rno;
            cout << "Enter Customer Name: ";
            cin.ignore();
            getline(cin, cname);

            cancelBooking(rno, cname);
        }

        else if (choice == 5) {
            searchBooking();
        }

        else if (choice == 6) {
            cout << "Exiting... Thank you!\n";
        }

        else {
            cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 6);

    return 0;
}
