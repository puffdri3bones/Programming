#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

using namespace std;

void log_transactions(string acc_no, string type, double amount);

void deposit(string acc_no);

void withdraw(string acc_no);

void transfer(string sender_acc);

void apply_interest();

void export_transactions_to_csv();

void export_branch_to_csv();

void export_all_to_csv();

#endif