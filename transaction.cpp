#include "transaction.h"
#include "utils.h"
#include <fstream>
#include <iostream>

using namespace std;

void log_transactions(string acc_no, string type, double amount) {
    ofstream file("transactions.dat", ios::app);
    file << acc_no << " | " << type << " | " << amount << "\n";
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
            cout << "Deposit Successful!\n";
            cout << "New Balance: R" << balance << "\n";
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
        cout << "Account not found\n";
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
                cout << "Withdrawal Successful!\n";
                cout << "New Balance: R" << balance << "\n";
                log_transactions(acc_no, "Withdraw", amount);
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
        cout << "Account not found\n";
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
    
    // Transfer implementation (similar to your existing code)
    cout << "Transfer feature - add your full implementation here\n";
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
    cout << "Interest applied successfully!\n";
}

void export_customers_to_csv() {
    ofstream csv("customers_export.csv");
    csv << "Account Number,Full Name,SA ID,Contact,Email,Balance\n";
    
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
            << contact << "," << email << "," << balance << "\n";
    }
    
    file.close();
    csv.close();
    cout << "Exported customers to customers_export.csv\n";
}

void export_transactions_to_csv() {
    ifstream src("transactions.dat");
    ofstream dst("transactions_export.csv");
    
    dst << "Transaction Log\n";
    string line;
    while(getline(src, line)) {
        dst << line << "\n";
    }
    
    src.close();
    dst.close();
    cout << "Exported transactions to transactions_export.csv\n";
}

void export_branch_report_to_csv() {
    ofstream csv("branch_report_export.csv");
    csv << "Branch Code,Number of Customers\n";
    
    ifstream file("customers.dat");
    string file_acc, name, sa_id, contact, email, address, dob,
           type, balance, branch, pin_salt, pin_hash;
    
    int b1=0, b2=0, b3=0;
    
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
    
    csv << "B001," << b1 << "\n";
    csv << "B002," << b2 << "\n";
    csv << "B003," << b3 << "\n";
    
    file.close();
    csv.close();
    cout << "Exported branch report to branch_report_export.csv\n";
}

void export_all_to_csv() {
    export_customers_to_csv();
    export_transactions_to_csv();
    export_branch_report_to_csv();
    cout << "\nAll exports completed!\n";
}