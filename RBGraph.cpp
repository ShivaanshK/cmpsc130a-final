#include "RBGraph.h"

/* VIM NAVIGATION PLEASE DON'T DELT MERCI BEAUCOUP
 * RBGraph() :30
 * ~RBGraph() :37
 * clear() :45
 * exact_query() :58
 * friendship_query() :80
 * range_query() :109
 * print_all() :122
 * print() :130
 * print_graph() :154
 * insert() :176
 * ginsert() :230
 * ginsert_friend() :252
 * read_file() :289
 * write_file() :374
 * 
 * find() :388
 * rfind() :414
 * find_range() :471
 * successor() :503
 * predecessor() :556
 * binsert() :609
 * fix() :632
 * lrotate() :724
 * rrotate() :782 */

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
    //  graph.clear(); // not really necessary as vector clears itself
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

/* EXACT QUERY
 * returns name's data (but not friends data) */
void RBGraph::exact_query(const std::string &name) const
{
    rbnode *query = find(name, root); // temp node for person and their friends
    if (query)
    {
        std::vector<std::string> nao = read_file(query->pointer); // temp var for printing name/occupation
        std::cout << "Name: " << nao[0] << std::endl;
        std::cout << "Age: " << nao[1] << std::endl;
        std::cout << "Occupation: " << nao[2] << std::endl;
        std::cout << "------------------------------------------\n\n";
    }
    else
    {
        std::cout << "User Not Found." << std::endl;
    }
    return;
}

/* FRIENDSHIP QUERY
 * (traverse the rb-tree to find the person of interest)
 * (then, get that person's friendslist from the graph)
 * (then, print the name and occupation of each friend) */
void RBGraph::friendship_query(const std::string &name) const
{
    rbnode *query = find(name, root); // temp node for person and their friends
    if (query)
    {
        // query the person's information
        exact_query(name);
        std::cout << name << "'s friends:\n\n";

        // go to the friendship graph and find the person
        i i_person = query->pointer;
        // iterate thru each friend in the person's friend list and
        // query each friend's information
        for (auto &friendname : graph[i_person].second)
        {
            exact_query(friendname);
        }
        std::cout << "------------------------------------------\n";
        std::cout << "------------------------------------------\n\n";
    }
    else
    {
        std::cout << "User Not Found." << std::endl
                  << std::endl;
    }
    return;
}

/* PRINT A RANGE OF RB-NODES */
void RBGraph::range_query(const std::string &name1, const std::string &name2) const
{
    std::vector<rbnode *> queries = find_range((name1 < name2) ? name1 : name2, (name1 > name2) ? name1 : name2);
    for (auto &query : queries)
    {
        exact_query(query->name);
    }
    std::cout << "------------------------------------------\n";
    std::cout << "------------------------------------------\n\n";
    return;
}

/* PRINT ALL RB-NODES IN ASCENDING ORDER */
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
        for (auto &name : names)
        {
            std::cout << "\t" << name << std::endl;
        }
        std::cout << "------------------------------------------\n";

        print(node->right);
    }
    return;
}

