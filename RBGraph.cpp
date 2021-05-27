#include "RBGraph.h"

/*
 * RBGraph() 
 * ~RBGraph()
 * clear()
 * insert()
 * read_file()
 * write_file()
 * ginsert()
 * ginsert_friend()
 * print_graph()
 * friendship_query()
 *
 *
 *
 */

/* CONSTRUCTOR */
RBGraph::RBGraph()
{
    root = nullptr;
    elements = 0;
}

/* DESTRUCTOR */
RBGraph::~RBGraph()
{
    if (root != nullptr)
        clear(root);
    graph.clear();
}

/* DESTRUCTOR HELPER */
void RBGraph::clear(rbnode *n)
{
    if (n)
    {
        clear(n->left);
        clear(n->right);
        delete n;
    }
    return;
}

/* RBT and GRAPH INSERT */
void RBGraph::insert(const std::string &s)
{
    std::string word("");
    std::string name, age, occupation;
    std::vector<std::string> friends;
    int counter = 0;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == ',' || (i == s.length() - 1))
        {
            if (i == s.length() - 1)
            {
                if (s[i] != '"')
                    word += s[i];
            }
            if (counter == 0)
                name = word; // store name portion of getline
            else if (counter == 1)
                age = word; // store age portion of getline
            else if (counter == 2)
                occupation = word; // store occupation portion of getline
            else if (word != "")
                friends.push_back(word); // store friends delineated by ','
            counter++;
            word = "";
        }
        else
        {
            if (s[i] != '"' && s[i] != ',')
            {
                word += s[i];
            }
        }
    }
    if (!find(name, root))  // insert only if the name is not in the tree
    {
        rbnode *entry = new rbnode{name, red, elements}; 
        root = binsert(root, entry);     // root == entry if tree is empty
        fix(root, entry); // fix tree
        write_file(name, age, occupation); // insert entry to file
        graphnode gentry(name, friends); // make a graph node
        graph.push_back(gentry); // push to the graph
        elements++; // after insert, increment # of nodes

        // here, the 'pointer' stored in the RBT corresponds
        // to both the file index and the graph index
    }

    return;
}

/* READ FROM THE FILE */
std::vector<std::string> RBGraph::read_file(i &filePointer) const
{
    char *buffer = new char[53];

    std::ifstream input("directory.txt", std::ios::binary);
    input.seekg(filePointer * 53, std::ios::beg);
    input.read(buffer, 53);
    input.close();
    input.clear();
    
    // make a vector of chars
    std::vector<char> preparse;
    for (size_t i = 0; i < 53; ++i) {
        preparse.push_back(buffer[i]);
    }
    delete [] buffer;

    std::ostringstream output;
    for (size_t i = 0; i < 20; ++i) {
        output << preparse[i];
    }
    std::string name(output.str());
    output.str(std::string());
    for (size_t i = 20; i < 23; ++i) {
        output << preparse[i];
    }
    std::string age(output.str());
    output.str(std::string());
    for (size_t i = 23; i < 53; ++i) {
        output << preparse[i];
    }
    std::string occupation(output.str());
    output.str(std::string());

    std::string n(""), a(""), o("");
    

    int k = 0;
    for (size_t i = 0; i < 20; i++)
    {
        if (name[i] == ' ')
            k++;
        else
            k = 0;
        if (k == 2)
        {
            n = n.substr(0, n.length() - 1);
            break;
        }
        n += name[i];
    }
    for (size_t i = 0; i < 3; i++)
    {
        if (age[i] == ' ')
        {
            break;
        }
        a += age[i];
    }
    k = 0;
    for (size_t i = 0; i < 30; i++)
    {
        if (occupation[i] == ' ')
            k++;
        else
            k = 0;
        if (k == 2)
        {
            o = o.substr(0, o.length() - 1);
            break;
        }
        o += occupation[i];
    }
    std::vector<std::string> s;
    s.push_back(n); // 1st element of return value is name
    s.push_back(a); // 2nd is age
    s.push_back(o); // 3rd is occupation

    return s;
}

/* INSERT TO THE FILE */
void RBGraph::write_file(const std::string &name, const std::string &age, const std::string &occupation) const
{
    std::ofstream output("directory.txt", std::ios::app);
    output << std::setfill(' ') << std::left << std::setw(20) << name << std::setw(3) << age << std::setw(30) << occupation;
    output.close();
}

