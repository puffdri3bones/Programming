#include "branch.h"
#include <fstream>
#include <iostream>

using namespace std;

void init_default_branches(vector<Branch> &branches) {
    branches.clear();
    branches.push_back({"B001", "West Coast Branch", "Cape Town"});
    branches.push_back({"B002", "Inland Branch", "Johannesburg"});
    branches.push_back({"B003", "East Coast Branch", "Durban"});
}

void load_branches(vector<Branch> &branches) {
    ifstream file("branches.dat");
    if(!file.is_open()) {
        cout << "branches.dat not found, using defaults\n";
        return;
    }
    
    Branch b;
    branches.clear();
    while(getline(file, b.code) && getline(file, b.name) && getline(file, b.location)) {
        branches.push_back(b);
    }
    file.close();
}

void save_branches(vector<Branch> &branches) {
    ofstream file("branches.dat");
    for(auto &b : branches) {
        file << b.code << "\n" << b.name << "\n" << b.location << "\n";
    }
    file.close();
}

void view_all_branches(vector<Branch> &branches) {
    cout << "\n--- Branches ---\n";
    for(auto &b : branches) {
        cout << "Code: " << b.code << "\n";
        cout << "Name: " << b.name << "\n";
        cout << "Location: " << b.location << "\n\n";
    }
}

void view_branch_details(vector<Branch> &branches) {
    string code;
    cout << "Enter Branch Code: ";
    cin >> code;
    
    for(auto &b : branches) {
        if(b.code == code) {
            cout << "\nBranch Found:\n";
            cout << "Name: " << b.name << "\n";
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
    
    cout << "\n--- Branch Comparison ---\n";
    cout << "B001 Customers: " << b1 << "\n";
    cout << "B002 Customers: " << b2 << "\n";
    cout << "B003 Customers: " << b3 << "\n";
}