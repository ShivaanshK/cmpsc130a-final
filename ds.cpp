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

void RBT::clear(RB_Node* n) {
    if (n) {
        clear(n->left);
        clear(n->right);
        delete n;
    }
    return;
}

/* FIND AND PRINT A RB_Node */
void RBT::exact_query(const std::string& name) const {
    RB_Node* query = find(name, root);
    std::string occupation = "";
    std::cout << query->name << occupation << std::endl;
    return;
}

/* FIND A RB_Node */
RB_Node* RBT::find(const std::string& name, RB_Node* n) const {
    if (n) {
        if (n->name == name) {
            return n;
        }
        else {
            RB_Node* child = find(name, n->left);
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

/* PRINT A RANGE OF RB_NodeS IN LEXICOGRAPHICAL ORDER */
void RBT::range_query(const std::string& name1, const std::string& name2) const {
    std::vector<RB_Node*> RB_Nodelist = find_range(name1, name2);
    std::string occupation;
    for (auto& person : RB_Nodelist) {
        occupation = ""; 
        std::cout << person->name << occupation << std::endl;
    }
    return;
}

/* FIND A RANGE OF RB_NodeS */
std::vector<RB_Node*> RBT::find_range(const std::string& name1, const std::string& name2) const {
    std::vector<RB_Node*> RB_Nodelist;
    RB_Nodelist.push_back(find(name1, root));
    RB_Node* current = successor(name1);

    while (current->name != name2) {
        RB_Nodelist.push_back(current);
        current = successor(current->name);
    }

    RB_Nodelist.push_back(find(name2, root));
    return RB_Nodelist;
}

/* FIND THE RB_Node WITH NAME DIRECTLY SUCCEEDING THE GIVEN NAME */
RB_Node* RBT::successor(const std::string& name) const {
    RB_Node* current = find(name, root);
    if (current) {
        // if this RB_Node has a right subtree
        if (current->right) {
            current = current->right;
            while (current->left != nullptr) {
                current = current->left;
            }
        }
        // if this RB_Node has no right subtree, 
        // starting from root to that RB_Node, the last left RB_Node
        // passed before arriving at the RB_Node is the successor
        else {
            RB_Node* lastleft = root;
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
        // if the last left RB_Node passed is this RB_Node
        // this RB_Node has no successor
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

/* PRINT ALL RB_NodeS (INORDER TRAVERSAL) */
void RBT::print_all() const {
    print(root);
    return;
}

/* RECURSIVE PRINT */
void RBT::print(RB_Node* n) const {
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

/* INSERT NEW RB_Node, AND MAINTAIN RBT PROPERTIES */
void RBT::insert(const std::string& name) {
    if (!find(name, root)) { // insert only if the name is not in the tree
        RB_Node* entry = new RB_Node{name, red, elements++}; // increment the number of RB_Nodes after insert
        root = binsert(root, entry); // root == entry if tree is empty
        fix(root, entry); // after BST insert, fix the RBT
    }
    return;
}

/* CALLED BY INSERT: PERFORMS BST INSERT */
RB_Node* RBT::binsert(RB_Node* r, RB_Node* entry) {
    if (!r) { // base case, insert new entry
        return entry; // root was empty, so set new entry as root
    }

    if (entry->name < r->name) { // traverse left
        r->left = binsert(r->left, entry);
        r->left->parent = r; // connect new left child with parent
    }
    else if (entry->name > r->name) { // traverse right
        r->right = binsert(r->right, entry);
        r->right->parent = r; // connect new right child with parent
    }

    // if the tree is not empty, the root remains unchanged
    return r;
}

/* CALLED BY INSERT: MAINTAINS RBT PROPERTIES */
void RBT::fix(RB_Node*& r, RB_Node*& entry) {
    RB_Node* pt_entry = nullptr; // entry's parent
    RB_Node* uc_entry = nullptr; // entry's uncle
    RB_Node* gpt_entry = nullptr; // entry's grandparent
    
    // the following loop runs until:
    // 1. entry is root
    // 2. entry is black OR
    // 3. entry's parent is black
    // i.e.
    // the following loop runs while:
    // 1. entry is not root
    // 2. entry is red AND
    // 3. entry's parent is red
    // entry != r must be checked first, since checking root's parent will segfault
    while ((entry != r) && (entry->color == red) && (entry->parent->color == red)) {
        // every loop assumes the entry has a parent and a grandparent
        pt_entry = entry->parent; // parent can be root
        gpt_entry = entry->parent->parent; // gparent can be nullptr

        // find entry's uncle
        if (pt_entry == gpt_entry->left) {
            uc_entry = gpt_entry->right;
        }
        else {
            uc_entry = gpt_entry->left;
        } // uncle can be nullptr

        // if uncle exists (then gparent exists)
        // and if uncle is red then 
        // (because parent is also red,
        // and by RBT invariant gparent is black)
        // switch parent/uncle and gparent colour
        if (uc_entry && uc_entry->color == red) {
            pt_entry->color = black;
            uc_entry->color = black;
            gpt_entry->color = red;
            entry = gpt_entry;
        }
        // uncle doesn't exist OR uncle is black
        else {
            // entry's parent is entry's grandparent's left child
            // entry is either LL or LR
            // LR degenerates to an LL case, so the following
            // if statement checks if it is LR, then does the LL case
            if (pt_entry == gpt_entry->left) {
                if (entry == pt_entry->right) {
                    // rotate left with pivot = parent
                    lrotate(r, pt_entry);
                    entry = pt_entry; // hi, im not sure why i needed this
                                      // but without it this DS has segfaults
                    pt_entry = entry->parent;
                }
                // rotate right with pivot = grandparent
                rrotate(r,gpt_entry);
            }
            // entry's parent is entry's grandparent's right child
            // entry is either RL or RR
            // RL degenerates to an RR case, so the following
            // if statement checks if it is RL, then does the RR case
            else if (pt_entry == gpt_entry->right) {
                if (entry == pt_entry->left) {
                    // rotate right with pivot = parent
                    rrotate(r, pt_entry);
                    entry = pt_entry;
                    pt_entry = entry->parent;
                }
                // rotate left with pivot = grandparent
                lrotate(r,gpt_entry);
            }
            // swapping parent and grandparent colours is always done
            std::swap(pt_entry->color, gpt_entry->color);
            // check the entry's parent for the next loop
            entry = pt_entry;
        }
    }
           
    r->color = black; // set root to black if it isn't already
    return;
}

/* ROTATE LEFT, PIVOT = ENTRY */
void RBT::lrotate(RB_Node*& r, RB_Node*& entry) {
    // temporary pointer to entry's right child 
    RB_Node* rc_entry = entry->right;

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
void RBT::rrotate(RB_Node*& r, RB_Node*& entry) {
    // temporary pointer to entry's left child 
    RB_Node* lc_entry = entry->left;

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
