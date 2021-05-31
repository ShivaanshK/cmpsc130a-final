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

    std::cout << "Printing The Adjacency List ..." << std::endl;
    rbgraph.print_graph();

    std::cout << "Printing All Persons' Name, Age, Occupation, Friends ..." << std::endl;
    rbgraph.print_all();
    
    std::string person1 = "Eric Cook";
    std::string person2 = "Todd Campbell";
    // the persons below are not found in users100
    // std::string person1 = "Elizabeth Yang"; 
    // std::string person2 = "Sarah Perez";
    std::cout << "Printing Name and Occupation from " << person1 << " to " << person2 << "..." << std::endl;
    
    rbgraph.range_query(person1, person2);

    std::string person3 = "Adelheid Arete";
    std::string person4 = "Eric Eszterhazy"; // should print from Alice to Eric Cook
    std::cout << "Printing Name and Occupation from " << person3 << " to " << person4 << "..." << std::endl;

    rbgraph.range_query(person3, person4);

    std::cout << "Printing Each of " << person1 << "'s Friend's Name and Occupation ..." << std::endl;
    rbgraph.friendship_query(person1);

    std::cout << "Printing Each of " << person2 << "'s Friend's Name and Occupation ..." << std::endl;
    rbgraph.friendship_query(person2);

    std::cout << "Adding " << person2 << " to " << person1 << "'s Friend's List ..." << std::endl;
    rbgraph.ginsert_friend(person1, person2);

    std::cout << "Checking if " << person1 << " and " << person2 << " have Updated Friend Lists ... " << std::endl;
    std::cout << "Printing Each of " << person1 << "'s Friend's Name and Occupation ..." << std::endl;
    rbgraph.friendship_query(person1);

    std::cout << "Printing Each of " << person2 << "'s Friend's Name and Occupation ..." << std::endl;
    rbgraph.friendship_query(person2);

    return 0;
}
