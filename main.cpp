#include "RBGraph.h"

int main(int argc, char *argv[])
{
    RBGraph rbgraph = RBGraph();
    std::ifstream infile;
    infile.open(std::string(argv[1]));
    std::string line;
    int counter = 0;
    while (std::getline(infile, line, '\n'))
    {
        if (line != "" && counter != 0)
            rbgraph.insert(line);
        line = "";
        counter++;
    }
    infile.close();
    infile.clear();

    std::string input1;
    std::string input2;
    std::string input3;
    while (true)
    {
        std::cout << "Select what action you would like to do:" << std::endl;
        std::cout << "[1] Friendship query" << std::endl;
        std::cout << "[2] Range query" << std::endl;
        std::cout << "[3] Add user" << std::endl;
        std::cout << "[4] Insert friendship" << std::endl;
        std::cout << "[5] Print all users and their friends" << std::endl;
        std::cout << "[6] Print Graph" << std::endl;
        std::cout << "[7] Quit" << std::endl;
        std::cout << "Enter your Selection: ";
        std::getline(std::cin, input1);
        if (input1 == "1")
        {
            std::cout << std::endl
                      << "Enter name: ";
            std::getline(std::cin, input1);
            std::cout << std::endl;
            rbgraph.friendship_query(input1);
            continue;
        }
        if (input1 == "2")
        {
            std::cout << std::endl
                      << "Enter lower bound of range: ";
            std::getline(std::cin, input1);
            std::cout << std::endl
                      << "Enter upper bound of range: ";
            std::getline(std::cin, input2);
            std::cout << std::endl;
            rbgraph.range_query(input1, input2);
            continue;
        }
        if (input1 == "3")
        {
            std::cout << std::endl
                      << "Enter name of new user: ";
            std::getline(std::cin, input1);
            std::cout
                << "Enter age of new user: ";
            std::getline(std::cin, input2);
            std::cout
                << "Enter occupation of new user: ";
            std::getline(std::cin, input3);
            std::cout << std::endl;
            rbgraph.ginsert(input1, input2, input3);
            std::cout << "Successfully inserted " << input1 << std::endl
                      << std::endl;
            continue;
        }
        if (input1 == "4")
        {
            std::cout << std::endl
                      << "Enter name of first user: ";
            std::getline(std::cin, input1);
            std::cout
                << "Enter name of second user: ";
            std::getline(std::cin, input2);
            rbgraph.ginsert_friend(input1, input2);
            std::cout << std::endl
                      << "Successfully inserted friendship between " << input1 << " and " << input2 << std::endl
                      << std::endl;
            continue;
        }
        if (input1 == "5")
        {
            rbgraph.print_all();
            continue;
        }
        if (input1 == "6")
        {
            rbgraph.print_graph();
            continue;
        }
        if (input1 == "7")
            break;
    }

    return 0;
}
