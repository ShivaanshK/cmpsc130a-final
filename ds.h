// ds.h (data structure file)

#ifndef DS_H
#define DS_H

#include <iostream>
#include <string>
#include <vector>

typedef uint32_t i; // indices are never negative

struct Node {
    Node(const std::string& n = std::string{}, i gi = 0, i fi = 0, Node* p = nullptr, Node* l = nullptr, Node* r = nullptr, int c = black)
        : name{n}, graph_index{gi}, file_index{fi}, parent{p}, left{l}, right(r), color{c} {}
    
    std::string name;
    i graph_index;
    i file_index;
    Node* parent;
    Node* left;
    Node* right;
    int color;
};

// Red-Black Tree and Friendship Graph
class RBFG {
    public:
        RBFG();
        ~RBFG();
        
        const static int red = 0;
        const static int black = 1;

        // functions for rb-tree
        Node* find(std::string& name, Node* n) const;
        std::vector<Node*> findrange(std::string& name1, std::string& name2) const;
        bool insert(std::string& name, i graph_index, i file_index);
        
        // functions for rb-tree & f-graph
        void range_query(std::string& name1, std::string& name2) const;
        void print_all() const;
    
    private:
        // rb-tree
        Node* root; // implemented with linked lists

        // helpers for rb-tree
        void yeet(Node* n); // recursive destructor helper
        bool insert(std::string& name, i graph_index, i file_index, Node* n); // recursive insert helper
        void rotate_left();
        void rotate_right();
        Node* successor(std::string& name) const; // findrange helper
        
};

#endif
