#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <random>
#include <cstdio>
#include <vector>


using namespace std;

struct Branch{

    string code;
    string name;
    string location;

};

vector<Branch> branches = {

        {"B001", "West Coast Branch", "Cape Town"},
        {"B002", "Inland Branch", "Johannessburg"},
        {"B003", "East Coast Branch", "Durban"}

    };

//Teller ID, Full Name, Password, Branch Code



class Teller{
    public:
        string id;
        string full_name;
        string password;
        string branch_code;
     
};

class Account{
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

        virtual void display(){
            cout << "Account: " << account_number << "\n";
            cout << "Name: " << full_name << "\n";
            cout << "Balance: " << balance << "\n";
        }
};

class savings_acc : public Account{
    public:
        double get_min_deposit() override{
            return 100.0;
        }

};

class cheque_acc : public Account{
    public:
        double get_min_deposit() override{
            return 500.0;
        }

};

class fixed_deposit_acc : public Account{
    public:
        double get_min_deposit() override{
            return 1000.0;
        }

};

class student_acc : public Account{
    public:
        double get_min_deposit() override{
            return 50.0;
        }

};

int create_teller_account(){

    ofstream out_file("tellers.dat", ios::app);

    Teller my_obj;

    cout << "Enter Teller ID: " << "\n";

    cin >> my_obj.id;

    cout << "Enter Teller Full Name: " << "\n";

    cin.ignore();

    getline(cin, my_obj.full_name);

    cout << "Create Teller Password: " << "\n";

    cin >> my_obj.password;

    cout << "Enter Branch Code: " << "\n";

    cin >> my_obj.branch_code;

    string salt = "XyZ123";

    string salted = my_obj.password + salt;

    hash<string> pass_hash;    

    size_t hashed_password = pass_hash(salted);

    cout << hashed_password;

        if (out_file.is_open()){

            out_file << my_obj.id << "\n";

            out_file << my_obj.full_name << "\n";

            out_file << salt << "\n";

            out_file << hashed_password << "\n";

            out_file << my_obj.branch_code << "\n";


        }

    return hashed_password;
}

string gen_acc_numb(string branch_code){
    static int counter = 1000;

    return "ACC-" + branch_code + "-" + to_string(counter++);
}

string gen_pin(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(10000, 99999);

    return to_string(dist(gen));
}

bool validate_said(string id){
    return id.length() == 13 && all_of(id.begin(), id.end(), ::isdigit);
}

bool validate_phone(string phone){
    return phone.length() == 10 && all_of(phone.begin(), phone.end(), ::isdigit);
}

bool validate_email(string email){
    return email.find("@") != string::npos &&
           email.find(".") != string::npos;
        }



void register_cust(string branch_code){
    Account * acc = nullptr;

    int type;
    cout << "Select Account Type: " << "\n";
    cout << "1. Savings" << "\n";
    cout << "2. Cheque" << "\n";
    cout << "3. Fixed Deposit" << "\n";
    cout << "4. Student" << "\n";

    cin >> type;

    if(type == 1) acc = new savings_acc();
    else if(type == 2) acc = new cheque_acc();
    else if(type == 3) acc = new fixed_deposit_acc();
    else if(type == 4) acc = new student_acc();

    cin.ignore();

    cout << "Full Name: " << "\n";
    getline(cin, acc -> full_name);

    do{
        cout << "SA ID (13 digits long): " << "\n";
        cin >> acc -> sa_id;
    }while(!validate_said(acc -> sa_id));

    do{
        cout << "Phone Number (10 digits): " << "\n";
        cin >> acc -> contact;
    }while(!validate_phone(acc -> contact));

    do{
        cout << "Email: : " << "\n";
        cin >> acc -> email;
    }while(!validate_email(acc -> email));

    cin.ignore();
    cout << "Address: " << "\n";
    getline(cin, acc -> address);

    cout << "Date of Birth: " << "\n";
    cin >> acc -> dob;

    acc -> branch_code = branch_code;
    acc -> account_number = gen_acc_numb(branch_code);

    double min_deposit = acc -> get_min_deposit();

    do{cout << "Initial Deposit: (Min: " << min_deposit << "): " << "\n";
       cin >> acc -> balance;
    }while(acc -> balance < min_deposit);

    string pin = gen_pin();

    string pin_salt = "P1N";

    hash<string> hasher;
    string pin_hash = to_string(hasher(pin + pin_salt));

    cout << "Generated Pin: " << pin << "\n";

    ofstream file("customers.dat", ios::app);

    file << acc -> account_number << "\n"
         << acc -> full_name << "\n"
         << acc -> sa_id << "\n"
         << acc -> contact << "\n"
         << acc -> email << "\n"
         << acc -> address<< "\n"
         << acc -> dob << "\n"
         << type << "\n"
         << acc -> balance << "\n"
         << acc -> branch_code << "\n"
         << pin_salt << "\n"
         << pin_hash << "\n";

    delete acc;


}

