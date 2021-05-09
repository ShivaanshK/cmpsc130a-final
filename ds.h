// ds.h (data structure file)

#ifndef DS_H
#define DS_H

#include <iostream>
#include <string>
#include <vector>
#include <utility> // std::pair<std::string, std::vector<i>>

typedef uint32_t i; // indices are never negative
typedef std::vector<std::pair<std::string, std::vector<i>>> adjlist;

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

// Red-Black Tree and Friendship Graph
class RBFG {
    public:
        RBFG();
        ~RBFG();
       
        // functions for rb-tree
        Node* find(std::string& name, Node* n) const;
        std::vector<Node*> findrange(std::string& name1, std::string& name2) const;
        bool insert(std::string& name, i graph_index, i file_index);
        
        // functions for rb-tree & f-graph
        void range_query(std::string& name1, std::string& name2) const;
        void print_all() const;
    
        // functions for f-graph
        void add_edge(std::vector<std::vector<i>> alist, std::string& name1, std::string& name2);

    private:
        // rb-tree
        Node* root; // implemented with linked lists

        // helpers for rb-tree
        void yeet(Node* n); // recursive destructor helper
        bool insert(std::string& name, i graph_index, i file_index, Node* n); // recursive insert helper
        void rotate_left();
        void rotate_right();
        Node* successor(std::string& name) const; // findrange helper
        
        // f-graph
        adjlist alist; // implemented with adjacency list
};

#endif
