#include "account.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;

// ============================================
// ACCOUNT CLASS METHODS
// ============================================

void Account::display() {
    cout << "Account: " << account_number << "\n";
    cout << "Name: " << full_name << "\n";
    cout << "Balance: " << balance << "\n";
}

double savings_acc::get_min_deposit() { return 100.0; }
double cheque_acc::get_min_deposit() { return 500.0; }
double fixed_deposit_acc::get_min_deposit() { return 1000.0; }
double student_acc::get_min_deposit() { return 50.0; }

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
    cout << "3. Fixed Deposit Account (Min: R1000)\n";
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
        if(acc->balance < min_deposit) {
            cout << "Deposit must be at least R" << min_deposit << "!\n";
        }
    } while(acc->balance < min_deposit);
    
    string pin = gen_pin();
    string pin_salt = "P1N";
    hash<string> hasher;
    string pin_hash = to_string(hasher(pin + pin_salt));
    
    cout << "\n========================================\n";
    cout << "*** IMPORTANT - CUSTOMER PIN ***\n";
    cout << "Generated PIN: " << pin << "\n";
    cout << "Please provide this PIN to the customer!\n";
    cout << "========================================\n";
    
    ofstream file("customers.dat", ios::app);
    if(file.is_open()) {
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
        cout << "\nCustomer registered successfully!\n";
        cout << "Account Number: " << acc->account_number << "\n";
    } else {
        cout << "Error saving customer data!\n";
    }
    
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
        if(!file.is_open()) {
            cout << "No customer data found!\n";
            return false;
        }
        
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
                } else {
                    cout << "Incorrect PIN!\n";
                    file.close();
                    break;
                }
            }
        }
        file.close();
        attempts++;
        cout << "Attempts remaining: " << (3 - attempts) << "\n";
    }
    
    cout << "Too many failed attempts. Access denied.\n";
    return false;
}

// ============================================
// VIEW BALANCE
// ============================================

void view_balance(string acc_no, string pin) {
    ifstream file("customers.dat");
    if(!file.is_open()) {
        cout << "Unable to open customer data!\n";
        return;
    }
    
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
            if(attempt == pin_hash) {
                cout << "\n=== ACCOUNT BALANCE ===\n";
                cout << "Account: " << file_acc << "\n";
                cout << "Customer: " << name << "\n";
                cout << "Current Balance: R" << balance << "\n";
            } else {
                cout << "Invalid PIN!\n";
            }
            file.close();
            return;
        }
    }
    
    cout << "Account not found!\n";
    file.close();
}

// ============================================
// VIEW STATEMENT
// ============================================

void view_statement(string acc_no) {
    ifstream file("customers.dat");
    if(!file.is_open()) {
        cout << "Unable to open customer data!\n";
        return;
    }
    
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(file_acc == acc_no) {
            string type_name;
            if(type == "1") type_name = "Savings";
            else if(type == "2") type_name = "Cheque";
            else if(type == "3") type_name = "Fixed Deposit";
            else if(type == "4") type_name = "Student";
            
            cout << "\n========================================\n";
            cout << "           ACCOUNT STATEMENT            \n";
            cout << "========================================\n";
            cout << "Account Number: " << file_acc << "\n";
            cout << "Customer Name:  " << name << "\n";
            cout << "SA ID Number:   " << sa_id << "\n";
            cout << "Contact:        " << contact << "\n";
            cout << "Email:          " << email << "\n";
            cout << "Address:        " << address << "\n";
            cout << "Date of Birth:  " << dob << "\n";
            cout << "Account Type:   " << type_name << "\n";
            cout << "Branch Code:    " << branch << "\n";
            cout << "----------------------------------------\n";
            cout << "Current Balance: R" << balance << "\n";
            cout << "========================================\n";
            
            ifstream trans("transactions.dat");
            if(trans.is_open()) {
                string line;
                cout << "\n--- TRANSACTION HISTORY ---\n";
                bool has_trans = false;
                while(getline(trans, line)) {
                    if(line.find(acc_no) != string::npos) {
                        cout << line << "\n";
                        has_trans = true;
                    }
                }
                if(!has_trans) {
                    cout << "No transactions found.\n";
                }
                trans.close();
            }
            
            file.close();
            return;
        }
    }
    
    cout << "Account not found!\n";
    file.close();
}

