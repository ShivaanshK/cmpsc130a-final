#include <bits/stdc++.h>
#include <fstream>
#include <sstream>

using namespace std;
  
enum Color {RED, BLACK};
  
struct Node
{
    std::string data;
    bool color;
    Node *left, *right, *parent;
  
    // Constructor
    Node(std::string data)
    {
       this->data = data;
       left = right = parent = NULL;
       this->color = RED;
    }
};
  
// Class to represent Red-Black Tree
class RBTree
{
private:
    Node *root;
protected:
    void rotateLeft(Node *&, Node *&);
    void rotateRight(Node *&, Node *&);
    void fixViolation(Node *&, Node *&);
public:
    // Constructor
    RBTree() { root = NULL; }
    void insert(const std::string &n);
    void inorder();
    void levelOrder();
};
  
// A recursive function to do inorder traversal
void inorderHelper(Node *root)
{
    std::string colour;
    if (root == NULL)
        return;
  
    inorderHelper(root->left);
    if (root->color == RED) {
        colour = "Red";
    }
    else {
        colour = "Black";
    }
    cout << "Name: " << root->data << ", Colour: " << colour << std::endl;
    inorderHelper(root->right);
}
  
/* A utility function to insert 
    a new node with given key
   in BST */
Node* BSTInsert(Node* root, Node *pt)
{
    /* If the tree is empty, return a new node */
    if (root == NULL)
       return pt;
  
    /* Otherwise, recur down the tree */
    if (pt->data < root->data)
    {
        root->left  = BSTInsert(root->left, pt);
        root->left->parent = root;
    }
    else if (pt->data > root->data)
    {
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }

    /* return the (unchanged) node pointer */
    return root;
}
  
// Utility function to do level order traversal
void levelOrderHelper(Node *root)
{
    if (root == NULL)
        return;
  
    std::queue<Node *> q;
    q.push(root);
  
    while (!q.empty())
    {
        Node *temp = q.front();
        std::string colour = temp->color == RED ? "Red" : "Black";
        cout << "Name: " << temp->data << ", Colour: " << colour << endl;
        q.pop();
  
        if (temp->left != NULL)
            q.push(temp->left);
  
        if (temp->right != NULL)
            q.push(temp->right);
    }
}
  
void RBTree::rotateLeft(Node *&root, Node *&pt)
{
    Node *pt_right = pt->right;
  
    pt->right = pt_right->left;
  
    if (pt->right != NULL)
        pt->right->parent = pt;
  
    pt_right->parent = pt->parent;
  
    if (pt->parent == NULL)
        root = pt_right;
  
    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;
  
    else
        pt->parent->right = pt_right;
  
    pt_right->left = pt;
    pt->parent = pt_right;
}
  
void RBTree::rotateRight(Node *&root, Node *&pt)
{
    Node *pt_left = pt->left;
  
    pt->left = pt_left->right;
  
    if (pt->left != NULL)
        pt->left->parent = pt;
  
    pt_left->parent = pt->parent;
  
    if (pt->parent == NULL)
        root = pt_left;
  
    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;
  
    else
        pt->parent->right = pt_left;
  
    pt_left->right = pt;
    pt->parent = pt_left;
}
  
// This function fixes violations 
// caused by BST insertion
void RBTree::fixViolation(Node *&root, Node *&pt)
{
    Node *parent_pt = NULL;
    Node *grand_parent_pt = NULL;
  
    while ((pt != root) && (pt->color != BLACK) &&
           (pt->parent->color == RED))
    {
  
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;
  
        /*  Case : A
            Parent of pt is left child 
            of Grand-parent of pt */
        if (parent_pt == grand_parent_pt->left)
        {
  
            Node *uncle_pt = grand_parent_pt->right;
  
            /* Case : 1
               The uncle of pt is also red
               Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->color == 
                                                   RED)
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
  
            else
            {
                /* Case : 2
                   pt is right child of its parent
                   Left-rotation required */
                if (pt == parent_pt->right)
                {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
  
                /* Case : 3
                   pt is left child of its parent
                   Right-rotation required */
                rotateRight(root, grand_parent_pt);
                swap(parent_pt->color, 
                           grand_parent_pt->color);
                pt = parent_pt;
            }
        }
  
        /* Case : B
           Parent of pt is right child 
           of Grand-parent of pt */
        else
        {
            Node *uncle_pt = grand_parent_pt->left;
  
            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->color == 
                                                    RED))
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else
            {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (pt == parent_pt->left)
                {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
  
                /* Case : 3
                   pt is right child of its parent
                   Left-rotation required */
                rotateLeft(root, grand_parent_pt);
                swap(parent_pt->color, 
                         grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }
    root->color = BLACK; // set root to black if it isn't already
}
  
// Function to insert a new node with given data
void RBTree::insert(const std::string &data)
{
    Node *pt = new Node(data);
    // Do a normal BST insert
    root = BSTInsert(root, pt);
  
    // fix Red Black Tree violations
    fixViolation(root, pt);
}
  
// Function to do inorder and level order traversals
void RBTree::inorder()     {  inorderHelper(root);}
void RBTree::levelOrder()  {  levelOrderHelper(root); }
  
// Driver Code
int main()
{
    RBTree rb;
    std::string line;
    std::ifstream infile("bruh.txt");
    while (getline(infile, line)) {
        std::istringstream current_line(line);
        while (current_line) {
            std::string name;
            if (getline(current_line, name, ',')) {
                rb.insert(name);
            }
            else {
                break;
            }
        }
    }
    infile.close();



    cout << "Inoder Traversal of Created Tree\n";
    rb.inorder();
      
    return 0;
}