/* INSERT NEW PERSON VIA THE GRAPH
 * (first inserts to the RB-Tree, to store file/graph index)
 * (then writes to the file, then makes a new graphnode) */
void RBGraph::ginsert(const std::string &name, const std::string &age, const std::string &occupation)
{
    if (!find(name,root)) 
    {
        rbnode *entry = new rbnode{name, red, elements};
        root = binsert(root, entry);
        fix(root, entry);
        write_file(name, age, occupation);

        // for a insert-by-graph, new user has no friends
        std::vector<std::string> friends;
        graphnode gentry(name,friends);
        graph.push_back(gentry);
        elements++;
    }
    return;
}

/* INSERT A FRIEND TO AN EXISTING PERSON IN THE GRAPH 
 * (adding a friend goes both ways, so both graphnodes must be updated) */
void RBGraph::ginsert_friend(const std::string &p1, const std::string &p2)
{
    rbnode *p1_node = find(p1,root);
    rbnode *p2_node = find(p2,root);
    if (p1_node && p2_node) // terminates if one or both persons are not in the network
    {
        // check if p2 is already p1's friend
        // the friend list is the graphnode's second element
        i i_p1 = p1_node->pointer;
        i i_p2 = p2_node->pointer;
        for (auto gnit = graph[i_p1].second.begin(); gnit != graph[i_p1].second.end(); ++gnit) {
            // if p2 is already in p1, then p1 is also already in p2
            if (*(gnit) == p2) {
                return;
            }
        }
        graph[i_p1].second.push_back(p2);
        graph[i_p2].second.push_back(p1);
    }
    return;
}

/* PRINT THE ENTIRE FRIENDSHIP-GRAPH */
void RBGraph::print_graph() const
{
    std::cout << "Graph:" << std::endl;
    i gindex = 0;
    while(gindex < elements) {
        for (auto& gnode : graph) {
            std::cout << gindex << ": " << gnode.first; // first = username
            for (auto& friendname : gnode.second) { // second = friend list
                std::cout << " --> " << friendname;
            }
            std::cout << "\n";
            ++gindex;
        }
        std::cout << "\n\n";
    }
    return;
}

//RB Tree Functions:

/* FRIENDSHIP QUERY
 * (traverse the rb-tree to find the person of interest)
 * (then, get that person's friendslist from the graph)
 * (then, print the name and occupation of each friend) */
void RBGraph::friendship_query(const std::string &name) const
{
    rbnode *query = find(name, root); // temp node for person and their friends
    if (query)
    {
        i i_person = query->pointer;
        std::vector<std::string> nao = read_file(i_person); // temp var for printing name/occupation
        std::cout << "Name: " << nao[0] << std::endl;
        std::cout << "Occupation: " << nao[2] << std::endl;
        std::cout << "------------------------------------------\n";
        std::cout << nao[0] << "'s friends:\n\n";
    
        // get friend list and start printing each friend's name and occupation
        for (auto& friendname : graph[i_person].second) {
            query = find(friendname, root); // find friend
            nao = read_file(query->pointer); // read friend's info from file
            std::cout << "Name: " << nao[0] << std::endl;
            std::cout << "Occupation: " << nao[2] << std::endl;
        }
        std::cout << "------------------------------------------\n";
        std::cout << "------------------------------------------\n\n";
    }
    return;
}

/* FIND AN RB-NODE, BST SEARCH */
rbnode *RBGraph::find(const std::string &name, rbnode *n) const
{
    if (n)
    {
        if (n->name == name)
        {
            return n;
        }
        else
        {
            rbnode *child = find(name, n->left);
            if (!child)
            {
                child = find(name, n->right);
            }
            return child;
        }
    }
    else
    { // tree empty or name not found
        return nullptr;
    }
}

/* PRINT A RANGE OF RB-NODES */
void RBGraph::range_query(const std::string &name1, const std::string &name2) const
{
    std::vector<rbnode *> queries = find_range(name1, name2);
    i i_person = 0;
    std::vector<std::string> nao;
    for (auto& query : queries)
    {
        i_person = query->pointer;
        nao = read_file(i_person); // temp var for printing name/occupation
        std::cout << "Name: " << nao[0] << std::endl;
        std::cout << "Occupation: " << nao[2] << std::endl;
    }
    std::cout << "------------------------------------------\n";
    std::cout << "------------------------------------------\n\n";
    return;
}

