#ifndef DFS_H
#define DFS_H

#include<iostream>
#include<list>
#include<stack>
#include<vector>

class Graph{
    private :
        int V;
        std::vector<bool> visited;
        std::list<int> *adj;
    public :
        Graph();
        Graph(int V);
        void addEdge(int, int);
        void DFS_Rec(int);
};

#endif /* DFS_H */