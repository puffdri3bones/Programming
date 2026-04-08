#include <iostream>
#include <string>
#include <functional> // Required for std::hash
#include <iomanip> // For std::setw and std::setfill
#include <format>

int main() {
    std::string text = "hello";
    std::hash<std::string> string_hasher;
    
    // Generate the hash
    size_t hashValue = string_hasher(text);
    
    std::cout << "Hash of 'hello': " << hashValue << std::endl;


    std::string data = "example";
    std::size_t hash_val = std::hash<std::string>{}(data);


    std::cout << "Hex: 0x" << std::hex << hash_val << std::endl;


    std::string hex_str = format("{:x}", hash_val); // Lowercase hex

    printf("%lx\n", (unsigned long)hash_val);

    return 0;
}