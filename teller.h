#ifndef TELLER_H
#define TELLER_H

#include <string>

using namespace std;

class Teller{
    public:
        string id;
        string full_name;
        string password;
        string branch_code;
};

int create_teller_account();

bool login_teller(string &branch_code);

#endif