/* FIND A RANGE OF RB-NODES */
std::vector<rbnode *> RBGraph::find_range(const std::string &name1, const std::string &name2) const
{
    std::vector<rbnode *> names;
    names.push_back(find(name1, root));
    rbnode *current = successor(name1);

    while (current->name != name2)
    {
        names.push_back(current);
        current = successor(current->name);
    }

    names.push_back(find(name2, root));

    return names;
}

/* FIND THE RB-NODE WITH NAME DIRECTLY SUCCEEDING THE GIVEN NAME */
rbnode *RBGraph::successor(const std::string &name) const
{
    rbnode *current = find(name, root);
    if (current)
    {
        // if this rbnode has a right subtree
        if (current->right)
        {
            current = current->right;
            while (current->left != nullptr)
            {
                current = current->left;
            }
        }
        // if this rbnode has no right subtree,
        // starting from root to that rbnode, the last left rbnode
        // passed before arriving at the rbnode is the successor
        else
        {
            rbnode *lastleft = root;
            while (lastleft->name != name)
            {
                if (lastleft->name < name)
                {
                    lastleft = lastleft->right;
                }
                else
                {
                    current = lastleft;
                    lastleft = lastleft->left;
                }
            }
        }
        // if the last left rbnode passed is this rbnode
        // this rbnode has no successor
        if (current->name == name)
        {
            return nullptr;
        }
        else
        {
            return current;
        }
    }
    else
    {
        // root was nullptr OR
        // name is not in rb-tree
        return nullptr;
    }
}

/* PRINT ALL rbnodeS (INORDER TRAVERSAL) */
void RBGraph::print_all() const
{
    print(root);
    std::cout << "------------------------------------------\n\n";
    return;
}

/* RECURSIVE PRINT */
void RBGraph::print(rbnode *node) const
{
    if (node)
    {
        print(node->left);

        std::vector<std::string> nao = read_file(node->pointer);
        std::cout << "Name: " << nao[0] << std::endl
                  << "Age: " << nao[1] << std::endl
                  << "Occupation: " << nao[2] << std::endl
                  << nao[0] << "'s Friends: " << std::endl;
        std::vector<std::string> names = graph[node->pointer].second; // friend list
        for (auto& name : names) {
            std::cout << "\t" << name << std::endl;
        }
        std::cout << "------------------------------------------\n";

        print(node->right);
    }
    return;
}

/* CALLED BY INSERT: PERFORMS BST INSERT */
rbnode *RBGraph::binsert(rbnode *r, rbnode *entry)
{
    if (!r)
    {                 // base case, insert new entry
        return entry; // root was empty, so set new entry as root
    }

    if (entry->name < r->name)
    { // traverse left
        r->left = binsert(r->left, entry);
        r->left->parent = r; // connect new left child with parent
    }
    else if (entry->name > r->name)
    { // traverse right
        r->right = binsert(r->right, entry);
        r->right->parent = r; // connect new right child with parent
    }

    // if the tree is not empty, the root remains unchanged
    return r;
}

