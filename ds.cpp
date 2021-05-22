// ds.cpp (data structure implementation file)

#include "ds.h"

RBFG::RBFG() {
    nullnode = new Node;
    nullnode->left = nullnode;
    nullnode->right = nullnode;
    root = new Node{"sentinel, right child is root"};
    root->left = nullnode;
    root->right = nullnode;
    elements = 0;
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
        return nullnode;
    }
}

// range-search
std::vector<Node*> RBFG::findrange(std::string& name1, std::string& name2) const {
    std::vector<Node*> nodelist;
    nodelist.push_back(find(name1, root->right));
    Node* current = successor(name1);

    while (current->name != name2) {
        nodelist.push_back(current);
        current = successor(current->name);
    }

    nodelist.push_back(find(name2, root->right));
    return nodelist;
}

// helper, private function
Node* RBFG::successor(std::string& name) const {
    Node* current = find(name, root->right);
    if (current) {
        // if this node has a right subtree
        if (current->right) {
            current = current->right;
            while (current->left != nullnode) {
                current = current->left;
            }
        }
        // if this node has no right subtree, 
        // starting from root to that node, the last left node
        // passed before arriving at the node is the successor
        else {
            Node* lastleft = root->right;
            while (lastleft->name != name) {
                if (lastleft->name < name) {
                    lastleft = lastleft->right;
                }
                else {
                    current = lastleft;
                    lastleft = lastleft->left;
                }
            }
        }
        // if the last left node passed is this node
        // this node has no successor
        if (current->name == name) {
            return nullnode;
        }
        else {
            return current;
        }
    }
    else {
        // root was nullptr OR
        // name is not in rb-tree
        return nullnode;
    }
}

// insert
void RBFG::insert(std::string& name) {
   traverse = root;
   pt = root; 
   gpt = root;
   nullnode->name = name;

   while(traverse->name != name) {
       ggpt = gpt;
       gpt = pt;
       pt = traverse;
       if (name < traverse->name) {
           traverse = traverse->left;
       }
       else {
           traverse = traverse->right;
       }
        
       // if the current node has two red children
       // swap colours / do rotations
       if ((traverse->left->color == red) && (traverse->left->color == red)) {
           fix(name);
       }
   }

   // this RB-tree assumes all elements are unique
   // if the newly-inserted name is not at the leaf
   // it was not unique, and this function is done
   if (traverse != nullnode) {
       return;
   }
   else {
       traverse = new Node{name, elements++, nullnode, nullnode};
       if (name < pt->name) {
           pt->left = traverse;
       }
       else {
           pt->right = traverse;
       }
       fix(name); // fix RB-tree
   }

}

// helper, private function
// during insert(), if node has two red children, fix() is called
void RBFG::fix(std::string& name) {
    // the 'traverse' node below points to the 
    // node that is being inserted (as fix is
    // called within the insert function)
    traverse->color = red;
    traverse->left->color = black;
    traverse->right->color = black;
    
    // the uncle is always black thanks to top-down implementation
    // thus, if the parent of traverse is red, there will be two rotations
    if (pt->color == red) {
        gpt->color = red;
        if ((name < gpt->name) != (name < pt->name)) {
            pt = rotate(name, gpt);
        }
        traverse = rotate(name, ggpt);
        traverse->color = black;
    }
    
    // for this implementation, the 
    // real root is root->right
    // the real root needs to become black
    root->right->color = black;
}

// helper, private function
// called by fix(), which is a subroutine of insert()
// this_pt is the parent of the subtree being rotated
Node* RBFG::rotate(std::string& name, Node* this_pt) {
    if (name < this_pt->name) {
        if (name < this_pt->left->name) {
            // LL case
            rrotate(this_pt->left);
        }
        else {
            // LR case
            lrotate(this_pt->left);
        }
        return this_pt->left;
    }
    else {
        if (name < this_pt->right->name) {
            // RL case
            rrotate(this_pt->right);
        }
        else {
            // RR case
            lrotate(this_pt->left);
        }
        return this_pt->right;
    }
}

// helper, private function
void RBFG::lrotate(Node*& n) { // rotate left
    pt = n->right;
    n->right = pt->left;
    pt->left = n;
    n = pt;
}

// helper, private function
void RBFG::rrotate(Node*& n) { // rotate right
    pt = n->left;
    n->left = pt->right;
    pt->right = n;
    n = pt;
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
void RBFG::print_all(Node* n) const {
    std::cout << "print everything stub" << std::endl;
    return;
}
