// ds.h (data structure file)

#ifndef DS_H
#define DS_H

#include <iostream>
#include <string>
#include <vector>

typedef uint32_t i;

struct Node {
    Node(const std::string& n = std::string{}, i gi = 0, i fi = 0)
        : name{n}, graph_index{gi}, file_index{fi}, parent(nullptr), left(nullptr), right(nullptr) {}
    
    std::string name;
    i graph_index;
    i file_index;
    Node* parent;
    Node* left;
    Node* right;
};

class RBFG {
    public:
        RBFG();
        ~RBFG();
       
        // functions for rb-tree
        Node* find(std::string& name, Node* n) const;
        std::vector<std::string> findrange(std::string& name1, std::string& name2) const;
        bool insert(std::string& name, i graph_index, i file_index);
        
        // functions for rb-tree & f-graph
        void print_range(std::string& name1, std::string& name2) const;
        void print_all() const;
        
    private:
        Node* root;

        void yeet(Node* n); // recursive destructor helper
        bool insert(std::string& name, i graph_index, i file_index, Node* n); // recursive insert helper
        Node* successor(std::string& name) const; // findrange helper
};

#endif
