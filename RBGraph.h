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
class RBNode
{
public:
    std::string name;
    bool color;
    i filePointer;
    RBNode *left;
    RBNode *right;
    RBNode *parent;

    // constructor
    RBNode(std::string n = std::string{}, bool c = red, i fp = 0, RBNode *l = nullptr, RBNode *r = nullptr, RBNode *p = nullptr)
        : name{n}, color{c}, filePointer{fp}, left{l}, right{r}, parent{p} {}

    // by default nodes are red; this is okay, because the 'null' nodes are considered black
};

/* IMPLEMENTATION OF FRIENDS LIST */
class LinkedListNode
{
public:
    std::string name;
    LinkedListNode *next;
    
    // constructor
    LinkedListNode(std::string n = std::string{}, LinkedListNode *lln = nullptr)
        : name{n}, next{lln} {}
};

/* IMPLEMENTATION OF FRIENDS LISTS */
class GraphNode
{
public:
    std::string name;
    LinkedListNode *list;
    i filePointer;
    
    GraphNode(std::string n = std::string{}, LinkedListNode *l = nullptr, i fp = 0)
        : name{n}, list{l}, filePointer{fp} {}
};

/* Red-Black Tree and Friendship Graph */
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
    RBNode *root;                   // RBT
    std::vector<GraphNode> graph;   // Graph
    i elements;                     // number of nodes

    // destructor helper
    void clear(RBNode *n);

    // print helper
    void print(RBNode *n);

    // find (exact or range), query helpers
    RBNode *find(const std::string &name, RBNode *n) const;
    std::vector<RBNode *> find_range(const std::string &name1, const std::string &name2) const;
    RBNode *successor(const std::string &name) const;

    // insert helper (BST portion)
    RBNode *binsert(RBNode *root, RBNode *entry);

    // insert helper (fix portion)
    void fix(RBNode *&r, RBNode *&entry);

    // rotations (fix helper)
    void lrotate(RBNode *&r, RBNode *&entry);
    void rrotate(RBNode *&r, RBNode *&entry);
};

#endif
