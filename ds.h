// ds.h (data structure file)
#ifndef DS_H
#define DS_H

#include <iostream>
#include <string>
#include <vector> // for std::vector<Node*> in findrange()
#include <utility> // for std::swap in fix()

typedef uint32_t i; // indices are never negative
const static bool red = true;
const static bool black = false;

/* IMPLEMENTATION OF RED-BLACK NODE */
class Node {
    public:
        std::string name;
        bool color;
        i graph_index;
        Node* left;
        Node* right;
        Node* parent;

        // constructor 
        Node(std::string n = std::string{}, bool c = red, i gi = 0, Node* l = nullptr, Node* r = nullptr, Node* p = nullptr)
            : name{n}, color{c}, graph_index{gi}, left{l}, right{r}, parent{p} {}
        
        // by default nodes are red; this is okay, because the 'null' nodes are considered black
};
 
/* BOTTOM-UP IMPLEMENTATION OF RED-BLACK TREE */
class RBT {
    public:
        // constructor
        RBT();
        // destructor
        ~RBT();

        // query (exact, range, or all)
        void exact_query(const std::string& name) const;
        void range_query(const std::string& name1, const std::string& name2) const;
        void print_all() const;
        
        // insert
        void insert(const std::string& n);

    private:
        Node* root; // RBT
        i elements; // number of nodes

        // destructor helper
        void clear(Node* n);

        // print helper
        void print(Node* n) const;

        // find (exact or range), query helpers
        Node* find(const std::string& name, Node* n) const;
        std::vector<Node*> find_range(const std::string& name1, const std::string& name2) const;
        Node* successor(const std::string& name) const;

        // insert helper (BST portion)
        Node* binsert(Node* root, Node* entry);

        // insert helper (fix portion)
        void fix(Node*& r, Node*& entry);

        // rotations (fix helper)
        void lrotate(Node*& r, Node*& entry);
        void rrotate(Node*& r, Node*& entry);
};
 
#endif
