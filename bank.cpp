#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <random>
#include <cstdio>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// ============================================
// BRANCH STRUCTURE
// ============================================

struct Branch {
    string code;
    string name;
    string location;
};

vector<Branch> branches = {
    {"B001", "West Coast Branch", "Cape Town"},
    {"B002", "Inland Branch", "Johannesburg"},
    {"B003", "East Coast Branch", "Durban"}
};

// ============================================
// TELLER CLASS
// ============================================

class Teller {
public:
    string id;
    string full_name;
    string password;
    string branch_code;
};

// ============================================
// ACCOUNT CLASSES (INHERITANCE)
// ============================================

class Account {
public:
    string account_number;
    string full_name;
    string sa_id;
    string contact;
    string email;
    string address;
    string dob;
    string account_type;
    double balance;
    string branch_code;
    string pin;

    virtual double get_min_deposit() = 0;
    virtual ~Account() {}  // Virtual destructor
};

class savings_acc : public Account {
public:
    double get_min_deposit() override { return 100.0; }
};

class cheque_acc : public Account {
public:
    double get_min_deposit() override { return 500.0; }
};

class fixed_deposit_acc : public Account {
public:
    double get_min_deposit() override { return 1000.0; }
};

class student_acc : public Account {
public:
    double get_min_deposit() override { return 50.0; }
};

// ============================================
// UTILITY FUNCTIONS
// ============================================

string gen_acc_numb(string branch_code) {
    static int counter = 1000;
    return "ACC-" + branch_code + "-" + to_string(counter++);
}

string gen_pin() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(10000, 99999);
    return to_string(dist(gen));
}

bool validate_said(string id) {
    return id.length() == 13 && all_of(id.begin(), id.end(), ::isdigit);
}

bool validate_phone(string phone) {
    return phone.length() == 10 && all_of(phone.begin(), phone.end(), ::isdigit);
}

bool validate_email(string email) {
    return email.find("@") != string::npos && email.find(".") != string::npos;
}

bool verify_pin(string acc_no, string pin) {
    ifstream file("customers.dat");
    if(!file.is_open()) return false;
    
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    while(getline(file, file_acc) && getline(file, name) && 
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) && 
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(file_acc == acc_no) {
            hash<string> hasher;
            string attempt = to_string(hasher(pin + pin_salt));
            file.close();
            return attempt == pin_hash;
        }
    }
    file.close();
    return false;
}

void change_pin(string acc_no, string old_pin, string new_pin) {
    if(!verify_pin(acc_no, old_pin)) {
        cout << "Invalid current PIN!\n";
        return;
    }
    
    if(new_pin.length() != 5 || !all_of(new_pin.begin(), new_pin.end(), ::isdigit)) {
        cout << "PIN must be 5 digits!\n";
        return;
    }
    
    ifstream file("customers.dat");
    ofstream temp("temp.dat");
    
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    bool found = false;
    
    while(getline(file, file_acc) && getline(file, name) && 
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) && 
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(file_acc == acc_no) {
            found = true;
            hash<string> hasher;
            pin_hash = to_string(hasher(new_pin + pin_salt));
            cout << "PIN changed successfully!\n";
        }
        
        temp << file_acc << "\n" << name << "\n" << sa_id << "\n"
             << contact << "\n" << email << "\n" << address << "\n"
             << dob << "\n" << type << "\n" << balance << "\n"
             << branch << "\n" << pin_salt << "\n" << pin_hash << "\n";
    }
    
    file.close();
    temp.close();
    
    if(found) {
        remove("customers.dat");
        rename("temp.dat", "customers.dat");
    } else {
        remove("temp.dat");
        cout << "Account not found!\n";
    }
}

double calc_interest(string type, double balance) {
    double rate = 0;
    if(type == "1") rate = 0.05;
    else if(type == "2") rate = 0.02;
    else if(type == "3") rate = 0.08;
    else if(type == "4") rate = 0.03;
    return balance * rate;
}

// ============================================
// TELLER FUNCTIONS
// ============================================

