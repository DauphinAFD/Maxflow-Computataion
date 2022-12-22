#ifndef MAXFLOW_H
#define MAXFLOW_H

#include<iostream>
#include<vector>
#include<queue>

class Network{
    private :
        int Network_size;
        std::vector<std::vector<int>>Graph;
    public :
        Network();
        Network(int);
        void addEdge(int, int, int);
        bool bfs(std::vector<int>&, int, int );
        int maxflow(int, int);              
};

#endif /*MAXFLOW_H*/