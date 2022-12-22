#include "./include/MAXFLOW.h"
#include<fstream>
#include<sstream>
#include<string>
#include <chrono>
using namespace std;
int main(){
    string line;
    stringstream ss;
    int No_of_Vertex,from_Vertex,to_Vertex,capacity;
    auto start = chrono::high_resolution_clock::now();
    
    fstream file("./text/maxflow.txt");
    file>>No_of_Vertex;
    Network N(No_of_Vertex);
    if(file.is_open()){
        while(getline(file,line)){
            file >> from_Vertex >> to_Vertex >> capacity;
            N.addEdge(from_Vertex, to_Vertex, capacity);
            //cout<< from_Vertex<< to_Vertex << capacity<<endl;
        }
        file.close();
    }
    cout << endl << "\t\t ---- Ford-Fulkerson Algorithm Serial ---- " << endl;
    cout << endl << "\t Max-Flow is : " << N.maxflow(0,No_of_Vertex-1) << endl;

    auto duration = chrono::duration_cast<chrono::milliseconds>(start - chrono::high_resolution_clock::now());

    cout << " \tduration -" << duration.count() << " ms" << endl;
    return 0;
}