int create_teller_account() {
    ofstream out_file("tellers.dat", ios::app);
    Teller my_obj;
    
    cout << "Enter Teller ID: ";
    cin >> my_obj.id;
    cout << "Enter Teller Full Name: ";
    cin.ignore();
    getline(cin, my_obj.full_name);
    cout << "Create Teller Password: ";
    cin >> my_obj.password;
    cout << "Enter Branch Code: ";
    cin >> my_obj.branch_code;
    
    string salt = "XyZ123";
    string salted = my_obj.password + salt;
    hash<string> pass_hash;
    size_t hashed_password = pass_hash(salted);
    
    if(out_file.is_open()) {
        out_file << my_obj.id << "\n"
                 << my_obj.full_name << "\n"
                 << salt << "\n"
                 << hashed_password << "\n"
                 << my_obj.branch_code << "\n";
        out_file.close();
    }
    
    cout << "Teller account created successfully!\n";
    return hashed_password;
}

bool login_teller(string &branch_code) {
    string identify, pass_word;
    cout << "Enter Teller ID: ";
    cin >> identify;
    cout << "Enter Password: ";
    cin >> pass_word;
    
    ifstream file("tellers.dat");
    string file_id, file_name, file_salt, file_password, file_branch;
    
    while(getline(file, file_id) && getline(file, file_name) &&
          getline(file, file_salt) && getline(file, file_password) &&
          getline(file, file_branch)) {
        
        hash<string> hash_pass;
        string attempt = to_string(hash_pass(pass_word + file_salt));
        
        if(file_id == identify && attempt == file_password) {
            branch_code = file_branch;
            cout << "Login Successful!\n";
            file.close();
            return true;
        }
    }
    
    cout << "Invalid ID or Password!\n";
    file.close();
    return false;
}

// ============================================
// CUSTOMER REGISTRATION
// ============================================

void register_cust(string branch_code) {
    Account* acc = nullptr;
    int type;
    
    cout << "\n=== REGISTER NEW CUSTOMER ===\n";
    cout << "Select Account Type:\n";
    cout << "1. Savings Account (Min: R100)\n";
    cout << "2. Cheque Account (Min: R500)\n";
    cout << "3. Fixed Deposit (Min: R1000)\n";
    cout << "4. Student Account (Min: R50)\n";
    cout << "Enter choice: ";
    cin >> type;
    
    if(type == 1) acc = new savings_acc();
    else if(type == 2) acc = new cheque_acc();
    else if(type == 3) acc = new fixed_deposit_acc();
    else if(type == 4) acc = new student_acc();
    else {
        cout << "Invalid account type!\n";
        return;
    }
    
    cin.ignore();
    cout << "Full Name: ";
    getline(cin, acc->full_name);
    
    do {
        cout << "SA ID (13 digits): ";
        cin >> acc->sa_id;
    } while(!validate_said(acc->sa_id));
    
    do {
        cout << "Phone Number (10 digits): ";
        cin >> acc->contact;
    } while(!validate_phone(acc->contact));
    
    do {
        cout << "Email: ";
        cin >> acc->email;
    } while(!validate_email(acc->email));
    
    cin.ignore();
    cout << "Physical Address: ";
    getline(cin, acc->address);
    cout << "Date of Birth (DD/MM/YYYY): ";
    cin >> acc->dob;
    
    acc->branch_code = branch_code;
    acc->account_number = gen_acc_numb(branch_code);
    
    double min_deposit = acc->get_min_deposit();
    do {
        cout << "Initial Deposit (Min: R" << min_deposit << "): R";
        cin >> acc->balance;
    } while(acc->balance < min_deposit);
    
    string pin = gen_pin();
    string pin_salt = "P1N";
    hash<string> hasher;
    string pin_hash = to_string(hasher(pin + pin_salt));
    
    cout << "\n========================================\n";
    cout << "*** CUSTOMER PIN: " << pin << " ***\n";
    cout << "========================================\n";
    
    ofstream file("customers.dat", ios::app);
    file << acc->account_number << "\n"
         << acc->full_name << "\n"
         << acc->sa_id << "\n"
         << acc->contact << "\n"
         << acc->email << "\n"
         << acc->address << "\n"
         << acc->dob << "\n"
         << type << "\n"
         << acc->balance << "\n"
         << acc->branch_code << "\n"
         << pin_salt << "\n"
         << pin_hash << "\n";
    file.close();
    
    cout << "Customer registered! Account: " << acc->account_number << "\n";
    delete acc;
}

// ============================================
// CUSTOMER LOGIN
// ============================================