/* PRINT THE ENTIRE FRIENDSHIP-GRAPH */
void RBGraph::print_graph() const
{
    std::cout << "Graph:" << std::endl;
    i gindex = 0;
    while (gindex < elements)
    {
        for (auto &gnode : graph)
        {
            std::cout << gindex << ": " << gnode.first; // first = username
            for (auto &friendname : gnode.second)
            { // second = friend list
                std::cout << " --> " << friendname;
            }
            std::cout << "\n";
            ++gindex;
        }
        std::cout << "\n\n";
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
    if (!find(name, root)) // insert only if the name is not in the tree
    {
        rbnode *entry = new rbnode{name, red, elements};
        root = binsert(root, entry);       // root == entry if tree is empty
        fix(root, entry);                  // fix tree
        write_file(name, age, occupation); // insert entry to file
        graphnode gentry(name, friends);   // make a graph node
        graph.push_back(gentry);           // push to the graph
        elements++;                        // after insert, increment # of nodes

        // here, the 'pointer' stored in the RBT corresponds
        // to both the file index and the graph index
    }

    return;
}

/* INSERT NEW PERSON VIA THE GRAPH
 * (first inserts to the RB-Tree, to store file/graph index)
 * (then writes to the file, then makes a new graphnode) */
void RBGraph::ginsert(const std::string &name, const std::string &age, const std::string &occupation)
{
    if (!find(name, root))
    {
        rbnode *entry = new rbnode{name, red, elements};
        root = binsert(root, entry);
        fix(root, entry);
        write_file(name, age, occupation);

        // for a insert-by-graph, new user has no friends
        std::vector<std::string> friends;
        graphnode gentry(name, friends);
        graph.push_back(gentry);
        elements++;

        std::cout << "Successfully inserted " << name << std::endl << std::endl;
    }
    return;
}

/* INSERT A FRIEND TO AN EXISTING PERSON IN THE GRAPH 
 * (adding a friend goes both ways, so both graphnodes must be updated) */
void RBGraph::ginsert_friend(const std::string &p1, const std::string &p2)
{
    rbnode *p1_node = find(p1, root);
    rbnode *p2_node = find(p2, root);
    if (p1_node && p2_node) // terminates if one or both persons are not in the network
    {
        // check if p2 is already p1's friend
        // the friend list is the graphnode's second element
        i i_p1 = p1_node->pointer;
        i i_p2 = p2_node->pointer;
        for (auto gnit = graph[i_p1].second.begin(); gnit != graph[i_p1].second.end(); ++gnit)
        {
            // if p2 is already in p1, then p1 is also already in p2
            if (*(gnit) == p2)
            {
                return;
            }
        }
        graph[i_p1].second.push_back(p2);
        graph[i_p2].second.push_back(p1);

        std::cout << "Successfully inserted friendship between " << p1 << " and " << p2 << std::endl;
    }

    if (!p1_node)
    {
        std::cout << p1 << " is not in the network." << std::endl;
    }
    if (!p2_node)
    {
        std::cout << p2 << " is not in the network." << std::endl;
    }
    std::cout << std::endl;
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
    for (size_t i = 0; i < 53; ++i)
    {
        preparse.push_back(buffer[i]);
    }
    delete[] buffer;

    std::ostringstream output;
    for (size_t i = 0; i < 20; ++i)
    {
        output << preparse[i];
    }
    std::string name(output.str());
    output.str(std::string());
    for (size_t i = 20; i < 23; ++i)
    {
        output << preparse[i];
    }
    std::string age(output.str());
    output.str(std::string());
    for (size_t i = 23; i < 53; ++i)
    {
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

/*
 * ------------------------------
 * PRIVATE FUNCTIONS
 * ------------------------------
 */

/* RECURSIVE FIND AN RB-NODE, BST SEARCH */
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

/* ITERATIVE FIND FOR RANGE-BASED RB-NODE, BST SEARCH 
 * (called for range query, considers case when name1 or name2 is not in the RBT) */
std::pair<rbnode *, rbnode *> RBGraph::rfind(const std::string &name, rbnode *n) const
{
    std::string penultimate;
    if (!n)
    {
        return std::make_pair(nullptr, nullptr);
    }
    else
    {
        while (n)
        {
            if (n->name < name)
            {
                penultimate = n->name;
                n = n->right;
            }
            else if (n->name > name)
            {
                penultimate = n->name;
                n = n->left;
            }
            else
            {
                return std::make_pair(n, n);
            }
        }
        // while loop terminates, in other words element not found
        // take the value of the parent of this node, and find successor and predecessor
        // it is likely that these are not the tightest bounds, so we check this
        std::pair<rbnode *, rbnode *> bounds = std::make_pair(successor(penultimate), predecessor(penultimate));
        if (bounds.first)
        {
            rbnode *tightleft = predecessor(bounds.first->name);
            while (tightleft && tightleft->name > name)
            {
                bounds.first = tightleft;
                tightleft = predecessor(bounds.first->name);
            }
        }
        if (bounds.second)
        {
            rbnode *tightright = successor(bounds.second->name);
            while (tightright && tightright->name < name)
            {
                bounds.second = tightright;
                tightright = successor(bounds.second->name);
            }
        }

        return bounds;
        // we use either of these values for the range query when
        // the queried name is not found, and we need the next largest
        // or the next smallest name
    }
}

/* FIND A RANGE OF RB-NODES */
std::vector<rbnode *> RBGraph::find_range(const std::string &name1, const std::string &name2) const
{
    rbnode *leftbound = find(name1, root);
    rbnode *rightbound = find(name2, root);

    if (!leftbound)
    {
        // find the first name after the leftbound
        leftbound = rfind(name1, root).first;
    }
    if (!rightbound)
    {
        // find the first name before the rightbound
        rightbound = rfind(name2, root).second;
    }

    std::vector<rbnode *> names;
    names.push_back(leftbound);
    rbnode *current = successor(leftbound->name);

    while (current->name != rightbound->name)
    {
        names.push_back(current);
        current = successor(current->name);
    }

    names.push_back(rightbound);

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

/* FIND THE RB-NODE WITH NAME DIRECTLY PRECEDING THE GIVEN NAME */
rbnode *RBGraph::predecessor(const std::string &name) const
{
    rbnode *current = find(name, root);
    if (current)
    {
        // if this rbnode has a left subtree
        if (current->left)
        {
            current = current->left;
            while (current->right != nullptr)
            {
                current = current->right;
            }
        }
        // if this rbnode has no left subtree,
        // starting from root to that rbnode, the last right rbnode
        // passed before arriving at the rbnode is the successor
        else
        {
            rbnode *lastright = root;
            while (lastright->name != name)
            {
                if (lastright->name > name)
                {
                    lastright = lastright->left;
                }
                else
                {
                    current = lastright;
                    lastright = lastright->right;
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

/* ROTATE LEFT */
void RBGraph::lrotate(rbnode *&r, rbnode *&pivot)
{
    // 1) temporary pointer to pivot's right child
    rbnode *rc_pivot = pivot->right;

    // 2) set pivot's right child to be pivot's right child's left child
    pivot->right = rc_pivot->left;

    // 2a) if that old right child's left child exists then ORC's LC 
    // needs to update its parent
    if (rc_pivot->left)
    {
        pivot->right->parent = pivot;
    }

    // 3) pivot's old right child's parent is pivot's parent
    // i.e. pivot's old right child and pivot now share the same parent
    rc_pivot->parent = pivot->parent;

    // 4a) if their parent exists check whether pivot is
    // its parent's left or right child
    if (pivot->parent)
    {
        if (pivot == pivot->parent->left)
        {
            // 4aa) if pivot is a left child
            pivot->parent->left = rc_pivot;
            // set the pivot's parent's left child
            // to be parent's grandchild instead
            // (pivot's old right child)
        }
        else
        {
            // 4ab) if pivot is a right child
            pivot->parent->right = rc_pivot;
            // set the pivot's parent's right child
            // to be parent's grandchild instead
            // (pivot's old right child)
        }
    }
    // 4b) if their parent does not exist (their parent is a nullptr)
    // that means pivot's parent (and rc pivot) is the root
    // set the root to be pivot's old right child
    else if (!pivot->parent)
    {
        r = rc_pivot;
    }

    // 5) set pivot's old right child's new left child to be pivot
    // instead of being its old left child (which is now pivot's
    // right child)
    rc_pivot->left = pivot;
    // 6) set pivot's parent be pivot's old right child
    pivot->parent = rc_pivot;

    return;
}

/* ROTATE RIGHT */
void RBGraph::rrotate(rbnode *&r, rbnode *&pivot)
{
    // 1) temporary pointer to pivot's left child
    rbnode *lc_pivot = pivot->left;

    // 2) set pivot's left child to be pivot's left child's right child
    pivot->left = lc_pivot->right;

    // 2a) if that old left child's right child exists then OLC's RC
    // needs to update its parent
    if (lc_pivot->right)
    {
        pivot->left->parent = pivot;
    }

    // 3) pivot's old left child's parent is pivot's parent
    // i.e. pivot's old left child and pivot now share the same parent
    lc_pivot->parent = pivot->parent;

    // 4a) if their parent exists check whether pivot is
    // its parent's left or right child
    if (pivot->parent)
    {
        if (pivot == pivot->parent->left)
        {
            // 4aa) if pivot is a left child
            pivot->parent->left = lc_pivot;
            // set the pivot's parent's left child
            // to be parent's grandchild instead
            // (pivot's old left child)
        }
        else
        {
            // 4ab) if pivot is a right child
            pivot->parent->right = lc_pivot;
            // set the pivot's parent's right child
            // to be parent's grandchild instead
            // (pivot's old right child)
        }
    }
    // 4b) if their parent does not exist (their parent is a nullptr)
    // that means pivot's parent (and rc pivot) is the root
    // set the root to be pivot's old left child
    else if (!pivot->parent)
    {
        r = lc_pivot;
    }

    // 5) set pivot's old right child's new left child to be pivot
    // instead of being its old left child (which is now pivot's
    // right child)
    lc_pivot->right = pivot;
    // 6) set pivot's parent be pivot's old right child
    pivot->parent = lc_pivot;

    return;
}
