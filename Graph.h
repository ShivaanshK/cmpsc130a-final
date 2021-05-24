#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include "Graphnode.h"
using namespace std;

class Graph
{
public:
    int c = 1;
    Graph(){};
    void insert(string s)
    {
        string word("");
        string name, age, occupation;
        vector<string> friends;
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
        int fileIndex = addToFile(name, age, occupation);
        Graphnode temp;
        temp.name = name;
        temp.filePointer = fileIndex;
        graph.push_back(temp);
        Node *tail;
        for (int i = 0; i < friends.size(); i++)
        {
            if (graph[graph.size() - 1].list == NULL)
            {
                Node *node = new Node();
                node->name = friends[i];
                graph[graph.size() - 1].list = node;
                tail = node;
            }
            else
            {
                Node *node = new Node();
                node->name = friends[i];
                tail->next = node;
                tail = node;
            }
        }
    }

    int addToFile(string name, string age, string occupation)
    {
        ofstream output("directory.txt", ios::app);
        output << setfill(' ') << left << setw(20) << name << setw(3) << age << setw(30) << occupation;
        output.close();
        return c++;
    }

    string* readFile(int filePointer)
    {
        char *name = new char[20];
        char *age = new char[3];
        char *occupation = new char[30];
        ifstream input("directory.txt", ios::binary);
        input.seekg(filePointer * 53, ios::beg);
        input.read(name, 20);
        input.read(age, 3);
        input.read(occupation, 30);
        input.close();
        input.clear();
        string n(""), a(""), o("");
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
        string* arr = new string[3];
        arr[0] = n;
        arr[1] = a;
        arr[2] = o;
        return arr;
    }

    void printGraph()
    {
        Node *temp;
        cout << "Graph:" << endl;
        for (int i = 0; i < graph.size(); i++)
        {
            cout << graph[i].filePointer << ": " << graph[i].name;
            temp = graph[i].list;
            while (temp != NULL)
            {
                cout << " --> " << temp->name;
                temp = temp->next;
            }
            cout << endl;
            cout << endl;
        }
    }

    void insertUser(string user)
    {
        Graphnode temp;
        temp.name = user;
        graph.push_back(temp);
    }

    void insertFriendship(string p1, string p2)
    {
        for (int i = 0; i < graph.size(); i++)
        {
            if (graph[i].name == p1)
            {

                Node *temp = graph[i].list;
                if (temp == NULL)
                {
                    Node *node = new Node();
                    node->name = p2;
                    graph[i].list = node;
                }
                else
                {
                    while (temp->next != NULL)
                        temp = temp->next;
                    Node *node = new Node();
                    node->name = p2;
                    temp->next = node;
                }
            }
            else if (graph[i].name == p2)
            {
                Node *temp = graph[i].list;
                if (temp == NULL)
                {
                    Node *node = new Node();
                    node->name = p1;
                    graph[i].list = node;
                }
                else
                {
                    while (temp->next != NULL)
                        temp = temp->next;
                    Node *node = new Node();
                    node->name = p1;
                    temp->next = node;
                }
            }
        }
    }

private:
    vector<Graphnode> graph;
};

#endif
