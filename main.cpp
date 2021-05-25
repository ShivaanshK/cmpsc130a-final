#include <iostream>
#include <string>
#include <fstream>
#include"RBGraph.h"
using namespace std;

int main(int argc, char *argv[])
{
    RBGraph rbgraph = RBGraph();
    ifstream infile;
    infile.open(string(argv[1]));
    string line;
    int counter = 0;
    while (getline(infile, line,'\n'))
    {
        if(line != "" && counter != 0) rbgraph.insert(line);
        line = "";
        counter ++;
    } 
    infile.close();
    infile.clear();

    return 1;
}