bool login_customer(string & acc_no){

    string pin;

    cout << "Enter Account Number: " << "\n";
    cin >> acc_no;

    int attempts = 0;

    while (attempts < 3){

        cout << "Enter PIN: " << "\n";
        cin >> pin;

        ifstream file("customers.dat");

        string file_acc, name, id, contact, email, 
        address, dob, type,
        balance, branch, pin_salt, pin_hash; 

        bool found = false;

        while (getline(file, file_acc) &&
               getline(file, name) &&
               getline(file, id) &&
               getline(file, contact) &&
               getline(file, email) &&
               getline(file, address) &&
               getline(file, dob) &&
               getline(file, type) &&
               getline(file, balance) &&
               getline(file, branch) &&
               getline(file, pin_salt) &&
               getline(file, pin_hash)){

                if(file_acc == acc_no){

                    found = true;

                    hash<string> hasher;
                    string attempt = to_string(hasher(pin + pin_salt));

                    if (attempt == pin_hash){

                        cout << "Login Successful" << "\n";
                        return true;
                    }else{
                        cout << "Incorrect PIN" << "\n";
                        break;
                    }
                }
            }

                    if (!found){
                        cout << "Account not found" << "\n";
                        return false;
                    }

                    attempts++;
                    cout << "Attempts left: " << (3 - attempts) << "\n";

                }
                cout << "Too many failed attempts. Account locked." << "\n";
                return false;

               }

bool load_customer(string acc_no, string pin_input,
                   string & file_acc,
                   string & file_name,
                   string & file_balance,
                   string & pin_salt,
                   string & pin_hash,
                   bool & valid){

            ifstream file("customers.dat");

            string sa_id, contact, email, address, dob, type, branch;

            valid = false;

            while (getline(file, file_acc) &&
                getline(file, file_name) &&
                getline(file, sa_id) &&
                getline(file, contact) &&
                getline(file, email) &&
                getline(file, address) &&
                getline(file, dob) &&
                getline(file, type) &&
                getline(file, file_balance) &&
                getline(file, branch) &&
                getline(file, pin_salt) &&
                getline(file, pin_hash))
                {

                    if (file_acc == acc_no){

                        hash<string> hasher;
                        string attempt = to_string(hasher(pin_input + pin_salt));

                        if (attempt == pin_hash){

                            valid = true;
                            return true;

                        }

                       

                    }

                
                }
    return false;
}

void view_balance(string acc_no, string pin){

    ifstream file("customers.dat");

    string file_acc, name, sa_id, contact, email,
           address, dob, type, balance,
           branch, pin_salt, pin_hash;

    while (getline(file, file_acc) &&
           getline(file, name) &&
           getline(file, sa_id) &&
           getline(file, contact) &&
           getline(file, email) &&
           getline(file, address) &&
           getline(file, dob) &&
           getline(file, type) &&
           getline(file, balance) &&
           getline(file, branch) &&
           getline(file, pin_salt) &&
           getline(file, pin_hash)){

            if (file_acc == acc_no){

                hash<string> hasher;
                string attempt = to_string(hasher(pin + pin_salt));

                if (attempt == pin_hash){

                    cout << "Balance: R" << balance<< "\n";
                }else{
                    cout << "Invalid PIN" << "\n";
                }

                return;

            }


        }

        cout << "Account Not Found" << "\n";
}

