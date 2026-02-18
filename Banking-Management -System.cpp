//adding header files
#include <iostream>
#include <fstream>
#include <string>
#include<iomanip>
#include <cctype>
#include <sstream>
using namespace std;

//structure for account
struct Account {
    int acc_no;
    string name;
    string type;
    long long balance;
};


// Strict integer validation 
int getValidInt(const string& msg) {
    string input;
    int number;

    while (true) {
        cout << msg;
        cin >> input;

        bool valid = true;
        for (char c : input) {
            if (!isdigit(c)) {
                valid = false;
                break;
            }
        }

        if (valid) {
            stringstream ss(input);
            ss >> number;
            return number;
        }

        cout << "Invalid input! Numbers only.\n";
    }
}

// Name validation
string getValidName(const string& msg) {
    string name;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer ONCE

    while (true) {
        cout << msg;
        getline(cin, name);

        bool valid = true;
        for (char c : name) {
            if (!isalpha(c) && c != ' ') {
                valid = false;
                break;
            }
        }

        if (valid && !name.empty())
            return name;

        cout << "Invalid name! Alphabets only.\n";
    }
}

// Only S or C
char getValidAccountType() {
    char type;
    while (true) {
        cout << "Enter Account Type (S = Saving, C = Current): ";
        cin >> type;
        type = toupper(type);

        if (type == 'S' || type == 'C')
            return type;

        cout << "Invalid choice! Enter S or C only.\n";
    }
}

// Balance limits
long long getValidBalance(char type) {
    string input;
    long long balance;

    while (true) {
        cout << "Enter Initial Balance: ";
        cin >> input;

        bool valid = true;
        for (char c : input) {
            if (!isdigit(c)) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            cout << "Numbers only!\n";
            continue;
        }

        stringstream ss(input);
        ss >> balance;

        if (type == 'S' && balance >= 500)
            return balance;
        if (type == 'C' && balance >= 1000)
            return balance;

        cout << "Minimum balance not met!\n";
    }
}



// 1. Add account
void addAccount() {
    Account acc;
    ifstream infile("account.txt");
    int found = 0;

    int newacc_no = getValidInt("Enter Account Number: ");

    while (infile >> acc.acc_no >> quoted(acc.name) >> acc.type >> acc.balance) {
        if (newacc_no == acc.acc_no) {
            found = 1;
            break;
        }
    }
    infile.close();

    if (found) {
        cout << "Account already exists!\n";
        return;
    }

    ofstream file("account.txt", ios::app);

    acc.name = getValidName("Enter Account Holder Name: ");
    char accType = getValidAccountType();
    acc.balance = getValidBalance(accType);

    acc.type = (accType == 'S') ? "Saving" : "Current";

    file << newacc_no << " "
        << quoted(acc.name) << " "
        << acc.type << " "
        << acc.balance << endl;

    file.close();
    cout << "Account added successfully!\n";
}