/* CALLED BY INSERT: MAINTAINS RBGraph PROPERTIES */
void RBGraph::fix(rbnode *&r, rbnode *&entry)
{
    rbnode *pt_entry = nullptr;  // entry's parent
    rbnode *uc_entry = nullptr;  // entry's uncle
    rbnode *gpt_entry = nullptr; // entry's grandparent

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
    while ((entry != r) && (entry->color == red) && (entry->parent->color == red))
    {
        // every loop assumes the entry has a parent and a grandparent
        pt_entry = entry->parent;          // parent can be root
        gpt_entry = entry->parent->parent; // gparent can be nullptr

        // find entry's uncle
        if (pt_entry == gpt_entry->left)
        {
            uc_entry = gpt_entry->right;
        }
        else
        {
            uc_entry = gpt_entry->left;
        } // uncle can be nullptr

        // if uncle exists (then gparent exists)
        // and if uncle is red then
        // (because parent is also red,
        // and by RBGraph invariant gparent is black)
        // switch parent/uncle and gparent colour
        if (uc_entry && uc_entry->color == red)
        {
            pt_entry->color = black;
            uc_entry->color = black;
            gpt_entry->color = red;
            entry = gpt_entry;
        }
        // uncle doesn't exist OR uncle is black
        else
        {
            // entry's parent is entry's grandparent's left child
            // entry is either LL or LR
            // LR degenerates to an LL case, so the following
            // if statement checks if it is LR, then does the LL case
            if (pt_entry == gpt_entry->left)
            {
                if (entry == pt_entry->right)
                {
                    // rotate left with pivot = parent
                    lrotate(r, pt_entry);
                    entry = pt_entry; // hi, im not sure why i needed this
                                      // but without it this DS has segfaults
                    pt_entry = entry->parent;
                }
                // rotate right with pivot = grandparent
                rrotate(r, gpt_entry);
            }
            // entry's parent is entry's grandparent's right child
            // entry is either RL or RR
            // RL degenerates to an RR case, so the following
            // if statement checks if it is RL, then does the RR case
            else if (pt_entry == gpt_entry->right)
            {
                if (entry == pt_entry->left)
                {
                    // rotate right with pivot = parent
                    rrotate(r, pt_entry);
                    entry = pt_entry;
                    pt_entry = entry->parent;
                }
                // rotate left with pivot = grandparent
                lrotate(r, gpt_entry);
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
void RBGraph::lrotate(rbnode *&r, rbnode *&entry)
{
    // 1) temporary pointer to entry's right child
    rbnode *rc_entry = entry->right;

    // 2) set entry's right child to be entry's right child's left child
    entry->right = rc_entry->left;

    // 2a) if that old right child's left child exists
    if (rc_entry->left)
    {
        // set entry's old right child's left child's parent be entry
        entry->right->parent = entry;
    }

    // entry's old right child's parent is entry's parent
    // 3) i.e. entry's old right child and entry now share the same parent
    rc_entry->parent = entry->parent;

    // 4a) if their parent exists check whether entry is
    // its parent's left or right child
    if (entry->parent)
    {
        if (entry == entry->parent->left)
        {
            // 4aa) if entry is a left child
            entry->parent->left = rc_entry;
            // set the entry's parent's left child
            // to be parent's grandchild instead
            // (entry's old right child)
        }
        else
        {
            // 4ab) if entry is a right child
            entry->parent->right = rc_entry;
            // set the entry's parent's right child
            // to be parent's grandchild instead
            // (entry's old right child)
        }
    }
    // 4b) if their parent does not exist (their parent is a nullptr)
    // set the root to be entry's old right child
    else if (!entry->parent)
    {
        r = rc_entry;
    }

    // 5) set entry's old right child's new left child to be entry
    // instead of being its old left child (which is now entry's
    // right child)
    rc_entry->left = entry;
    // 6) set entry's parent be entry's old right child
    entry->parent = rc_entry;

    return;
}

/* ROTATE RIGHT, PIVOT = ENTRY */
void RBGraph::rrotate(rbnode *&r, rbnode *&entry)
{
    // 1) temporary pointer to entry's left child
    rbnode *lc_entry = entry->left;

    // 2) set entry's left child to be entry's left child's right child
    entry->left = lc_entry->right;

    // 2a) if that old left child's right child exists
    if (lc_entry->right)
    {
        // set entry's old left child's right child's parent be entry
        entry->left->parent = entry;
    }
    // 3) entry's old left child's parent is entry's parent
    // i.e. entry's old left child and entry now share the same parent
    lc_entry->parent = entry->parent;

    // 4a) if their parent exists check whether entry is
    // its parent's left or right child
    if (entry->parent)
    {
        if (entry == entry->parent->left)
        {
            // 4aa) if entry is a left child
            entry->parent->left = lc_entry;
            // set the entry's parent's left child
            // to be parent's grandchild instead
            // (entry's old left child)
        }
        else
        {
            // 4ab) if entry is a right child
            entry->parent->right = lc_entry;
            // set the entry's parent's right child
            // to be parent's grandchild instead
            // (entry's old right child)
        }
    }
    // 4b) if their parent does not exist (their parent is a nullptr)
    // set the root to be entry's old left child
    else if (!entry->parent)
    {
        r = lc_entry;
    }

    // 5) set entry's old right child's new left child to be entry
    // instead of being its old left child (which is now entry's
    // right child)
    lc_entry->right = entry;
    // 6) set entry's parent be entry's old right child
    entry->parent = lc_entry;

    return;
}