void log_transactions(string acc_no, string type, double amount){

    ofstream file("transactions.dat", ios::app);

    file << acc_no << " | "
         << type << " | "
         << amount << "\n";

}

void deposit(string acc_no){

    double amount;
    cout << "Enter Deposit Amount: " << "\n";
    cin >> amount;

    if (amount <=0){
        cout << "Invalid Amount" << "\n";
        return;
    }

    ifstream file("customers.dat");
    ofstream temp("temp.dat");

    string file_acc, name, sa_id, contact, email,
           address, dob, type, balance,
           branch, pin_salt, pin_hash;

    bool found = false;

    while (getline(file, file_acc) &&
           getline(file, name) &&
           getline(file, sa_id) &&
           getline(file, contact) &&
           getline(file, email) &&
           getline(file, address) &&
           getline(file, dob) &&
           getline(file, type) &&
           getline(file, balance) &&
           getline(file, branch) &&
           getline(file, pin_salt) &&
           getline(file, pin_hash)){

            if (file_acc == acc_no){

                found = true;

                double current_balance = stod(balance);
                current_balance += amount;

                balance = to_string(current_balance);

                cout << "Deposit Successful" << "\n";
                cout << "New Balanace: " << balance << "\n";
                
            }

            temp << file_acc << "\n"
                 << name << "\n"
                 << sa_id << "\n"
                 << contact << "\n"
                 << email << "\n"
                 << address << "\n"
                 << dob << "\n"
                 << type << "\n"
                 << balance << "\n"
                 << branch << "\n"
                 << pin_salt << "\n"
                 << pin_hash << "\n";


        }
        file.close();
        temp.close();

        log_transactions(acc_no, "Deposit", amount);

        remove("customers.dat");
        rename("temp.dat", "customers.dat");

        if (!found){

            cout << "Amount Not Found" << "\n";
        }
}

void withdraw(string acc_no){

    double amount;
    cout << "Enter Deposit Amount: " << "\n";
    cin >> amount;

    if (amount <= 0){
        cout << "Invalid Amount" << "\n";
        return;
    }

    ifstream file("customers.dat");
    ofstream temp("temp.dat");

    string file_acc, name, sa_id, contact, email,
           address, dob, type, balance,
           branch, pin_salt, pin_hash;

    bool found = false;

    while (getline(file, file_acc) &&
           getline(file, name) &&
           getline(file, sa_id) &&
           getline(file, contact) &&
           getline(file, email) &&
           getline(file, address) &&
           getline(file, dob) &&
           getline(file, type) &&
           getline(file, balance) &&
           getline(file, branch) &&
           getline(file, pin_salt) &&
           getline(file, pin_hash)){

            if (file_acc == acc_no){

                found = true;

                double current_balance = stod(balance);

                if (amount <= current_balance){

                    current_balance -= amount;

                    balance = to_string(current_balance);

                    cout << "Withdraw Successful" << "\n";
                    cout << "New Balanace: " << balance << "\n";

                }else{

                    cout << "Insufficient Funds!!!" << "\n"; 
                }
      
            }

            temp << file_acc << "\n"
                 << name << "\n"
                 << sa_id << "\n"
                 << contact << "\n"
                 << email << "\n"
                 << address << "\n"
                 << dob << "\n"
                 << type << "\n"
                 << balance << "\n"
                 << branch << "\n"
                 << pin_salt << "\n"
                 << pin_hash << "\n";


        }
        file.close();
        temp.close();

        remove("customers.dat");
        rename("temp.dat", "customers.dat");

        if (!found){

            cout << "Amount Not Found" << "\n";
        }

        log_transactions(acc_no, "Withdraw", amount);
}

