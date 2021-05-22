// ds.cpp (data structure implementation file)

#include "ds.h"

RBFG::RBFG() {
    root = nullptr;
}

RBFG::~RBFG() {
    yeet(root);
    // vectors are freed when they go out of scope
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
    if (n) {
        if (n->name == name) {
            return n;
        }
        else {
            Node* child = find(name, n->left);
            if (!child) {
                child = find(name, n->right);
            }
            return child;
        }
    }
    else {
        return nullptr;
    }
}

// range-search
std::vector<Node*> RBFG::findrange(std::string& name1, std::string& name2) const {
    std::vector<Node*> nodelist;
    nodelist.push_back(find(name1, root));
    Node* traverse = successor(name1);

    while (traverse->name != name2) {
        nodelist.push_back(traverse);
        traverse = successor(traverse->name);
    }

    nodelist.push_back(find(name2, root));
    return nodelist;
}

// helper, private function
Node* RBFG::successor(std::string& name) const {
    Node* traverse = find(name, root);
    if (traverse) {
        // if this node has a right subtree
        if (traverse->right) {
            traverse = traverse->right;
            while (traverse->left != nullptr) {
                traverse = traverse->left;
            }
        }
        // if this node has no right subtree, 
        // starting from root to that node, the last left node
        // passed before arriving at the node is the successor
        else {
            Node* lastleft = root;
            while (lastleft->name != name) {
                if (lastleft->name < name) {
                    lastleft = lastleft->right;
                }
                else {
                    traverse = lastleft;
                    lastleft = lastleft->left;
                }
            }
        }
        // if the last left node passed is this node
        // this node has no successor
        if (traverse->name == name) {
            return nullptr;
        }
        else {
            return traverse;
        }
    }
    else {
        // root was nullptr OR
        // name is not in rb-tree
        return nullptr;
    }
}

// insert
bool RBFG::insert(std::string& name, i graph_index, i file_index) {
    if (root) {
        return insert(name, graph_index, file_index, root);
    }
    else {
        root = new Node(name, graph_index, file_index);
        return true;
    }
}

// helper, private function
bool RBFG::insert(std::string& name, i graph_index, i file_index, Node* n) {
    if (n->name == name) {
        return false;
    }
    else {
        if (n->name < name) {
            if (n->right) {
                return insert(name, graph_index, file_index, n->right);
            }
            else {
                n->right = new Node(name, graph_index, file_index);
                n->right->parent = n;
                return true;
            }
        }
        // already known that n->name != name
        // checks n->name > name
        else { 
            if (n->left) {
                return insert(name, graph_index, file_index, n->left);
            }
            else {
                n->left = new Node(name, graph_index, file_index);
                n->left->parent = n;
                return true;
            }
        }
    }
}

// functions for rb-tree & f-graph
// print name and occupation given range of names
void RBFG::range_query(std::string& name1, std::string& name2) const {
    std::vector<Node*> nodelist = findrange(name1, name2);
    std::string occupation;
    for (auto& person : nodelist) {
        // stub. read from file, the occupation. the index
        occupation = ""; // should be something like find(name1, root)->file_index
        std::cout << person->name << occupation << std::endl;
    }
    return;
}

// print all information for all users
void RBFG::print_all() const {
    std::cout << "print everything stub" << std::endl;
    return;
}
