#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>

using namespace std;

// Base Account Class (Abstract)
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
    virtual void display();
    virtual ~Account() {}  // VIRTUAL DESTRUCTOR - FIXES VTABLE ERROR
};

// Derived Account Types
class savings_acc : public Account {
public:
    double get_min_deposit() override;
};

class cheque_acc : public Account {
public:
    double get_min_deposit() override;
};

class fixed_deposit_acc : public Account {
public:
    double get_min_deposit() override;
};

class student_acc : public Account {
public:
    double get_min_deposit() override;
};

// Customer Functions
void register_cust(string branch_code);
bool login_customer(string &acc_no);
void view_balance(string acc_no, string pin);
void view_statement(string acc_no);

// Search Functions
void search_by_account();
void search_by_name();
void filter_by_branch();

#endif