void transfer(string sender_acc){

    string receiver_acc;
    double amount;

    cout << "Enter receiver account number: " << "\n";
    cin >> receiver_acc;

    cout << "Enter Transfer Amount: " << "\n";
    cin >> amount;

    if (amount <= 0){

        cout << "Invalid Amount!!!" << "\n";
        return;

    }

    ifstream file("customers.dat");
    ofstream temp("temp.dat");

    string file_acc, name, sa_id, contact, email,
           address, dob, type, balance,
           branch, pin_salt, pin_hash;

    bool sender_found = false;
    bool receiver_found = false;

    double sender_balance = 0;

    while (getline(file, file_acc) &&
           getline(file, name) &&
           getline(file, sa_id) &&
           getline(file, contact) &&
           getline(file, email) &&
           getline(file, address) &&
           getline(file, dob) &&
           getline(file, type) &&
           getline(file, balance) &&
           getline(file, branch) &&
           getline(file, pin_salt) &&
           getline(file, pin_hash)){

        double current_balance = stod(balance);

        if (file_acc == sender_acc){

            sender_found = true;

            if (amount > current_balance){

                cout << "Insufficient Funds!!!" << "\n";

                file.close();
                temp.close();
                remove("temp.dat");
                return;

            }else{

                current_balance -= amount;
                sender_balance = current_balance;
                balance = to_string(current_balance);

            }
        }

        else if (file_acc == receiver_acc){

            receiver_found = true;

            current_balance += amount;
            balance = to_string(current_balance);
        }

        temp << file_acc << "\n"
                 << name << "\n"
                 << sa_id << "\n"
                 << contact << "\n"
                 << email << "\n"
                 << address << "\n"
                 << dob << "\n"
                 << type << "\n"
                 << balance << "\n"
                 << branch << "\n"
                 << pin_salt << "\n"
                 << pin_hash << "\n";

           }

           file.close();
           temp.close();

           if (!sender_found){

            cout << "Sender Not Found!!!" << "\n";
            remove("temp.dat");
            return;

           }

           if (!receiver_found){

            cout << "Receiver Not Found!!!" << "\n";
            remove("temp.dat");
            return;

           }

           log_transactions(sender_acc, "Transfer Out", amount);
           log_transactions(receiver_acc, "Transfer In", amount);

           remove("customers.dat");
           rename("temp.dat", "customers.dat");

           cout << "Transfer Successful" << "\n";


}


void view_statement(string acc_no){

    ifstream file("customers.dat");

    string file_acc, name, sa_id, contact, email,
           address, dob, type, balance,
           branch, pin_salt, pin_hash;

    bool found = false;

    while (getline(file, file_acc) &&
           getline(file, name) &&
           getline(file, sa_id) &&
           getline(file, contact) &&
           getline(file, email) &&
           getline(file, address) &&
           getline(file, dob) &&
           getline(file, type) &&
           getline(file, balance) &&
           getline(file, branch) &&
           getline(file, pin_salt) &&
           getline(file, pin_hash)){

        if (file_acc == acc_no){

            found = true;

            cout << "Account Number: " << file_acc << "\n";
            cout << "Name: " << name << "\n";
            cout << "SA ID: " << sa_id << "\n";
            cout << "Contact: " << contact << "\n";
            cout << "Email: " << email << "\n";
            cout << "Account Type: " << type << "\n";
            cout << "Branch" << branch << "\n";
            cout << "Balance: R" << balance << "\n";

            ifstream trans("transactions.dat");
            string line;

            cout << "---Transactions---" << "\n";

            bool has_transactions = false;

            while (getline(trans, line)){

                if (line.find(acc_no) != string::npos){
                    cout << line << "\n";
                    has_transactions = true;

                }

            }

            if (!has_transactions){

                cout << "No Transactions Found!!!" << "\n";
            }

            return;

        }

        
    }

    if (!found){
        cout << "Account Not Found!!!" << "\n";
    }

}

void view_all_branches(vector<Branch> &branches){

    cout << "---Branches---" << "\n";

    for (auto &b : branches){
        cout << "Code: " << b.code << "\n";
        cout << "Name: " << b.name << "\n";
        cout << "Location: " << b.location << "\n";
    }
}

