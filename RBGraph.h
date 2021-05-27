#ifndef RBGRAPH_H
#define RBGRAPH_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream> // for std::ostringstream in read_file()
#include <utility> // for std::swap in fix(), std::pair for graphnode
#include <iomanip> // setw(), setfill(), in write_file()
#include <fstream>

typedef uint32_t i; // graph and file indices

/* IMPLEMENTATION OF A GRAPH NODE
 * A BUNCH OF THESE MAKE UP THE FRIENDSHIP GRAPH */
typedef std::pair<std::string,std::vector<std::string>> graphnode;

// colour of rb-tree nodes
const static bool red = true;
const static bool black = false;

/* IMPLEMENTATION OF A RED-BLACK NODE 
 * A BUNCH OF THESE MAKE UP THE RB-TREE */
class rbnode
{
public:
    std::string name;
    bool color;
    i pointer;
    rbnode *left;
    rbnode *right;
    rbnode *parent;
    // the pointer corresponds to index of both file disk and graph
    // this is because the rb-node data is inserted to the file and
    // the graph at the same instance

    // constructor
    rbnode(std::string n = std::string{}, bool c = red, i gfp = 0, rbnode *l = nullptr, rbnode *r = nullptr, rbnode *p = nullptr)
        : name{n}, color{c}, pointer{gfp}, left{l}, right{r}, parent{p} {}

    // by default nodes are red; this is okay, because the 'null' nodes are considered black
};

/* Red-Black Tree and Friendship Graph */
class RBGraph
{
public:
    // constructor
    RBGraph();
    // destructor
    ~RBGraph();

    // insert to RB-Tree and F-Graph
    void insert(const std::string &s);

    // query (friends, range, or all)
    void friendship_query(const std::string &name) const;
    void range_query(const std::string &name1, const std::string &name2) const;
    void print_all() const;

    // manipulate graph
    void ginsert(const std::string &name, const std::string &age, const std::string &occupation);
    void ginsert_friend(const std::string &p1, const std::string &p2);
    void print_graph() const;

    // read/write to file
    std::vector<std::string> read_file(i &filePointer) const;
    void write_file(const std::string &name, const std::string &age, const std::string &occupation) const;


private:
    rbnode *root;                   // RBT
    std::vector<graphnode> graph;   // Graph
    i elements;                     // number of nodes, file index, graph index

    // destructor helper
    void clear(rbnode *n);

    // print helper
    void print(rbnode *n) const;

    // find (exact or range), query helpers
    rbnode *find(const std::string &name, rbnode *n) const;
    std::vector<rbnode *> find_range(const std::string &name1, const std::string &name2) const;
    rbnode *successor(const std::string &name) const;

    // insert helper (BST portion)
    rbnode *binsert(rbnode *root, rbnode *entry);

    // insert helper (fix portion)
    void fix(rbnode *&r, rbnode *&entry);

    // rotations (fix helper)
    void lrotate(rbnode *&r, rbnode *&entry);
    void rrotate(rbnode *&r, rbnode *&entry);
};

#endif
