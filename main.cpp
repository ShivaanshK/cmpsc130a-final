#include <iostream>
#include <string>
#include <fstream>
#include"RBGraph.h"

int main(int argc, char *argv[])
{
    RBGraph rbgraph = RBGraph();
    std::ifstream infile;
    infile.open(std::string(argv[1]));
    std::string line;
    int counter = 0;
    while (std::getline(infile, line,'\n'))
    {
        if(line != "" && counter != 0) rbgraph.insert(line);
        line = "";
        counter ++;
    } 
    infile.close();
    infile.clear();

    return 1;
}