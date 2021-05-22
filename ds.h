// ds.h (data structure file)

#ifndef DS_H
#define DS_H

#include <iostream>
#include <string>
#include <vector>

typedef uint32_t i; // indices are never negative
const static bool red = true;
const static bool black = false;

struct Node {
    Node(const std::string& n = std::string{}, i gi = 0, Node* l = nullptr, Node* r = nullptr, bool c = black)
        : name{n}, graph_index{gi}, left{l}, right(r), color{c} {}
    
    std::string name;
    i graph_index;
    Node* left;
    Node* right;
    bool color;
};

// Top-Down Red-Black Tree
class RBFG {
    public:
        RBFG();
        ~RBFG();

        // functions for rb-tree
        Node* find(std::string& name, Node* n) const;
        std::vector<Node*> findrange(std::string& name1, std::string& name2) const;
        void insert(std::string& name);

        // functions for rb-tree & f-graph
        void range_query(std::string& name1, std::string& name2) const;
        void print_all(Node* n) const;
    
    private:
        i elements; // number of nodes

        // rb-tree
        Node* root; // sentinel root node
        Node* nullnode; // sentinel null node
        
        // nodes used in insert and its subroutines
        Node* traverse;
        Node* pt;
        Node* gpt;
        Node* ggpt;

        // helpers for rb-tree
        void yeet(Node* n); // recursive destructor helper

        void fix(std::string& name); // insert helper
        Node* rotate(std::string& name, Node* this_pt); // fix helper
        void lrotate(Node*& n); // rotate helper
        void rrotate(Node*& n); // rotate helper

        Node* successor(std::string& name) const; // findrange helper
};

#endif
