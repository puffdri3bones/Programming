#include "teller.h"
#include <fstream>
#include <iostream>
#include <functional>

using namespace std;

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
    }
    out_file.close();
    
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
            return true;
        }
    }
    
    cout << "Invalid ID or Password!\n";
    return false;
}