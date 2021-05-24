// ds.cpp (data structure implementation file)

#include "ds.h"

/* IMPLEMENTATION LOCATIONS
 * RBT() = :24
 * ~RBT() = :30
 * clear() = :34
 * exact_query() = :44
 * find() = :52
 * range_query() = :71
 * find_range() = :82
 * successor() = :97
 * print_all() = :139
 * print() = :145
 * insert() = :162
 * binsert() = :172
 * fix() = :191
 * lrotate() = :259
 * rrotate() = :310
 */

/* CONSTRUCTOR */
RBT::RBT() {
    root = nullptr;
    elements = 0;
}

/* DESTRUCTOR */
RBT::~RBT() {
    clear(root);
}

void RBT::clear(Node* n) {
    if (n) {
        clear(n->left);
        clear(n->right);
        delete n;
    }
    return;
}

/* FIND AND PRINT A NODE */
void RBT::exact_query(const std::string& name) const {
    Node* query = find(name, root);
    std::string occupation = "";
    std::cout << query->name << occupation << std::endl;
    return;
}

/* FIND A NODE */
Node* RBT::find(const std::string& name, Node* n) const {
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
    else { // tree empty or name not found
        return nullptr;
    }
}

/* PRINT A RANGE OF NODES IN LEXICOGRAPHICAL ORDER */
void RBT::range_query(const std::string& name1, const std::string& name2) const {
    std::vector<Node*> nodelist = find_range(name1, name2);
    std::string occupation;
    for (auto& person : nodelist) {
        occupation = ""; 
        std::cout << person->name << occupation << std::endl;
    }
    return;
}

/* FIND A RANGE OF NODES */
std::vector<Node*> RBT::find_range(const std::string& name1, const std::string& name2) const {
    std::vector<Node*> nodelist;
    nodelist.push_back(find(name1, root));
    Node* current = successor(name1);

    while (current->name != name2) {
        nodelist.push_back(current);
        current = successor(current->name);
    }

    nodelist.push_back(find(name2, root));
    return nodelist;
}

/* FIND THE NODE WITH NAME DIRECTLY SUCCEEDING THE GIVEN NAME */
Node* RBT::successor(const std::string& name) const {
    Node* current = find(name, root);
    if (current) {
        // if this node has a right subtree
        if (current->right) {
            current = current->right;
            while (current->left != nullptr) {
                current = current->left;
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
                    current = lastleft;
                    lastleft = lastleft->left;
                }
            }
        }
        // if the last left node passed is this node
        // this node has no successor
        if (current->name == name) {
            return nullptr;
        }
        else {
            return current;
        }
    }
    else {
        // root was nullptr OR
        // name is not in rb-tree
        return nullptr;
    }
}

/* PRINT ALL NODES (INORDER TRAVERSAL) */
void RBT::print_all() const {
    print(root);
    return;
}

/* RECURSIVE PRINT */
void RBT::print(Node* n) const {
    std::string colour;
    if (n) {
        print(n->left);
        if (n->color == red) {
            colour = "Red";
        }
        else {
            colour = "Black";
        }
        std::cout << "Name: " << n->name << ", Colour: " << colour << std::endl;
        print(n->right);
    }
    return;
}

/* INSERT NEW NODE, AND MAINTAIN RBT PROPERTIES */
void RBT::insert(const std::string& name) {
    if (!find(name, root)) { // insert only if the name is not in the tree
        Node* entry = new Node{name, red, elements++}; // increment the number of nodes after insert
        root = binsert(root, entry); // root == entry if tree is empty
        fix(root, entry); // after BST insert, fix the RBT
    }
    return;
}

/* CALLED BY INSERT: PERFORMS BST INSERT */
Node* RBT::binsert(Node* r, Node* entry) {
    if (!r) {
        return entry; // root was empty, so set new entry as root
    }

    if (entry->name < r->name) {
        r->left = binsert(r->left, entry);
        r->left->parent = r; // connect new left child with parent
    }
    else if (entry->name > r->name) {
        r->right = binsert(r->right, entry);
        r->right->parent = r; // connect new right child with parent
    }

    // if the tree is not empty, the root remains unchanged
    return r;
}

