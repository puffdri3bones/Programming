#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using namespace std;

// Account Generation
string gen_acc_numb(string branch_code);
string gen_pin();

// Validation Functions
bool validate_said(string id);
bool validate_phone(string phone);
bool validate_email(string email);
bool validate_pin_format(string pin);

// PIN Management
bool verify_pin(string acc_no, string pin);
void change_pin(string acc_no, string old_pin, string new_pin);

// Interest Calculation
double calc_interest(string type, double balance);

#endif