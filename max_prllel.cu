#include<cuda.h>
#include<fstream>
#include<sstream>
#include<iostream>
#include<string>
#define milliseconds 1e3

# define CHECK( call )\
{\
    const cudaError_t error = call;\
    if( error != cudaSuccess) \
    {\
        cout << " Error " << __FILE__ << " : " << __LINE__ << endl;\
        cout << " Code : " << error << ", reason : " << cudaGetErrorString(error);\
        exit(1);\
    }\
}

using namespace std;

typedef struct Vertex_info{
    int parent;
    int flow;
}Vertex_info;

void readfile(string filename,int N, int* residual_capacity){

    int source, destination, capacity;
    string line;
    stringstream ss;
    
    fstream file(filename);

    if(file.is_open()){

        while(getline(file,line)){
            
            ss.clear();
            ss.str("");
            ss.str(line);

            ss >> source >> destination >> capacity;
            residual_capacity[source*N + destination] = capacity;

        }
        file.close();
    }
}

bool sink_found (bool* frontier, int N, int sink){
    
    for(int i = N-1; i >=0 ; i--){
        if(frontier[i]){
            return i == sink ;
        }
    }
    return true;        // frontier is empty, cannot explore further
    
}

void host_set_parameters (bool *frontier, bool *change_capacity, int source, int N){

    for(int i = 0; i < N; i++ ){
        change_capacity[i] = false;
        frontier[i] = (i == source);
    }
    
}

__global__ void device_set_parameters (bool *frontier, bool *visited, bool *change_capacity, Vertex_info *Vertex, int *locks, int N, int source){

    int id = blockDim.x * blockIdx.x + threadIdx.x;

    if(id < N){
        locks[id] = 0;
        visited[id] = false;
        change_capacity[id] = false;
        frontier[id] = (id == source);
        Vertex[id].flow = INT_MAX;
    }
}

__global__ void search_path(int* residual_capacity, Vertex_info* Vertex, bool* frontier, bool* visited, int* locks, int N, int sink ){

	int id = blockDim.x * blockIdx.x + threadIdx.x;
    
    if( id < N && frontier[id] && !frontier[sink] ){

        frontier[id] = false;
        visited[id] = true;
        int capacity; 

        Vertex_info This_Vertex = Vertex[id];
        Vertex_info* neighbour;

        for(int i = 0; i < N; i++){
            capacity = residual_capacity[id * N + i];
            if( frontier[i] || visited[i] || (capacity <= 0) ){
                continue;
            }

            if(atomicCAS(locks+i, 0 , 1) == 1 || frontier[i]){      // Ensure frontier is visited only once
				continue;
			}

            frontier[i] = true;

            locks[i] = 0;                           // unlock

            neighbour = Vertex + i;
            neighbour->parent = id;
            neighbour->flow = min(This_Vertex.flow, capacity);

        }
    }
}

__global__ void set_residual_capacity(Vertex_info *Vertex, bool *change_capacity, int* residual_capacity, int N, int bottleneck){

    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if(id < N && change_capacity[id]){
        residual_capacity[Vertex[id].parent * N + id] -= bottleneck;
        residual_capacity[id * N + Vertex[id].parent] += bottleneck; 
    }    
}

int main(){

    int N = 2000;
    int *residual_capacity;
    size_t graph_size = N*N*sizeof(int);
	
    residual_capacity = (int *)malloc(graph_size);
    memset(residual_capacity, 0, graph_size); 
    
    readfile("./text/maxflow.txt",N,residual_capacity);
    
    int maxflow = 0, bottleneck = 0;
    int source = 0, sink = N-1;                             // Setting first node as Source and the last node as Sink 
    int V;
    
    bool found_path;
	bool *frontier, *change_capacity;
    int *d_residual_capacity, *d_locks;
	bool *d_frontier, *d_visited, *d_change_capacity;

    Vertex_info *Vertex, *d_Vertex;

    size_t lock_size = N*sizeof(int);
    size_t frontier_size = N*sizeof(bool);
    size_t Vertex_size = N*sizeof(Vertex_info);

    frontier = (bool *)malloc(frontier_size);
    Vertex = (Vertex_info *)malloc(Vertex_size);
    change_capacity = (bool *)malloc(frontier_size);

    CHECK( cudaMalloc((void **)&d_change_capacity, frontier_size) );
    CHECK( cudaMalloc((void **)&d_residual_capacity, graph_size) );
    CHECK( cudaMalloc((void **)&d_frontier, frontier_size) );
    CHECK( cudaMalloc((void **)&d_visited, frontier_size) );
    CHECK( cudaMalloc((void **)&d_Vertex, Vertex_size) );
    CHECK( cudaMalloc((void **)&d_locks, lock_size) );

    
	int threads = 512;
	int blocks = ceil(N * 1.0 /threads);

    cudaMemcpy(d_residual_capacity, residual_capacity, graph_size, cudaMemcpyHostToDevice);

    clock_t start_time = clock(); 

    do{                                    
        device_set_parameters<<<blocks, threads >>>(d_frontier, d_visited,d_change_capacity, d_Vertex, d_locks, N, source);
		host_set_parameters(frontier, change_capacity, source, N);
        CHECK( cudaDeviceSynchronize() );

        while(!sink_found(frontier,N,sink)){

            search_path<<< blocks, threads >>>(d_residual_capacity, d_Vertex, d_frontier, d_visited, d_locks, N, sink);
            CHECK( cudaDeviceSynchronize() );
            cudaMemcpy(frontier, d_frontier, frontier_size, cudaMemcpyDeviceToHost);
        }

        found_path = frontier[sink];

        if(!found_path){
            break;
        }
        
        cudaMemcpy(Vertex, d_Vertex, Vertex_size, cudaMemcpyDeviceToHost);
        
        bottleneck = Vertex[sink].flow;
        maxflow += bottleneck;

        V = sink;
        while(V!= source){
            change_capacity[V] = true;
            V = Vertex[V].parent;
        }

        cudaMemcpy(d_change_capacity, change_capacity, frontier_size, cudaMemcpyHostToDevice);

        set_residual_capacity<<< blocks, threads >>>(d_Vertex, d_change_capacity, d_residual_capacity, N, bottleneck);
        CHECK( cudaDeviceSynchronize() );

    }while(found_path);

    double time_taken = ((double)clock() - start_time)/CLOCKS_PER_SEC * milliseconds; // in milliseconds 
    cout << endl << "\t\t ---- Ford-Fulkerson Algorithm Parallel ---- " << endl;
    cout << endl << "\t Max-Flow is : " << maxflow << endl;
	cout << " \tduration --" << time_taken << " ms "  << endl;

	CHECK( cudaFree(d_residual_capacity) );
    CHECK( cudaFree(d_change_capacity) );
	CHECK( cudaFree(d_frontier) );
	CHECK( cudaFree(d_visited) );
	CHECK( cudaFree(d_Vertex) );
    CHECK( cudaFree(d_locks) );
    
    free(residual_capacity);
    free(change_capacity);
	free(frontier);
	free(Vertex);

    return 0;
}