/* CALLED BY INSERT: MAINTAINS RBT PROPERTIES */
void RBT::fix(Node*& r, Node*& entry) {
    Node* pt_entry = nullptr; // entry's parent
    Node* uc_entry = nullptr; // entry's uncle
    Node* gpt_entry = nullptr; // entry's grandparent
    
    // fix() does nothing if:
        // 1. the entry is the root (fix only recolours the root black)
        // 2. the entry is already black
        // 3. the entry's parent is already black

    // thus the following loop runs while none of the above is true, i.e.
    // while entry is not root, entry is red, and entry's parent is red
    // entry != r must be checked first, since checking root's parent will segfault
    while ((entry != r) && (entry->color == red) && (entry->parent->color == red)) {
        pt_entry = entry->parent;
        gpt_entry = entry->parent->parent;
        
        // if entry's parent is left child of entry's grandparent
        if (pt_entry == gpt_entry->left) {
            Node* uc_entry = gpt_entry->right; // uncle is grandparent's other child 
            if (uc_entry && uc_entry->color == red) { // uncle exists and is also red
                pt_entry->color = black; // recolour parent and uncle
                uc_entry->color = black;
                gpt_entry->color = red; // recolour their parent (entry's grandparent)
                entry = gpt_entry; // check entry's grandparent
            }
            else { // uncle doesn't exist OR uncle is black
                // LR case (rotate left and make it a LL case)
                if (entry == pt_entry->right) {
                    lrotate(r, pt_entry);
                    entry = pt_entry;
                    pt_entry = entry->parent;
                }
                // LL case (rotate right)
                rrotate(r, gpt_entry);
                std::swap(pt_entry->color, gpt_entry->color);
                entry = pt_entry;
            }
        }
        // if entry's parent is right child of entry's grandparent
        else if (pt_entry == gpt_entry->right) {
            Node* uc_entry = gpt_entry->left; // uncle is grandparent's other child
            if (uc_entry && uc_entry->color == red) { // uncle exists and is also red
                pt_entry->color = black; // recolour parent and uncle
                uc_entry->color = black;
                gpt_entry->color = red; // recolour their parent (entry's grandparent)
                entry = gpt_entry; // check entry's grandparent
            }
            else { // uncle doesn't exist OR uncle is black
               // RL case (rotate right and make it a RR case)
                if (entry == pt_entry->left) {
                    rrotate(r, pt_entry);
                    entry = pt_entry;
                    pt_entry = entry->parent;
                }
                // RR case (rotate left)
                lrotate(r, gpt_entry);
                std::swap(pt_entry->color, gpt_entry->color);
                entry = pt_entry;
            }
        }
    }
    
    r->color = black; // set root to black if it isn't already
    return;
}

/* ROTATE LEFT, PIVOT = ENTRY */
void RBT::lrotate(Node*& r, Node*& entry) {
    // temporary pointer to entry's right child 
    Node* rc_entry = entry->right;

    // set entry's right child to be entry's right child's left child
    entry->right = rc_entry->left; 

    // if that old right child's left child exists
    if (entry->right) {
        // set entry to be that old right child's left child's parent
        entry->right->parent = entry;
    } 
    // entry's old right child's parent is entry's parent
    // i.e. entry's old right child and entry now share the same parent
    rc_entry->parent = entry->parent;

    // if their parent exists check whether entry is
    // its parent's left or right child
    if (entry->parent) {
        if (entry == entry->parent->left) {
            // if entry is a left child
            entry->parent->left = rc_entry;
            // set the entry's parent's left child 
            // to be parent's grandchild instead
            // (entry's old right child)
        }
        else {
            // if entry is a right child
            entry->parent->right = rc_entry;
            // set the entry's parent's right child 
            // to be parent's grandchild instead
            // (entry's old right child)
        }
    }
    // if their parent does not exist (their parent is a nullptr)
    // set the root to be entry's old right child
    else {
        r = rc_entry;
    }
    
    // set entry's old right child's new left child to be entry
    // instead of being its old left child (which is now entry's
    // right child)
    rc_entry->left = entry;
    // set entry's parent be entry's old right child
    entry->parent = rc_entry;

    return;
}

/* ROTATE RIGHT, PIVOT = ENTRY */
void RBT::rrotate(Node*& r, Node*& entry) {
    // temporary pointer to entry's left child 
    Node* lc_entry = entry->left;

    // set entry's left child to be entry's left child's right child
    entry->left = lc_entry->right;

    // if that old left child's right child exists
    if (entry->left) {
        // set entry to be that old left child's right child's parent
        entry->left->parent = entry;
    }
    // entry's old left child's parent is entry's parent
    // i.e. entry's old left child and entry now share the same parent
    lc_entry->parent = entry->parent;

    // if their parent exists check whether entry is
    // its parent's left or right child
    if (entry->parent) {
        if (entry == entry->parent->left) {
            // if entry is a left child
            entry->parent->left = lc_entry;
            // set the entry's parent's left child 
            // to be parent's grandchild instead
            // (entry's old left child)
        }
        else {
            // if entry is a right child
            entry->parent->right = lc_entry;
            // set the entry's parent's right child 
            // to be parent's grandchild instead
            // (entry's old right child)
        }
    }
    // if their parent does not exist (their parent is a nullptr)
    // set the root to be entry's old left child
    else {
        r = lc_entry;
    }

    // set entry's old right child's new left child to be entry
    // instead of being its old left child (which is now entry's
    // right child)
    lc_entry->right = entry;
    // set entry's parent be entry's old right child
    entry->parent = lc_entry;

    return;
}
