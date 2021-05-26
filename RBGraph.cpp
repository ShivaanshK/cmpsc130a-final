#include "RBGraph.h"

//Constructor, Destructor, and insert
RBGraph::RBGraph()
{
    root = nullptr;
    elements = 0;
}

RBGraph::~RBGraph()
{
    if (root != nullptr)
        clear(root);
}

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
                name = word;
            else if (counter == 1)
                age = word;
            else if (counter == 2)
                occupation = word;
            else if (word != "")
                friends.push_back(word);
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
    if (!find(name, root))
    {                                                      // insert only if the name is not in the tree
        RB_Node *entry = new RB_Node{name, red, elements}; // increment the number of RB_Nodes after insert
        root = binsert(root, entry);                       // root == entry if tree is empty
        fix(root, entry);
        write_to_file(name, age, occupation);
        Graphnode temp;
        temp.name = name;
        temp.filePointer = elements;
        graph.push_back(temp);
        Linked_List_Node *tail;
        for (int i = 0; i < friends.size(); i++)
        {
            if (graph[graph.size() - 1].list == nullptr)
            {
                Linked_List_Node *node = new Linked_List_Node();
                node->name = friends[i];
                graph[graph.size() - 1].list = node;
                tail = node;
            }
            else
            {
                Linked_List_Node *node = new Linked_List_Node();
                node->name = friends[i];
                tail->next = node;
                tail = node;
            }
        }
        elements++; // after BST insert, fix the RBT
    }

    return;
}

//Read/Write to file functions:

void RBGraph::write_to_file(const std::string &name, const std::string &age, const std::string &occupation) const
{
    std::ofstream output("directory.txt", std::ios::app);
    output << std::setfill(' ') << std::left << std::setw(20) << name << std::setw(3) << age << std::setw(30) << occupation;
    output.close();
}

std::vector<std::string> RBGraph::read_file(i &filePointer)
{
    char *na = new char[20];
    char *ag = new char[3];
    char *occ = new char[30];
    std::ifstream input("directory.txt", std::ios::binary);
    input.seekg(filePointer * 53, std::ios::beg);
    input.read(na, 20);
    input.read(ag, 3);
    input.read(occ, 30);
    input.close();
    input.clear();
    std::string name(na), age(ag), occupation(occ), n(""), a(""), o("");
    int k = 0;
    for (int i = 0; i < 20; i++)
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
    for (int i = 0; i < 3; i++)
    {
        if (age[i] == ' ')
        {
            break;
        }
        a += age[i];
    }
    k = 0;
    for (int i = 0; i < 30; i++)
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
    s.push_back(n);
    s.push_back(a);
    s.push_back(o);
    return s;
}

//Graph Functions:

void RBGraph::insert_user(const std::string &name, const std::string &age, const std::string &occupation)
{
    RB_Node *entry = new RB_Node{name, red, elements}; // increment the number of RB_Nodes after insert
    root = binsert(root, entry);                       // root == entry if tree is empty
    fix(root, entry);
    write_to_file(name, age, occupation);
    Graphnode temp;
    temp.name = name;
    temp.filePointer = elements;
    graph.push_back(temp);
    elements++;
    return;
}

void RBGraph::insert_friendship(const std::string &p1, const std::string &p2)
{
    for (int i = 0; i < graph.size(); i++)
    {
        if (graph[i].name == p1)
        {

            Linked_List_Node *temp = graph[i].list;
            if (temp == nullptr)
            {
                Linked_List_Node *node = new Linked_List_Node();
                node->name = p2;
                graph[i].list = node;
            }
            else
            {
                while (temp->next != nullptr)
                    temp = temp->next;
                Linked_List_Node *node = new Linked_List_Node();
                node->name = p2;
                temp->next = node;
            }
        }
        else if (graph[i].name == p2)
        {
            Linked_List_Node *temp = graph[i].list;
            if (temp == nullptr)
            {
                Linked_List_Node *node = new Linked_List_Node();
                node->name = p1;
                graph[i].list = node;
            }
            else
            {
                while (temp->next != nullptr)
                    temp = temp->next;
                Linked_List_Node *node = new Linked_List_Node();
                node->name = p1;
                temp->next = node;
            }
        }
    }
}

