#include <iostream>
#include <fstream>
#include <string>

int main(){

std::ifstream file("data.data");
std::string word, target = "hello";
bool found = false;

while (file >> word) {
    if (word == target) {
        found = true;
        std::cout << "good";
        break;
    }
}



    return 0;
}