void view_branch_details(vector<Branch> &branches){

    string code;
    cout << "Enter Branch Code: " << "\n";
    cin >> code;

    for (auto &b : branches){

        if (b.code == code){

            cout << "Branch Found" << "\n";
            cout << "Name: " << b.name << "\n";
            cout << "Location: " << b.location << "\n";
            return;

        }
    }
}

void compare_branches(){

    ifstream file("customers.dat");

    string file_acc, name, sa_id, contact, email,
           address, dob, type, balance,
           branch, pin_salt, pin_hash;

    int b1 = 0, b2 = 0, b3 = 0;

    while (getline(file, file_acc) &&
           getline(file, name) &&
           getline(file, sa_id) &&
           getline(file, contact) &&
           getline(file, email) &&
           getline(file, address) &&
           getline(file, dob) &&
           getline(file, type) &&
           getline(file, balance) &&
           getline(file, branch) &&
           getline(file, pin_salt) &&
           getline(file, pin_hash)){

        if (branch == "B01"){

            b1++;

        }else if(branch == "B02"){

            b2++;

        }else if(branch == "B03"){

            b3++;

        }

        cout << "---Branch Comparison---" << "\n";
        cout << "B01 Customers: " << b1 << "\n";
        cout << "B02 Customers: " << b2 << "\n";
        cout << "B03" << b3 << "\n";


    }




}

double calc_interest(string type, double balance){

    double rate = 0;

    if (type == "1"){

        rate == 0.05;

    }else if(type == "2"){

        rate == 0.02;

    }else if(type == "3"){

        rate == 0.08;

    }else if(type == "4"){

        rate == 0.03;

    }

    return balance * rate;
}

bool verify_pin(string acc_no, string pin){

    ifstream file("customers.dat");

    string file_acc, name, sa_id, contact, email,
           address, dob, type, balance,
           branch, pin_salt, pin_hash;

        while (getline(file, file_acc) &&
           getline(file, name) &&
           getline(file, sa_id) &&
           getline(file, contact) &&
           getline(file, email) &&
           getline(file, address) &&
           getline(file, dob) &&
           getline(file, type) &&
           getline(file, balance) &&
           getline(file, branch) &&
           getline(file, pin_salt) &&
           getline(file, pin_hash)){

        if (file_acc == acc_no){

            hash<string> hasher;
            string attempt = to_string(hasher(pin + pin_salt));

            return attempt == pin_hash;
        }


    }

    return false;

}

void apply_interest(){

    ifstream file("customers.dat");
    ofstream temp("temp.dat");

    string file_acc, name, sa_id, contact, email,
           address, dob, type, balance,
           branch, pin_salt, pin_hash;

        while (getline(file, file_acc) &&
           getline(file, name) &&
           getline(file, sa_id) &&
           getline(file, contact) &&
           getline(file, email) &&
           getline(file, address) &&
           getline(file, dob) &&
           getline(file, type) &&
           getline(file, balance) &&
           getline(file, branch) &&
           getline(file, pin_salt) &&
           getline(file, pin_hash)){

        double current_balance = stod(balance);

        double interest = calc_interest(type, current_balance);

        current_balance += interest;

        balance = to_string(current_balance);

        temp << file_acc << "\n"
                 << name << "\n"
                 << sa_id << "\n"
                 << contact << "\n"
                 << email << "\n"
                 << address << "\n"
                 << dob << "\n"
                 << type << "\n"
                 << balance << "\n"
                 << branch << "\n"
                 << pin_salt << "\n"
                 << pin_hash << "\n";

           }

           file.close();
           temp.close();

           remove("customers.dat");
           rename("temp.dat", "customers.dat");

           cout << "Interest Applied Successfully" << "\n";

}

