#include <iostream>
#include <string>
#include <fstream>
#include"Graph.h"
using namespace std;

int main(int argc, char *argv[])
{
    Graph graph;
    ifstream infile;
    infile.open(string(argv[1]));
    string line;
    int counter = 0;
    while (getline(infile, line,'\n'))
    {
        graph.insert(line);
        line = "";
    } 
    infile.close();

    graph.printGraph();

    return 1;
}