// ds.cpp (data structure implementation file)

#include "ds.h"

RBFG::RBFG() {
    root = nullptr;
}

RBFG::~RBFG() {
    yeet(root);
}

// helper, private function
void RBFG::yeet(Node* n) {
    if (n) {
        yeet(n->left);
        yeet(n->right);
        delete n;
    }
    else {
        return;
    }
}

// functions for rb-tree
// exact-match
Node* RBFG::find(std::string& name, Node* n) const {
    return nullptr;
}

// range-search
std::vector<std::string> RBFG::findrange(std::string& name1, std::string& name2, Node* n) const {
    std::vector<std::string> bruh;
    return bruh;
}

// helper, private function
Node* RBFG::successor(std::string& name) const {
    return nullptr;
}

// insert
bool RBFG::insert(std::string& name) {
    return true;
}

// helper, private function
bool RBFG::insert(std::string& m, Node* n) {
    return false;
}

// functions for rb-tree & f-graph
// print all information for all users
void RBFG::print_all() const {
    return;
}

