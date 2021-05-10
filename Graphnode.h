#include<string>
#include"Node.h"
struct Graphnode{
    Graphnode(){list = NULL;};
    std::string name;
    Node* list;
    int filePointer;
};