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
        if(line != "" && counter != 0) graph.insert(line);
        line = "";
        counter ++;
    } 
    infile.close();
    infile.clear();

    string* output = graph.readFile(0);
    cout << *(output) << ", " + *(output + 1) << ", " + *(output + 2) << endl; 
    // graph.printGraph();

    return 1;
}