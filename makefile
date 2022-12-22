MAX_cpp := Max_Algo.cpp ./helper/MAXFLOW.cpp
MAX_cu := max_prllel.cu

MAXFLOW_CPP : $(MAX_cpp)
	g++ $(MAX_cpp) -o run
	./run

MAXFLOW_CU : $(MAX_cu)
	nvcc $(MAX_cu) -o run
	./run

clear :
	rm run
