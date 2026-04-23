#ifndef BRANCH_H
#define BRANCH_H

#include <string>
#include <vector>

using namespace std;

struct Branch{
    string code;
    string name;
    string location;
};

void init_default_branches(vector<Branch> &branches);

void load_branches(vector<Branch> &branches);

void save_branches(vector<Branch> &branches);

void view_all_branches(vector<Branch> &branches);

void view_branch_details(vector<Branch> &branch);

void compare_branches();

#endif