void search_by_account(){

    string acc_no;
    cout << "Enter Account Number: ";
    cin >> acc_no;

    ifstream file("customers.dat");

    string file_acc, name, sa_id, contact, email,
           address, dob, type, balance,
           branch, pin_salt, pin_hash;

        bool found = false;

        while (getline(file, file_acc) &&
           getline(file, name) &&
           getline(file, sa_id) &&
           getline(file, contact) &&
           getline(file, email) &&
           getline(file, address) &&
           getline(file, dob) &&
           getline(file, type) &&
           getline(file, balance) &&
           getline(file, branch) &&
           getline(file, pin_salt) &&
           getline(file, pin_hash)){

        if (file_acc == acc_no){

            found = true;

            cout << "---Customer Details---" << "\n";
            cout << "Account: " << file_acc << "\n";
            cout << "Name: " << name << "\n";
            cout << "Balance: R" << balance << "\n";
            cout << "Branch: " << branch << "\n";

            return;

        }

    }

    if (!found){

        cout << "Account Not Found!!!" << "\n";
    }

}

void search_by_name(){

    string search_name;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, search_name);

    ifstream file("customers.dat");

    string file_acc, name, sa_id, contact, email,
           address, dob, type, balance,
           branch, pin_salt, pin_hash;

    bool found = false;

    while (getline(file, file_acc) &&
           getline(file, name) &&
           getline(file, sa_id) &&
           getline(file, contact) &&
           getline(file, email) &&
           getline(file, address) &&
           getline(file, dob) &&
           getline(file, type) &&
           getline(file, balance) &&
           getline(file, branch) &&
           getline(file, pin_salt) &&
           getline(file, pin_hash)){

        if (name.find(search_name) != string::npos){

            found = true;

            cout << "---Match---" << "\n";
            cout << "Account: " << file_acc << "\n";
            cout << "Name: " << name << "\n";
            cout << "Balance: R" << balance << "\n";
        }
    }

    if (!found){

        cout << "No Customer Match!!!" << "\n";

    }

}

void filter_by_branch(){

    string code;
    cout << "Enter Branch Code: ";
    cin >> code;

    ifstream file("customers");

    string file_acc, name, sa_id, contact, email,
           address, dob, type, balance,
           branch, pin_salt, pin_hash;

    bool found = false;

    while (getline(file, file_acc) &&
           getline(file, name) &&
           getline(file, sa_id) &&
           getline(file, contact) &&
           getline(file, email) &&
           getline(file, address) &&
           getline(file, dob) &&
           getline(file, type) &&
           getline(file, balance) &&
           getline(file, branch) &&
           getline(file, pin_salt) &&
           getline(file, pin_hash)){

        if (branch == code){

            found = true;

            cout << "---Customer---" << "\n";
            cout << "Account: " << file_acc << "\n";
            cout << "Name: " << name << "\n";
            cout << "Balance: R" << balance << "\n";

        }

    }

    if (!found){

        cout << "No Customer Match For Branch!!!" << "\n";

    }

}