bool login_customer(string &acc_no) {
    string pin;
    cout << "\n=== CUSTOMER LOGIN ===\n";
    cout << "Enter Account Number: ";
    cin >> acc_no;
    
    int attempts = 0;
    while(attempts < 3) {
        cout << "Enter PIN: ";
        cin >> pin;
        
        ifstream file("customers.dat");
        string file_acc, name, id, contact, email, address, dob,
               type, balance, branch, pin_salt, pin_hash;
        
        while(getline(file, file_acc) && getline(file, name) &&
              getline(file, id) && getline(file, contact) &&
              getline(file, email) && getline(file, address) &&
              getline(file, dob) && getline(file, type) &&
              getline(file, balance) && getline(file, branch) &&
              getline(file, pin_salt) && getline(file, pin_hash)) {
            
            if(file_acc == acc_no) {
                hash<string> hasher;
                string attempt = to_string(hasher(pin + pin_salt));
                if(attempt == pin_hash) {
                    cout << "Login Successful!\n";
                    file.close();
                    return true;
                }
            }
        }
        file.close();
        attempts++;
        cout << "Attempts left: " << (3 - attempts) << "\n";
    }
    
    cout << "Too many failed attempts. Account locked.\n";
    return false;
}

// ============================================
// TRANSACTION FUNCTIONS
// ============================================

void log_transactions(string acc_no, string type, double amount) {
    ofstream file("transactions.dat", ios::app);
    file << acc_no << " | " << type << " | R" << amount << "\n";
    file.close();
}

void deposit(string acc_no) {
    double amount;
    cout << "Enter Deposit Amount: R";
    cin >> amount;
    
    if(amount <= 0) {
        cout << "Invalid amount!\n";
        return;
    }
    
    ifstream file("customers.dat");
    ofstream temp("temp.dat");
    
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    bool found = false;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(file_acc == acc_no) {
            found = true;
            double current_balance = stod(balance);
            current_balance += amount;
            balance = to_string(current_balance);
            cout << "Deposit successful! New balance: R" << balance << "\n";
        }
        
        temp << file_acc << "\n" << name << "\n" << sa_id << "\n"
             << contact << "\n" << email << "\n" << address << "\n"
             << dob << "\n" << type << "\n" << balance << "\n"
             << branch << "\n" << pin_salt << "\n" << pin_hash << "\n";
    }
    
    file.close();
    temp.close();
    
    if(found) {
        remove("customers.dat");
        rename("temp.dat", "customers.dat");
        log_transactions(acc_no, "Deposit", amount);
    } else {
        remove("temp.dat");
        cout << "Account not found!\n";
    }
}

void withdraw(string acc_no) {
    double amount;
    cout << "Enter Withdrawal Amount: R";
    cin >> amount;
    
    if(amount <= 0) {
        cout << "Invalid amount!\n";
        return;
    }
    
    ifstream file("customers.dat");
    ofstream temp("temp.dat");
    
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    bool found = false;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(file_acc == acc_no) {
            found = true;
            double current_balance = stod(balance);
            if(amount <= current_balance) {
                current_balance -= amount;
                balance = to_string(current_balance);
                cout << "Withdrawal successful! New balance: R" << balance << "\n";
                log_transactions(acc_no, "Withdrawal", amount);
            } else {
                cout << "Insufficient funds!\n";
            }
        }
        
        temp << file_acc << "\n" << name << "\n" << sa_id << "\n"
             << contact << "\n" << email << "\n" << address << "\n"
             << dob << "\n" << type << "\n" << balance << "\n"
             << branch << "\n" << pin_salt << "\n" << pin_hash << "\n";
    }
    
    file.close();
    temp.close();
    
    if(found) {
        remove("customers.dat");
        rename("temp.dat", "customers.dat");
    } else {
        remove("temp.dat");
        cout << "Account not found!\n";
    }
}

