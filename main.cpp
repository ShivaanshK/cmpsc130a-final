#include "ds.h"
#include <fstream>
#include <sstream>

int main() {
    RBT rb;
    std::string line;
    std::ifstream infile("bruh.txt");
    while (getline(infile, line)) {
        std::istringstream current_line(line);
        while (current_line) {
            std::string name;
            if (getline(current_line, name, ',')) {
                rb.insert(name);
            }
            else {
                break;
            }
        }
    }
    infile.close();

    std::cout << "Printing names in order" << std::endl;
    rb.print_all();

    std::cout << "Printing from amr to rashid" << std::endl;
    std::string a = "amr";
    std::string b = "rashid";
    rb.range_query(a,b);

    std::cout << "Printing just amr" << std::endl;
    rb.exact_query(a);

    std::cout << "Printing just hamud" << std::endl;
    std::string c = "hamud";
    rb.exact_query(c);

    return 0;

}
