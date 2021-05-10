#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <vector>
#include "Graphnode.h"
using namespace std;

class Graph
{
public:
    Graph(){};
    void insert(string s)
    {
        string word("");
        string name, age, occupation;
        vector<string> friends;
        int counter = 0;
        for (int i = 0; i < s.length(); i++)
        {
            if (s[i] == ',' || (i == s.length()-1))
            {
                if(i == s.length()-1){
                    word+=s[i];
                }
                if (counter == 0)
                    name = word;
                else if (counter == 1)
                    age = word;
                else if (counter == 2)
                    occupation = word;
                else
                friends.push_back(word);
                counter++;
                word = "";
            }
            else
            {
                word += s[i];
            }
        }

        Graphnode temp;
        temp.name = name;
        graph.push_back(temp);
        Node *tail;
        for (int i = 0; i < friends.size(); i++)
        {
            if (graph[graph.size() - 1].list == NULL)
            {
                Node *node = new Node();
                node->name = friends[i];
                cout << friends[i] << endl;
                graph[graph.size() - 1].list = node;
                tail = node;
            }
            else
            {
                Node *node= new Node();
                node->name = friends[i];
                tail->next = node;
                tail = node;
            }
        }
    }

    void printGraph()
    {
        Node *temp;
        cout << "Graph:" << endl;
        for (int i = 0; i < graph.size(); i++)
        {
            cout << i << ": " << graph[i].name;
            temp = graph[i].list;
            while (temp != NULL)
            {
                cout << " --> " << temp->name;
                temp = temp->next;
            }
            cout << endl;
            cout << endl;
        }
        cout << endl;
    }

private:
    vector<Graphnode> graph;
};

#endif