// 2. Edit account
void editAccount() {
    Account acc;
    int edit, found = 0; //initialize found with 0 which mean not found
    ifstream file("account.txt"); //for reading the file
    ofstream temp("temp.txt"); //making temporary file
    cout << "Enter the account number which you want to edit : ";
    cin >> edit;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid choice! Enter numbers only.\n";
        return;
    }
    while (file >> acc.acc_no >> quoted(acc.name) >> acc.type >> acc.balance) { //reading the file until end of the file
        if (acc.acc_no == edit) { //if they are same it allow to edit the account
            found = 1; //1 means found
            cout << "Enter your name : ";
            cin.ignore();
            getline(cin, acc.name);
            cout << "Enter your account type (Saving/Current) : ";
            cin >> acc.type;
            cout << "Enter balance : ";
            cin >> acc.balance;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid choice! Enter numbers only.\n";
                return;
            }
        }
        temp << acc.acc_no << " " << quoted(acc.name) << " " << acc.type << " " << acc.balance << endl; //now all data in file account is stored in temp file
    }
    file.close(); //file closed
    temp.close(); //file closed
    remove("account.txt"); //deleting the account file
    rename("temp.txt", "account.txt");//renaming the temp file to account file

    if (found) //if found
        cout << "Account successfully edited " << endl;
    else //if not found
        cout << "Invalid account number" << endl;
}
// 3. View accounts
void viewAccounts() {
    Account acc;
    ifstream file("account.txt"); //reading file
    if (!file) //if file not found
    {
        cout << "Error file not found";
        return;
    }
    else { //if file found
        cout << "\tAccount lists" << endl;
    }
    while (file >> acc.acc_no >> quoted(acc.name) >> acc.type >> acc.balance) { //read the all data in file
        cout << "Account number : " << acc.acc_no << endl;
        cout << "Account name : " << quoted(acc.name) << endl;
        cout << "Account type : " << acc.type << endl;
        cout << "Account balance : " << acc.balance << endl;
        cout << "------------------------------------------" << endl;
    }
    file.close(); //file closed
}
// 4. Search account
void searchAccount() {
    Account acc;
    int id, found = 0;
    ifstream file("account.txt");
    cout << "Enter the account number : ";
    cin >> id;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid choice! Enter numbers only.\n";
        return;
    }
    while (file >> acc.acc_no >> quoted(acc.name) >> acc.type >> acc.balance) {
        if (acc.acc_no == id) {
            cout << "\tAccount found" << endl;
            cout << "Account number : " << acc.acc_no << endl;
            cout << "Account name : " << quoted(acc.name) << endl;
            cout << "Account type : " << acc.type << endl;
            cout << "Account balance : " << acc.balance << endl;
            found = 1;
        }
    }
    file.close();
    if (!found) {
        cout << "Invalid account number ";
    }
}
// 5. Delete account
void deleteAccount() {
    Account acc;
    int id, found = 0;
    ifstream file("account.txt");
    ofstream temp("temp.txt");
    cout << "Enter the account number for delete: ";
    cin >> id;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid choice! Enter numbers only.\n";
        return;
    }
    while (file >> acc.acc_no >> quoted(acc.name) >> acc.type >> acc.balance) {
        if (acc.acc_no != id) { //in this part of program all data is saved in temporary file except the account number which we enter to delete
            found = 1;
            temp << acc.acc_no << " " << quoted(acc.name) << " " << acc.type << " " << acc.balance << endl;
        }
    }
    file.close();
    temp.close();
    remove("account.txt"); //then remove this account file
    rename("temp.txt", "account.txt"); //and rename the temporary file by account file
    if (found) {
        cout << "Account successfully deleted ";
    }
    else {
        cout << "Invalid account" << endl;
    }
}
void withdrawmoney() {
    Account acc;
    int id, found = 0;
    double amount;
    ifstream file("account.txt");
    ofstream temp("temp.txt");
    cout << "Enter account number: ";
    cin >> id;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid account number!\n";
        return;
    }
    cout << "Enter withdraw amount: ";
    cin >> amount;
    if (cin.fail() || amount <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid amount!\n";
        return;
    }
    while (file >> acc.acc_no >> quoted(acc.name) >> acc.type >> acc.balance) {
        if (acc.acc_no == id) {
            found = 1;
            if (acc.balance >= amount) {
                acc.balance -= amount;
                cout << "Withdrawal successful!\n";
                cout << "Remaining balance: " << acc.balance << endl;
            }
            else 
            {
                cout << "Insufficient balance!\n";
            }
           
        }

        temp << acc.acc_no << " " << quoted(acc.name) << " "<< acc.type << " "<< acc.balance << endl;
    }
    file.close();
    temp.close();

    remove("account.txt");
    rename("temp.txt", "account.txt");

    if (!found) {
        cout << "Account not found!\n";
    }
}
//main function
int main() {
    int choice;
    do {
        cout << "\n--------------------------------------------------------";
        cout << "\n\tWelcome to banking management system" << endl;
        cout << "1.Add account " << endl;
        cout << "2.Edit account " << endl;
        cout << "3.View accounts " << endl;
        cout << "4.Search account " << endl;
        cout << "5.Delete account " << endl;
        cout << "6.Withdraw Money" << endl;
        cout << "7.Exit " << endl;
        cout << "Enter your choice : ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid choice! Enter numbers only.\n";
            continue; // go back to menu safely
        }
        //using pointer to call functions
        void (*funcPtr)() = nullptr;
        switch (choice) {
        case 1: funcPtr = addAccount; break;
        case 2: funcPtr = editAccount; break;
        case 3: funcPtr = viewAccounts; break;
        case 4: funcPtr = searchAccount; break;
        case 5: funcPtr = deleteAccount; break;
        case 6: funcPtr = withdrawmoney; break;
        case 7:
            cout << "Thanks for visiting: ";
            funcPtr = nullptr;
            break;
        default:
            cout << "You entered an invalid choice!";
            funcPtr = nullptr;
        }
        if (funcPtr != nullptr) {
            funcPtr(); //call the function through pointer
        }
    } while (choice != 7);
    return 0;
}

