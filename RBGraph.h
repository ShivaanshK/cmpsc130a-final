#ifndef RBGRAPH_H
#define RBGRAPH_H
#include <iostream>
#include <string>
#include <vector>
#include <utility> // for std::swap in fix()
#include <iomanip>
#include <fstream>

typedef uint32_t i; // indices are never negative
const static bool red = true;
const static bool black = false;

/* IMPLEMENTATION OF RED-BLACK NODE */
class RB_Node
{
public:
    std::string name;
    bool color;
    i filePointer;
    RB_Node *left;
    RB_Node *right;
    RB_Node *parent;

    // constructor
    RB_Node(std::string n = std::string{}, bool c = red, i fp = 0, RB_Node *l = nullptr, RB_Node *r = nullptr, RB_Node *p = nullptr)
        : name{n}, color{c}, filePointer{fp}, left{l}, right{r}, parent{p} {}

    // by default nodes are red; this is okay, because the 'null' nodes are considered black
};

//Implementation of Node in linked list of graph
struct Linked_List_Node
{
    Linked_List_Node() { next = nullptr; };
    std::string name;
    Linked_List_Node *next;
};

//Implementation of Node in Graph
struct Graphnode
{
    Graphnode() { list = nullptr; };
    std::string name;
    Linked_List_Node *list;
    i filePointer;
};

//Class which implements a RB Tree and a Graph
class RBGraph
{
public:
    // constructor
    RBGraph();
    // destructor
    ~RBGraph();

    // query (exact, range, or all)
    void friendship_query(const std::string &name);
    void range_query(const std::string &name1, const std::string &name2);
    void print_all();

    // manipulate graph
    void insert_user(const std::string &name, const std::string &age, const std::string &occupation);
    void insert_friendship(const std::string &p1, const std::string &p2);
    void print_Graph() const;

    //read/write to file
    void write_to_file(const std::string &name, const std::string &age, const std::string &occupation) const;
    std::vector<std::string> read_file(i &filePointer);

    // insert
    void insert(const std::string &s);

private:
    RB_Node *root;                // RBT
    std::vector<Graphnode> graph; // Graph

    i elements; // number of nodes

    // destructor helper
    void clear(RB_Node *n);

    // print helper
    void print(RB_Node *n);

    // find (exact or range), query helpers
    RB_Node *find(const std::string &name, RB_Node *n) const;
    std::vector<RB_Node *> find_range(const std::string &name1, const std::string &name2) const;
    RB_Node *successor(const std::string &name) const;

    // insert helper (BST portion)
    RB_Node *binsert(RB_Node *root, RB_Node *entry);

    // insert helper (fix portion)
    void fix(RB_Node *&r, RB_Node *&entry);

    // rotations (fix helper)
    void lrotate(RB_Node *&r, RB_Node *&entry);
    void rrotate(RB_Node *&r, RB_Node *&entry);
};

#endif