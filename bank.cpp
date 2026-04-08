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

    ofstream out_file("data.data");

    Teller my_obj;

    cout << "Enter Teller ID: " << "\n";

    cin >> my_obj.id;

    cout << "Enter Teller Full Name: " << "\n";

    cin >> my_obj.full_name;

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

        cout << "1. Create Account" << "\n";

        cout << "2. View Account Details" << "\n";

        cout << "4. Exit" << "\n";

        cin >> choice;

        if (choice == 1){

           create_account();

           run = false;

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

            cout << pass;

            ifstream file("data.data");
            string word, target = identify;
            string wordd, targett = pass;

            bool found = false;

            while (file >> word){
                if (wordd == targett){
                    found = true;
                    cout << "word found" << "\n";
                    run = false;
                } 
            }


        }else if(choice == 4){

            run = false;

        }
        
    }
   
    return 0;
}