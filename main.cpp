#include "ds.h"

void insert_disk();

int main() {
    std::string input;
    while(1) {
        std::cout << "welcomeeeeee" << std::endl;
        std::cout << "Insert Name Age \"Profession\"" << std::endl;
        std::cout << "AddFriend Name Friend" << std::endl;
        std::cin >> input;

        if (input.substr(0,6) == "Insert") {
            std::cout << "inserted!" << std::endl;
        }
        else if (input.substr(0,10) == "AddFriend") {
            std::cout << "friend added!" << std::endl;
        }
    }
    return 0;
}