// ============================================
// SEARCH FUNCTIONS
// ============================================

void search_by_account() {
    string acc_no;
    cout << "\n--- Search by Account Number ---\n";
    cout << "Enter Account Number: ";
    cin >> acc_no;
    
    ifstream file("customers.dat");
    if(!file.is_open()) {
        cout << "Unable to open customers.dat file!\n";
        return;
    }
    
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
            
            string type_name;
            if(type == "1") type_name = "Savings";
            else if(type == "2") type_name = "Cheque";
            else if(type == "3") type_name = "Fixed Deposit";
            else if(type == "4") type_name = "Student";
            
            cout << "\n=== CUSTOMER FOUND ===\n";
            cout << "Account Number: " << file_acc << "\n";
            cout << "Full Name: " << name << "\n";
            cout << "SA ID: " << sa_id << "\n";
            cout << "Contact: " << contact << "\n";
            cout << "Email: " << email << "\n";
            cout << "Address: " << address << "\n";
            cout << "Date of Birth: " << dob << "\n";
            cout << "Account Type: " << type_name << "\n";
            cout << "Current Balance: R" << balance << "\n";
            cout << "Branch Code: " << branch << "\n";
            break;
        }
    }
    
    if(!found) {
        cout << "Account number " << acc_no << " not found!\n";
    }
    
    file.close();
}

void search_by_name() {
    string search_name;
    cout << "\n--- Search by Customer Name ---\n";
    cout << "Enter name to search (or partial name): ";
    cin.ignore();
    getline(cin, search_name);
    
    ifstream file("customers.dat");
    if(!file.is_open()) {
        cout << "Unable to open customers.dat file!\n";
        return;
    }
    
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    bool found = false;
    int match_count = 0;
    
    cout << "\n=== SEARCH RESULTS ===\n";
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(name.find(search_name) != string::npos) {
            found = true;
            match_count++;
            
            cout << "\n--- Match #" << match_count << " ---\n";
            cout << "Account Number: " << file_acc << "\n";
            cout << "Name: " << name << "\n";
            cout << "Contact: " << contact << "\n";
            cout << "Balance: R" << balance << "\n";
            cout << "Branch: " << branch << "\n";
        }
    }
    
    if(!found) {
        cout << "No customers found with name containing '" << search_name << "'\n";
    } else {
        cout << "\nTotal matches found: " << match_count << "\n";
    }
    
    file.close();
}

void filter_by_branch() {
    string code;
    cout << "\n--- Filter Customers by Branch ---\n";
    cout << "Enter Branch Code (B001, B002, B003): ";
    cin >> code;
    
    ifstream file("customers.dat");
    if(!file.is_open()) {
        cout << "Unable to open customers.dat file!\n";
        return;
    }
    
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    bool found = false;
    int customer_count = 0;
    double total_balance = 0;
    
    cout << "\n=== CUSTOMERS IN BRANCH " << code << " ===\n";
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(branch == code) {
            found = true;
            customer_count++;
            total_balance += stod(balance);
            
            cout << "\n--- Customer #" << customer_count << " ---\n";
            cout << "Account: " << file_acc << "\n";
            cout << "Name: " << name << "\n";
            cout << "Contact: " << contact << "\n";
            cout << "Balance: R" << balance << "\n";
        }
    }
    
    if(!found) {
        cout << "No customers found in branch " << code << "\n";
    } else {
        cout << "\n=== BRANCH SUMMARY ===\n";
        cout << "Total Customers: " << customer_count << "\n";
        cout << "Total Balance: R" << total_balance << "\n";
        cout << "Average Balance: R" << (customer_count > 0 ? total_balance / customer_count : 0) << "\n";
    }
    
    file.close();
}