void transfer(string sender_acc) {
    string receiver_acc;
    double amount;
    
    cout << "Enter receiver account number: ";
    cin >> receiver_acc;
    cout << "Enter Transfer Amount: R";
    cin >> amount;
    
    if(amount <= 0) {
        cout << "Invalid amount!\n";
        return;
    }
    
    ifstream file("customers.dat");
    ofstream temp("temp.dat");
    
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    bool sender_found = false, receiver_found = false;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        double current_balance = stod(balance);
        
        if(file_acc == sender_acc) {
            sender_found = true;
            if(amount > current_balance) {
                cout << "Insufficient funds!\n";
                file.close();
                temp.close();
                remove("temp.dat");
                return;
            }
            current_balance -= amount;
            balance = to_string(current_balance);
        }
        else if(file_acc == receiver_acc) {
            receiver_found = true;
            current_balance += amount;
            balance = to_string(current_balance);
        }
        
        temp << file_acc << "\n" << name << "\n" << sa_id << "\n"
             << contact << "\n" << email << "\n" << address << "\n"
             << dob << "\n" << type << "\n" << balance << "\n"
             << branch << "\n" << pin_salt << "\n" << pin_hash << "\n";
    }
    
    file.close();
    temp.close();
    
    if(!sender_found) {
        cout << "Sender account not found!\n";
        remove("temp.dat");
        return;
    }
    if(!receiver_found) {
        cout << "Receiver account not found!\n";
        remove("temp.dat");
        return;
    }
    
    remove("customers.dat");
    rename("temp.dat", "customers.dat");
    log_transactions(sender_acc, "Transfer Out", amount);
    log_transactions(receiver_acc, "Transfer In", amount);
    cout << "Transfer successful!\n";
}

void view_balance(string acc_no, string pin) {
    if(!verify_pin(acc_no, pin)) {
        cout << "Invalid PIN!\n";
        return;
    }
    
    ifstream file("customers.dat");
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(file_acc == acc_no) {
            cout << "\n=== BALANCE ===\n";
            cout << "Account: " << file_acc << "\n";
            cout << "Balance: R" << balance << "\n";
            file.close();
            return;
        }
    }
    file.close();
}

void view_statement(string acc_no) {
    ifstream file("customers.dat");
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(file_acc == acc_no) {
            cout << "\n=== STATEMENT ===\n";
            cout << "Account: " << file_acc << "\n";
            cout << "Name: " << name << "\n";
            cout << "Balance: R" << balance << "\n";
            cout << "Branch: " << branch << "\n";
            
            ifstream trans("transactions.dat");
            string line;
            cout << "\nTransactions:\n";
            while(getline(trans, line)) {
                if(line.find(acc_no) != string::npos) {
                    cout << "  " << line << "\n";
                }
            }
            trans.close();
            file.close();
            return;
        }
    }
    cout << "Account not found!\n";
    file.close();
}

// ============================================
// BRANCH FUNCTIONS
// ============================================

void view_all_branches() {
    cout << "\n=== BRANCHES ===\n";
    for(auto &b : branches) {
        cout << "Code: " << b.code << "\n";
        cout << "Name: " << b.name << "\n";
        cout << "Location: " << b.location << "\n\n";
    }
}

void view_branch_details() {
    string code;
    cout << "Enter Branch Code: ";
    cin >> code;
    
    for(auto &b : branches) {
        if(b.code == code) {
            cout << "\nBranch: " << b.name << "\n";
            cout << "Location: " << b.location << "\n";
            return;
        }
    }
    cout << "Branch not found!\n";
}

void compare_branches() {
    ifstream file("customers.dat");
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    int b1 = 0, b2 = 0, b3 = 0;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(branch == "B001") b1++;
        else if(branch == "B002") b2++;
        else if(branch == "B003") b3++;
    }
    file.close();
    
    cout << "\n=== BRANCH COMPARISON ===\n";
    cout << "B001 (Cape Town): " << b1 << " customers\n";
    cout << "B002 (Johannesburg): " << b2 << " customers\n";
    cout << "B003 (Durban): " << b3 << " customers\n";
}

void apply_interest() {
    ifstream file("customers.dat");
    ofstream temp("temp.dat");
    
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        double current_balance = stod(balance);
        double interest = calc_interest(type, current_balance);
        current_balance += interest;
        balance = to_string(current_balance);
        
        temp << file_acc << "\n" << name << "\n" << sa_id << "\n"
             << contact << "\n" << email << "\n" << address << "\n"
             << dob << "\n" << type << "\n" << balance << "\n"
             << branch << "\n" << pin_salt << "\n" << pin_hash << "\n";
    }
    
    file.close();
    temp.close();
    
    remove("customers.dat");
    rename("temp.dat", "customers.dat");
    cout << "Interest applied to all accounts!\n";
}

