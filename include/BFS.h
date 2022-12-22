#ifndef BFS_H
#define BFS_H

#include<iostream>
#include<list>
#include<vector>
#include<queue>

class Graph{
    private :
        int V;
        std::list<int>*adj;
    public :
        Graph();
        Graph(int);
        void addEdge(int, int);
        void BFS(int);
};

#endif /* BFS_H */

