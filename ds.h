// ds.h (data structure file)

#ifndef DS_H
#define DS_H

#include <string>
#include <vector> // range based queue return value

struct Node {
    Node(const std::string& n = std::string{})
        : name{n}, parent(nullptr), left(nullptr), right(nullptr) {}
    
    std::string name;
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
        std::vector<std::string> findrange(std::string& name1, std::string& name2, Node* n) const;
        bool insert(std::string& name);
        
        
        // functions for rb-tree & f-graph
        void print_all() const;
        
    private:
        Node* root;

        void yeet(Node* n); // recursive destructor helper
        bool insert(std::string& m, Node* n); // recursive insert helper
        Node* successor(std::string& name) const; // findrange helper
};

/*
class FG {

};

class PD {

};
*/
#endif