// ============================================
// SEARCH FUNCTIONS
// ============================================

void search_by_account() {
    string acc_no;
    cout << "Enter Account Number: ";
    cin >> acc_no;
    
    ifstream file("customers.dat");
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(file_acc == acc_no) {
            cout << "\n=== CUSTOMER FOUND ===\n";
            cout << "Account: " << file_acc << "\n";
            cout << "Name: " << name << "\n";
            cout << "Balance: R" << balance << "\n";
            cout << "Branch: " << branch << "\n";
            file.close();
            return;
        }
    }
    cout << "Account not found!\n";
    file.close();
}

void search_by_name() {
    string search_name;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, search_name);
    
    ifstream file("customers.dat");
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    bool found = false;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(name.find(search_name) != string::npos) {
            cout << "\nAccount: " << file_acc << "\n";
            cout << "Name: " << name << "\n";
            cout << "Balance: R" << balance << "\n";
            found = true;
        }
    }
    if(!found) cout << "No customers found!\n";
    file.close();
}

void filter_by_branch() {
    string code;
    cout << "Enter Branch Code: ";
    cin >> code;
    
    ifstream file("customers.dat");
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    bool found = false;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(branch == code) {
            cout << "\nAccount: " << file_acc << "\n";
            cout << "Name: " << name << "\n";
            cout << "Balance: R" << balance << "\n";
            found = true;
        }
    }
    if(!found) cout << "No customers in this branch!\n";
    file.close();
}

// ============================================
// REPORT FUNCTIONS
// ============================================

void daily_transaction_report() {
    ifstream file("transactions.dat");
    string line;
    cout << "\n=== DAILY TRANSACTION REPORT ===\n";
    while(getline(file, line)) {
        cout << line << "\n";
    }
    file.close();
}

void customer_summary() {
    ifstream file("customers.dat");
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    cout << "\n=== CUSTOMER SUMMARY ===\n";
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        cout << "\nAccount: " << file_acc << "\n";
        cout << "Name: " << name << "\n";
        cout << "Balance: R" << balance << "\n";
        cout << "Branch: " << branch << "\n";
    }
    file.close();
}

void branch_report(string branch_code) {
    ifstream file("customers.dat");
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    int count = 0;
    double total = 0;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(branch == branch_code) {
            count++;
            total += stod(balance);
        }
    }
    file.close();
    
    cout << "\n=== BRANCH REPORT ===\n";
    cout << "Branch: " << branch_code << "\n";
    cout << "Total Customers: " << count << "\n";
    cout << "Total Balance: R" << total << "\n";
}

void reports_menu() {
    int choice;
    do {
        cout << "\n=== REPORTS MENU ===\n";
        cout << "1. Daily Transaction Report\n";
        cout << "2. Customer Summary\n";
        cout << "3. Branch Report\n";
        cout << "4. Back\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        if(choice == 1) daily_transaction_report();
        else if(choice == 2) customer_summary();
        else if(choice == 3) {
            string code;
            cout << "Enter Branch Code: ";
            cin >> code;
            branch_report(code);
        }
    } while(choice != 4);
}

// ============================================
// EXPORT FUNCTIONS
// ============================================

void export_customers_to_csv() {
    ofstream csv("customers_export.csv");
    csv << "Account Number,Full Name,SA ID,Contact,Email,Balance,Branch\n";
    
    ifstream file("customers.dat");
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        csv << file_acc << "," << name << "," << sa_id << ","
            << contact << "," << email << "," << balance << "," << branch << "\n";
    }
    file.close();
    csv.close();
    cout << "Exported to customers_export.csv\n";
}

void export_transactions_to_csv() {
    ofstream csv("transactions_export.csv");
    csv << "Transaction Log\n";
    
    ifstream file("transactions.dat");
    string line;
    while(getline(file, line)) {
        csv << line << "\n";
    }
    file.close();
    csv.close();
    cout << "Exported to transactions_export.csv\n";
}

