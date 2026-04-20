#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <random>


using namespace std;

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

                return false;
            }

        
           }
    
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

        remove("customers.dat");
        rename("temp.dat", "customers.dat");

        if (!found){

            cout << "Amount Not Found" << "\n";
        }
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

                        cout << "1. Register New Customer" << "\n";
                        cout << "2. Logout" << "\n";

                        cin >> choices;

                        if (choices == 1){

                            register_cust(file_branch);

                        }else if(choices == 2){

                            teller_session = false;
                            cout << "Logging Out" << "\n";

                        }else{

                            cout << "Invalid Option" << "\n";

                        }
                    }

                    break; 
                            
                } 

                        
                    }
            if (!found){

                    cout << "Invalid Id or Password" << "\n";
                }

            }else if(choice == 3){

                string acc_no;
                
                if (login_customer(acc_no)){

                    bool session = true;

                    while (session){

                        int opt;

                        cout << "***Customer Menu***" << "\n";
                        cout << "1. View Balance" << "\n";
                        cout << "2. Deposit" << "\n";
                        cout << "4. Withdraw" << "\n";
                        cout << "4. Logout" << "\n";

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

                                cout << "Invalid PIN" << "\n";
                            }

                        }else if(opt == 3){

                        }else if(opt == 4){

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