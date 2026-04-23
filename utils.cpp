#include "utils.h"
#include <fstream>
#include <random>
#include <functional>
#include <algorithm>
#include <iostream>

using namespace std;

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

bool validate_pin_format(string pin) {
    return pin.length() == 5 && all_of(pin.begin(), pin.end(), ::isdigit);
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
    
    if(!validate_pin_format(new_pin)) {
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