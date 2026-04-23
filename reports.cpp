#include "reports.h"
#include <fstream>
#include <iostream>

using namespace std;

void daily_transaction_report() {
    ifstream file("transactions.dat");
    string line;
    
    cout << "\n=== DAILY TRANSACTION REPORT ===\n";
    bool has_data = false;
    
    while(getline(file, line)) {
        cout << line << "\n";
        has_data = true;
    }
    
    if(!has_data) {
        cout << "No transactions found\n";
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
    double total_balance = 0;
    
    while(getline(file, file_acc) && getline(file, name) &&
          getline(file, sa_id) && getline(file, contact) &&
          getline(file, email) && getline(file, address) &&
          getline(file, dob) && getline(file, type) &&
          getline(file, balance) && getline(file, branch) &&
          getline(file, pin_salt) && getline(file, pin_hash)) {
        
        if(branch == branch_code) {
            count++;
            total_balance += stod(balance);
        }
    }
    
    cout << "\n=== BRANCH REPORT ===\n";
    cout << "Branch: " << branch_code << "\n";
    cout << "Total Customers: " << count << "\n";
    cout << "Total Deposits: R" << total_balance << "\n";
    file.close();
}

void reports_menu() {
    int choice;
    do {
        cout << "\n=== REPORTS MENU ===\n";
        cout << "1. Daily Transaction Report\n";
        cout << "2. Customer Summary\n";
        cout << "3. Branch Report\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        if(choice == 1) {
            daily_transaction_report();
        } else if(choice == 2) {
            customer_summary();
        } else if(choice == 3) {
            string code;
            cout << "Enter Branch Code: ";
            cin >> code;
            branch_report(code);
        }
    } while(choice != 4);
}