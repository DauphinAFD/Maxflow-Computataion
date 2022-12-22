#include "../include/BFS.h"
#include<iostream>
using namespace std;

Graph::Graph(){
    this->V = 0;
    adj = new list<int>[V];
}
Graph::Graph(int V){
    this->V = V;
    adj = new list<int>[V];
}
void Graph::addEdge(int n,int m){
    adj[n].push_back(m);
}
void Graph::BFS(int v){
    vector<bool> visited;
    visited.assign(V,false);

    list<int> queue;
    queue.push_back(v);

    while(!queue.empty()){
        v = queue.front();
        cout << v << " ";
        queue.pop_front();
        for(auto i : adj[v]){
            if(!visited[i]){
                visited[i] = true;
                queue.push_back(i);
            }
        }
    }

}
