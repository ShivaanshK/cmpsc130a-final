#include<string>

struct Node{
    Node(){next = NULL;};
    std::string name;
    Node* next;
};