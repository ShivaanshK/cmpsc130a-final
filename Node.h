#include<string>

struct Linked_List_Node{
    Linked_List_Node(){next = NULL;};
    std::string name;
    Linked_List_Node* next;
};