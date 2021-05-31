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

    std::cout << "/* TEST I: PRINT ADJACENCY LIST */" << std::endl;
    std::cout << "Printing The Adjacency List ..." << std::endl;
    rbgraph.print_graph();

    std::cout << "/* TEST II: PRINT ALL */" << std::endl;
    std::cout << "Printing All Persons' Name, Age, Occupation, Friends ..." << std::endl;
    rbgraph.print_all();
    
    std::string person1 = "Aaron Jones"; // in disk
    std::string person2 = "Todd Campbell";
    std::string person3 = "Adelheid Arete"; // not in disk
    std::string person4 = "Eric Eszterhazy"; 

    std::cout << "/* TEST III: PRINT RANGE, BOTH IN FILE DISK */" << std::endl;
    std::cout << "Printing Name and Occupation from " << person1 << " to " << person2 << "..." << std::endl;
    rbgraph.range_query(person1, person2); // prints from Aaron Jones to Todd Campbell

    std::cout << "/* TEST IV: PRINT RANGE, LOWER BOUND NOT IN FILE DISK */" << std::endl;
    std::cout << "Printing Name and Occupation from " << person3 << " to " << person2 << "..." << std::endl;
    rbgraph.range_query(person3, person2); // prints from Alice Todd to Todd Campbell

    std::cout << "/* TEST V: PRINT RANGE, UPPER BOUND NOT IN FILE DISK */" << std::endl;
    std::cout << "Printing Name and Occupation from " << person1 << " to " << person4 << "..." << std::endl;
    rbgraph.range_query(person1, person4); // prints Aaron Jones to Eric Cook
    
    std::cout << "/* TEST VI: PRINT RANGE, BOTH NOT IN FILE DISK */" << std::endl;
    std::cout << "Printing Name and Occupation from " << person3 << " to " << person4 << "..." << std::endl;
    rbgraph.range_query(person3, person4); // prints Alice Todd to Eric Cook

    std::cout << "/* TEST VII: PRINT RANGE, BOUNDS ARE SMALLER/LARGER THAN ALL ENTRIES, EDGE CASE */" << std::endl;
    person3 = "Aadi Prithvipati";
    person4 = "Wojtas Grochowsky"; // prints from Aaron Jones to William Richardson
    std::cout << "Printing Name and Occupation from " << person3 << " to " << person4 << "..." << std::endl;
    rbgraph.range_query(person3, person4);


    std::cout << "/* TEST VIII: FRIENDSHIP QUERIES */" << std::endl;
    std::cout << "Printing Each of " << person1 << "'s Friend's Name and Occupation ..." << std::endl;
    rbgraph.friendship_query(person1);

    std::cout << "Printing Each of " << person2 << "'s Friend's Name and Occupation ..." << std::endl;
    rbgraph.friendship_query(person2);

    std::cout << "/* TEST IX: TEST ADD_FRIEND WITH CHECK UPDATED FRIEND LISTS */" << std::endl;
    std::cout << "Adding " << person2 << " to " << person1 << "'s Friend's List ..." << std::endl;
    rbgraph.ginsert_friend(person1, person2);
    std::cout << "Next, Checking if " << person1 << " and " << person2 << " have Updated Friend Lists ... " << std::endl;
    std::cout << "Printing Each of " << person1 << "'s Friend's Name and Occupation ..." << std::endl;
    rbgraph.friendship_query(person1);
    std::cout << "Printing Each of " << person2 << "'s Friend's Name and Occupation ..." << std::endl;
    rbgraph.friendship_query(person2);

    std::cout << "/* TEST X: TEST GRAPH, RB-TREE, and FILE DISK after INDIVIDUAL INSERT*/" << std::endl;
    std::cout << "Inserting Wojtas Grochowsky, 10, Unemployed via the Graph Insert" << std::endl;
    std::string nombre("Wojtas Grochowsky");
    std::string edad("10");
    std::string ocupacion("Unemployed");
    rbgraph.ginsert(nombre, edad, ocupacion);
   
    std::cout << "Printing Each of " << nombre << "'s Friend's Name and Occupation." << std::endl;
    std::cout << "This invokes RB-TREE FIND and information from the File Disk." << std::endl;
    rbgraph.friendship_query(nombre);
   
    std::cout << "Next, adding a friend, " << person1 << std::endl;
    rbgraph.ginsert_friend(person1,nombre);

    std::cout << "Printing Each of " << nombre << "'s Friend's Name and Occupation again." << std::endl;
    rbgraph.friendship_query(nombre);
    
    std::cout << "Printing Each of " << person1 << "'s Friend's Name and Occupation ..." << std::endl;
    rbgraph.friendship_query(person1);

    std::cout << "Finally, printing the Graph:" << std::endl;
    rbgraph.print_graph();

    return 0;
}