void RBGraph::print_Graph() const
{
    Linked_List_Node *temp;
    std::cout << "Graph:" << std::endl;
    for (int i = 0; i < graph.size(); i++)
    {
        std::cout << graph[i].filePointer << ": " << graph[i].name;
        temp = graph[i].list;
        while (temp != nullptr)
        {
            std::cout << " --> " << temp->name;
            temp = temp->next;
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
}

//RB Tree Functions:

/* FIND AND PRINT A RB_Node and its friends */
void RBGraph::friendship_query(const std::string &name)
{
    if (find(name, root))
    {
        Graphnode node;
        for (Graphnode person : graph)
        {
            if (person.name == name)
                node = person;
        }
        RB_Node *query = find(node.name, root);
        std::vector<std::string> nao = read_file(query->filePointer); //stores vector as <name, age, occupation>
        std::cout << nao[0] << "'s information:" << std::endl;
        std::cout << "Age: " << nao[1] << std::endl
                  << "Occupation: " << nao[2] << std::endl
                  << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << nao[0] << " friends' information:" << std::endl
                  << std::endl;
        Linked_List_Node *temp = node.list;
        while (temp != NULL)
        {
            query = find(temp->name, root);
            nao = read_file(query->filePointer);
            std::cout << nao[0] << "'s information:" << std::endl;
            std::cout << "Age: " << nao[1] << std::endl
                      << "Occupation: " << nao[2] << std::endl
                      << std::endl;
            temp = temp->next;
        }
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "------------------------------------------" << std::endl
                  << std::endl;
    }
    return;
}

/* FIND A RB_Node */
RB_Node *RBGraph::find(const std::string &name, RB_Node *n) const
{
    if (n)
    {
        if (n->name == name)
        {
            return n;
        }
        else
        {
            RB_Node *child = find(name, n->left);
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

/* PRINT A RANGE OF RB_NodeS IN LEXICOGRAPHICAL ORDER */
void RBGraph::range_query(const std::string &name1, const std::string &name2)
{
    std::vector<RB_Node *> RB_Nodelist = find_range(name1, name2);
    for (RB_Node *node : RB_Nodelist)
    {
        Graphnode n;
        for (Graphnode person : graph)
        {
            if (person.name == node->name)
                n = person;
        }
        RB_Node *query = find(n.name, root);
        std::vector<std::string> nao = read_file(query->filePointer); //stores vector as <name, age, occupation>
        std::cout << nao[0] << "'s information:" << std::endl;
        std::cout << "Age: " << nao[1] << std::endl
                  << "Occupation: " << nao[2] << std::endl
                  << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << nao[0] << " friends' information:" << std::endl
                  << std::endl;
        Linked_List_Node *temp = n.list;
        while (temp != NULL)
        {
            query = find(temp->name, root);
            nao = read_file(query->filePointer);
            std::cout << nao[0] << "'s information:" << std::endl;
            std::cout << "Age: " << nao[1] << std::endl
                      << "Occupation: " << nao[2] << std::endl
                      << std::endl;
            temp = temp->next;
        }
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "------------------------------------------" << std::endl
                  << std::endl;
    }
    return;
}

/* FIND A RANGE OF RB_NodeS */
std::vector<RB_Node *> RBGraph::find_range(const std::string &name1, const std::string &name2) const
{
    std::vector<RB_Node *> RB_Nodelist;
    RB_Nodelist.push_back(find(name1, root));
    RB_Node *current = successor(name1);

    while (current->name != name2)
    {
        RB_Nodelist.push_back(current);
        current = successor(current->name);
    }

    RB_Nodelist.push_back(find(name2, root));

    return RB_Nodelist;
}

/* FIND THE RB_Node WITH NAME DIRECTLY SUCCEEDING THE GIVEN NAME */
RB_Node *RBGraph::successor(const std::string &name) const
{
    RB_Node *current = find(name, root);
    if (current)
    {
        // if this RB_Node has a right subtree
        if (current->right)
        {
            current = current->right;
            while (current->left != nullptr)
            {
                current = current->left;
            }
        }
        // if this RB_Node has no right subtree,
        // starting from root to that RB_Node, the last left RB_Node
        // passed before arriving at the RB_Node is the successor
        else
        {
            RB_Node *lastleft = root;
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
        // if the last left RB_Node passed is this RB_Node
        // this RB_Node has no successor
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

/* PRINT ALL RB_NodeS (INORDER TRAVERSAL) */
void RBGraph::print_all()
{
    print(root);
    return;
}

/* RECURSIVE PRINT */
void RBGraph::print(RB_Node *node)
{
    if (node)
    {
        print(node->left);
        Graphnode n;
        for (Graphnode person : graph)
        {
            if (person.name == node->name)
                n = person;
        }
        RB_Node *query = find(n.name, root);
        std::vector<std::string> nao = read_file(query->filePointer); //stores vector as <name, age, occupation>
        std::cout << nao[0] << "'s information:" << std::endl;
        std::cout << "Age: " << nao[1] << std::endl
                  << "Occupation: " << nao[2] << std::endl
                  << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << nao[0] << " friends' information:" << std::endl
                  << std::endl;
        Linked_List_Node *temp = n.list;
        while (temp != NULL)
        {
            query = find(temp->name, root);
            nao = read_file(query->filePointer);
            std::cout << nao[0] << "'s information:" << std::endl;
            std::cout << "Age: " << nao[1] << std::endl
                      << "Occupation: " << nao[2] << std::endl
                      << std::endl;
            temp = temp->next;
        }
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "------------------------------------------" << std::endl
                  << std::endl;

        print(node->right);
    }
    return;
}

/* CALLED BY INSERT: PERFORMS BST INSERT */
RB_Node *RBGraph::binsert(RB_Node *r, RB_Node *entry)
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
void RBGraph::fix(RB_Node *&r, RB_Node *&entry)
{
    RB_Node *pt_entry = nullptr;  // entry's parent
    RB_Node *uc_entry = nullptr;  // entry's uncle
    RB_Node *gpt_entry = nullptr; // entry's grandparent

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
void RBGraph::lrotate(RB_Node *&r, RB_Node *&entry)
{
    // temporary pointer to entry's right child
    RB_Node *rc_entry = entry->right;

    // set entry's right child to be entry's right child's left child
    entry->right = rc_entry->left;

    // if that old right child's left child exists
    if (entry->right)
    {
        // set entry to be that old right child's left child's parent
        entry->right->parent = entry;
    }
    // entry's old right child's parent is entry's parent
    // i.e. entry's old right child and entry now share the same parent
    rc_entry->parent = entry->parent;

    // if their parent exists check whether entry is
    // its parent's left or right child
    if (entry->parent)
    {
        if (entry == entry->parent->left)
        {
            // if entry is a left child
            entry->parent->left = rc_entry;
            // set the entry's parent's left child
            // to be parent's grandchild instead
            // (entry's old right child)
        }
        else
        {
            // if entry is a right child
            entry->parent->right = rc_entry;
            // set the entry's parent's right child
            // to be parent's grandchild instead
            // (entry's old right child)
        }
    }
    // if their parent does not exist (their parent is a nullptr)
    // set the root to be entry's old right child
    else
    {
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
void RBGraph::rrotate(RB_Node *&r, RB_Node *&entry)
{
    // temporary pointer to entry's left child
    RB_Node *lc_entry = entry->left;

    // set entry's left child to be entry's left child's right child
    entry->left = lc_entry->right;

    // if that old left child's right child exists
    if (entry->left)
    {
        // set entry to be that old left child's right child's parent
        entry->left->parent = entry;
    }
    // entry's old left child's parent is entry's parent
    // i.e. entry's old left child and entry now share the same parent
    lc_entry->parent = entry->parent;

    // if their parent exists check whether entry is
    // its parent's left or right child
    if (entry->parent)
    {
        if (entry == entry->parent->left)
        {
            // if entry is a left child
            entry->parent->left = lc_entry;
            // set the entry's parent's left child
            // to be parent's grandchild instead
            // (entry's old left child)
        }
        else
        {
            // if entry is a right child
            entry->parent->right = lc_entry;
            // set the entry's parent's right child
            // to be parent's grandchild instead
            // (entry's old right child)
        }
    }
    // if their parent does not exist (their parent is a nullptr)
    // set the root to be entry's old left child
    else
    {
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

void RBGraph::clear(RB_Node *n)
{
    if (n)
    {
        clear(n->left);
        clear(n->right);
        delete n;
    }
    return;
}
