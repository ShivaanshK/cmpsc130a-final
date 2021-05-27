#include "RBGraph.h"

int main(int argc, char *argv[])
{
    RBGraph rbgraph = RBGraph();
    std::ifstream infile;
    infile.open(std::string(argv[1]));
    std::string line;
    int counter = 0;
    while (std::getline(infile, line,'\n'))
    {
        if (line != "" && counter != 0)
            rbgraph.insert(line);
        line = "";
        counter++;
    } 
    infile.close();
    infile.clear();

    std::cout << "Printing All Persons' Name, Age, Occupation, Friends ..." << std::endl;
    rbgraph.print_all();

    std::string person1 = "Elizabeth Yang";
    std::string person2 = "Sarah Perez";
    std::cout << "Printing Name and Occupation from " << person1 << " to " << person2 << "..." << std::endl;
    
    rbgraph.range_query(person1, person2);


    std::cout << "Printing Each of " << person1 << "'s Friend's Name and Occupation ..." << std::endl;
    rbgraph.friendship_query(person1);


    rbgraph.print_Graph();

    return 0;
}
