#include <iostream>
#include <string>

int main(int argc, char** argv) {
    for (std::string line; std::getline(std::cin, line);) {
        std::cout << line << std::endl;
    }
    return 0;
}