void export_all_to_csv() {
    export_customers_to_csv();
    export_transactions_to_csv();
    cout << "All exports completed!\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    bool run = true;
    
    cout << "\n========================================\n";
    cout << "   MULTI-BRANCH BANKING SYSTEM\n";
    cout << "========================================\n";
    
    while(run) {
        int choice;
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Create Teller Account\n";
        cout << "2. Login As Teller\n";
        cout << "3. Login As Customer\n";
        cout << "4. Export Data to CSV\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        if(choice == 1) {
            create_teller_account();
        }
        else if(choice == 2) {
            string branch_code;
            if(login_teller(branch_code)) {
                bool teller_session = true;
                while(teller_session) {
                    int choices;
                    cout << "\n--- TELLER MENU ---\n";
                    cout << "1. Register Customer\n";
                    cout << "2. Deposit\n";
                    cout << "3. Withdraw\n";
                    cout << "4. Transfer\n";
                    cout << "5. View Branches\n";
                    cout << "6. Branch Details\n";
                    cout << "7. Compare Branches\n";
                    cout << "8. Apply Interest\n";
                    cout << "9. Search Customer\n";
                    cout << "10. Reports\n";
                    cout << "11. Export Data\n";
                    cout << "12. Logout\n";
                    cout << "Enter choice: ";
                    cin >> choices;
                    
                    if(choices == 1) register_cust(branch_code);
                    else if(choices == 2) {
                        string acc, pin;
                        cout << "Account: "; cin >> acc;
                        cout << "PIN: "; cin >> pin;
                        if(verify_pin(acc, pin)) deposit(acc);
                    }
                    else if(choices == 3) {
                        string acc, pin;
                        cout << "Account: "; cin >> acc;
                        cout << "PIN: "; cin >> pin;
                        if(verify_pin(acc, pin)) withdraw(acc);
                    }
                    else if(choices == 4) {
                        string acc, pin;
                        cout << "Sender Account: "; cin >> acc;
                        cout << "PIN: "; cin >> pin;
                        if(verify_pin(acc, pin)) transfer(acc);
                    }
                    else if(choices == 5) view_all_branches();
                    else if(choices == 6) view_branch_details();
                    else if(choices == 7) compare_branches();
                    else if(choices == 8) apply_interest();
                    else if(choices == 9) {
                        int s_choice;
                        cout << "1. By Account\n2. By Name\n3. By Branch\n";
                        cin >> s_choice;
                        if(s_choice == 1) search_by_account();
                        else if(s_choice == 2) search_by_name();
                        else if(s_choice == 3) filter_by_branch();
                    }
                    else if(choices == 10) reports_menu();
                    else if(choices == 11) export_all_to_csv();
                    else if(choices == 12) teller_session = false;
                }
            }
        }
        else if(choice == 3) {
            string acc_no;
            if(login_customer(acc_no)) {
                bool session = true;
                while(session) {
                    int opt;
                    cout << "\n--- CUSTOMER MENU ---\n";
                    cout << "1. View Balance\n";
                    cout << "2. Deposit\n";
                    cout << "3. Withdraw\n";
                    cout << "4. Transfer\n";
                    cout << "5. View Statement\n";
                    cout << "6. Change PIN\n";
                    cout << "7. Logout\n";
                    cout << "Enter choice: ";
                    cin >> opt;
                    
                    if(opt == 1) {
                        string pin;
                        cout << "Enter PIN: "; cin >> pin;
                        view_balance(acc_no, pin);
                    }
                    else if(opt == 2) {
                        string pin;
                        cout << "Enter PIN: "; cin >> pin;
                        if(verify_pin(acc_no, pin)) deposit(acc_no);
                    }
                    else if(opt == 3) {
                        string pin;
                        cout << "Enter PIN: "; cin >> pin;
                        if(verify_pin(acc_no, pin)) withdraw(acc_no);
                    }
                    else if(opt == 4) {
                        string pin;
                        cout << "Enter PIN: "; cin >> pin;
                        if(verify_pin(acc_no, pin)) transfer(acc_no);
                    }
                    else if(opt == 5) view_statement(acc_no);
                    else if(opt == 6) {
                        string old_pin, new_pin;
                        cout << "Current PIN: "; cin >> old_pin;
                        cout << "New PIN (5 digits): "; cin >> new_pin;
                        change_pin(acc_no, old_pin, new_pin);
                    }
                    else if(opt == 7) session = false;
                }
            }
        }
        else if(choice == 4) {
            export_all_to_csv();
        }
        else if(choice == 5) {
            run = false;
            cout << "Goodbye!\n";
        }
    }
    return 0;
}