#include "../include/MAXFLOW.h"
using namespace std;

Network::Network(){
    this->Network_size = 0;
    Graph = vector<vector<int>>(Network_size,vector<int>(Network_size));
}

Network::Network(int Network_size){
    this->Network_size = Network_size;
    Graph = vector<vector<int>>(Network_size,vector<int>(Network_size));
}

void Network::addEdge(int from,int to, int capacity){
    Graph[from][to] = capacity;
}

bool Network::bfs(vector<int>&parent,int source,int sink){
    
    queue<int> vertex_Queue;
    vector<bool>visited(Network_size,false);

    vertex_Queue.push(source);
    visited[source] = true;
    parent[source] = -1;

    while(!vertex_Queue.empty()){

        int from_vertex = vertex_Queue.front();
        vertex_Queue.pop();

        for(int to_vertex = 0; to_vertex < Network_size; to_vertex++ ){

            if(!visited[to_vertex] && Graph[from_vertex][to_vertex]>0){

                vertex_Queue.push(to_vertex);
                visited[to_vertex] = true;
                parent[to_vertex] = from_vertex;
            }
        } 
    }

    return visited[sink];
}

int Network::maxflow(int source,int sink){

    vector<int>parent(Network_size,-1);
    int maxflow = 0;
    int from = 0;
    int to = 0;
    while(Network::bfs(parent,source,sink)){            // The while loop continues as long as there is a augmneted path

        int minflow = INT_MAX;
        to = sink;

        while(to!=source){                               //the while loop is to find the minflow in the augmented path
            from = parent[to];
            minflow = min(minflow,Graph[from][to]);
            to = from;
        }
                                                        // the for loop is to create residual graph from residual capacity
        for(to = sink; to!= source; to = parent[to]){
            from = parent[to];
            Graph[from][to] -= minflow;
            Graph[to][from] += minflow;
        }

        maxflow += minflow;
        parent.assign(Network_size,-1);
    }

    return maxflow;

}