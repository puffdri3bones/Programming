#include <iostream>
#include <fstream>
#include <string>
#include <functional>


using namespace std;

//Teller ID, Full Name, Password, Branch Code

class Teller{
    public:
        string id;
        string full_name;
        string password;
        string branch_code;

        
};



int create_account(){

    ofstream out_file("data.dat");

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

    hash<string> pass_hash;    

    size_t hashed_password = pass_hash(my_obj.password);

    cout << hashed_password;

        if (out_file.is_open()){

            out_file << my_obj.id << "\n";

            out_file << my_obj.full_name << "\n";

            out_file << hashed_password << "\n";

            out_file << my_obj.branch_code << "\n";


        }

    return hashed_password;
}


int main(){


    bool run = true;

    while (run == true){

        int choice;

        cout << "Enter Choice: " << "\n";

        cout << "1. Create Teller Account" << "\n";

        cout << "2. Login As Teller" << "\n";

        cout << "4. Exit" << "\n";

        cin >> choice;

        if (choice == 1){

           
            create_account();
           

        }else if(choice == 2){

            string identify;
            string pass_word;

            cout << "Please Enter ID & Password Below: " << "\n";

            cout << "ID Number:" << "\n";

            cin >> identify;

            cout << "Password:" << "\n";

            cin >> pass_word;

            hash<string> hash_pass;
            size_t hashed_password = hash_pass(pass_word);

            string pass = to_string(hashed_password);

            ifstream file("data.dat");

            bool found = false;

            string file_id, file_name, file_password, file_branch;

            while (getline(file, file_id) && 
                   getline(file, file_name) && 
                   getline(file, file_password) &&
                   getline(file, file_branch)){

                if (file_id == identify && file_password == pass){
                    found = true;
                    cout << "Login Successful" << "\n";

                    int choices;

                    cout << "1. Register New Customer" << "\n";

                    run = false;

                    if (choices == 1){

                        cout << "enter customer details";

                    }
                    

                } 

                if (!found){

                    cout << "Invalid Id or Password" << "\n";
                }
            }


        }else if(choice == 4){

            run = false;

        }
        
    }
   
    return 0;
}