int main(){



    bool run = true;

    while (run == true){

        int choice;

        cout << "Enter Choice: " << "\n";

        cout << "1. Create Teller Account" << "\n";

        cout << "2. Login As Teller" << "\n";

        cout << "3. Login As Customer" << "\n";

        cout << "4. Exit" << "\n";

        cin >> choice;

        if (choice == 1){

           
            create_teller_account();
           

        }else if(choice == 2){

            string identify; 
            string pass_word;

            cout << "Please Enter ID & Password Below: " << "\n";

            cout << "ID Number:" << "\n";

            cin >> identify;

            cout << "Password:" << "\n";

            cin >> pass_word;

            string salt = "XyZ123";

            string salted = pass_word + salt;

            hash<string> hash_pass;
            size_t hashed_password = hash_pass(salted);

            string pass = to_string(hashed_password);

            ifstream file("tellers.dat");

            bool found = false;

            string file_id, file_name, file_salt, file_password, file_branch;

            while (getline(file, file_id) && 
                   getline(file, file_name) && 
                   getline(file, file_salt) &&
                   getline(file, file_password) &&
                   getline(file, file_branch)){

                string attempt = to_string(hash_pass(pass_word + file_salt));

                if (file_id == identify && attempt == file_password){
                    found = true;
                    cout << "Login Successful" << "\n";

                    bool teller_session = true;

                    while (teller_session){

                        int choices;

                        cout << "---Teller Menu---" << "\n";
                        cout << "1. Register New Customer" << "\n";
                        cout << "2. Deposit For Customer" << "\n";
                        cout << "3. Withdraw For Customer" << "\n";
                        cout << "4. Transfer For Customer" << "\n";
                        cout << "5. View All Branches" << "\n";
                        cout << "6. View Branch Details" << "\n";
                        cout << "7. Compare Branches" << "\n";
                        cout << "8. Apply Interest" << "\n";
                        cout << "9. Search For Customer" << "\n";
                        cout << "10. Logout" << "\n";

                        cin >> choices;

                        if (choices == 1){

                            register_cust(file_branch);

                        }else if(choices == 2){

                            string acc_no;
                            string pin;

                            cout << "Enter Customer Account Number: " << "\n";
                            cin >> acc_no;


                            cout << "Enter Customer PIN: " << "\n";
                            cin >> pin;

                            if (verify_pin(acc_no, pin)){

                                deposit(acc_no);

                            }else{

                                cout << "Invalid PIN!!!" << "\n";

                            }
                        }else if(choices == 3){

                            string acc_no;
                            string pin;

                            cout << "Enter Customer Account Number: " << "\n";
                            cin >> acc_no;

                            cout << "Enter Customer PIN: " << "\n";
                            cin >> pin;

                            if (verify_pin(acc_no, pin)){

                                withdraw(acc_no);

                            }else{

                                cout << "Invalid PIN!!!" << "\n";

                            }
                        }else if (choices == 4){

                            string acc_no;
                            string pin;

                            cout << "Enter Sender Account Number: " << "\n";
                            cin >> acc_no;

                            cout << "Enter Customer PIN: " << "\n";
                            cin >> pin;

                            if (verify_pin(acc_no, pin)){

                                transfer(acc_no);

                            }else{

                                cout << "Invalid PIN!!!" << "\n";

                            }

                        }else if(choices == 5){

                            view_all_branches(branches);

                        }else if(choices == 6){

                            view_branch_details(branches);

                        }else if(choices == 7){

                            compare_branches();

                        
                        
                        }else if(choices == 10){

                            teller_session = false;
                            cout << "Logging Out" << "\n";

                        }else{

                            cout << "Invalid Option!!" << "\n";

                        }
                    }

                    break; 
                            
                } 

                        
                    }
            if (!found){

                    cout << "Invalid Id or Password!!" << "\n";
                }

            }else if(choice == 3){

                string acc_no;
                
                if (login_customer(acc_no)){

                    bool session = true;

                    while (session){

                        int opt;

                        cout << "---Customer Menu---" << "\n";
                        cout << "1. View Balance" << "\n";
                        cout << "2. Deposit" << "\n";
                        cout << "3. Withdraw" << "\n";
                        cout << "4. Transfer" << "\n";
                        cout << "5. View Statement" << "\n";
                        cout << "6. Logout" << "\n";

                        cin >> opt;

                        if (opt == 1){
                             string pin;
                             cout << "Enter PIN again: " << "\n";
                             cin >> pin;

                             view_balance(acc_no, pin);

                        }else if(opt == 2){

                            string pin;
                            cout << "Enter PIN: " << "\n";
                            cin >> pin;

                            if (verify_pin(acc_no, pin)){
                                deposit(acc_no);
                            }else{

                                cout << "Invalid PIN!!!" << "\n";
                            }

                        }else if(opt == 3){

                            string pin;
                            cout << "Enter PIN: " << "\n";
                            cin >> pin;

                            if (verify_pin(acc_no, pin)){
                                withdraw(acc_no);
                            }else{

                                cout << "Invalid PIN!!!" << "\n";
                            }

                        }else if(opt == 4){

                            string pin;
                            cout << "Enter PIN: " << "\n";
                            cin >> pin;

                            if (verify_pin(acc_no, pin)){
                                transfer(acc_no);
                            }else{

                                cout << "Invalid PIN!!!" << "\n";
                            }

                        }else if(opt == 5){

                            view_statement(acc_no);

                        }else if(opt == 6){

                            session = false;

                        }
                    }


                }

            }else if(choice == 4){

            run = false;

        }
        
    }
   
    return 0;
}