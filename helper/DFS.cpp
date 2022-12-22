#include "../include/DFS.h"
#include <iostream>
using namespace std;

Graph::Graph(){
    this->V = 0;
    adj = new list<int>[V];
    visited.assign(V,false);
}

Graph::Graph(int V){
    this->V = V;
    adj = new list<int>[V];
    visited.assign(V,false);
}

void Graph::addEdge(int n, int m){
    adj[n].push_back(m);
}

void Graph::DFS_Rec(int v){
    visited[v] = true;
    cout<<v <<" ";
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i){
        if (!visited[*i])
            DFS_Rec(*i);
    }
}
