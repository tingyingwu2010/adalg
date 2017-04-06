#include <cstdio>
#include <chrono>
#include <math.h>
#include <vector>
#include "nheap.h"
#include "dgraph.h"

int NUM_EXP = 20;

using namespace std;

vector<unsigned int> swaps(NUM_EXP+1, 0);
vector<double> times(NUM_EXP+1, 0.0);
vector<unsigned int> e(NUM_EXP+1, 0.0);

std::chrono::time_point<std::chrono::system_clock> tstart, tend;
std::chrono::duration<double> elapsed_seconds;

void test_delete(int hd){
	NHeap h(hd, pow(2, NUM_EXP)+1);
	
	for(int i=1;i<=NUM_EXP;i++){
		int N = pow(2, i) - 1;
		int n = N;
		
		while(n > 0){
			n--;
			h.insert(rand()%N, n);
		}

	   	tstart = std::chrono::system_clock::now();
		h.n_swaps = 0;
		while(n < N){
			n++;	
			h.deletemin();
		}
		
		tend = std::chrono::system_clock::now();
		elapsed_seconds = tend-tstart;	
		
		swaps[i] = h.n_swaps;
		times[i] = elapsed_seconds.count();
	}
	
	printf("%i,\t\t%u,\t\t%u,\t\t%lf\n", 0, swaps[0], e[0], times[0]);	
	for(int j=1;j<swaps.size();j++){
		printf("%i,\t\t%u,\t\t%u,\t\t%lf\n", j, swaps[j], e[j], times[j]-times[j-1]);
	}	
}

void test_update(int hd){
	
	for(unsigned int i=1;i<=NUM_EXP;i++){
		unsigned int n = pow(2, i) - 1;
		unsigned int k = pow(2, i) + 1;
		unsigned int el = 0;
		
		NHeap h(hd, n*2+1);
				
		for(unsigned int x=0;x<n;x++)
			h.insert(k, el++);
		
		n = n+1;
		k = k+1;
		
		vector<unsigned int> els;
		for(unsigned int x=0;x<n;x++){
			h.insert(k, el);
			els.push_back(el);
			el++;
		}
		
		
		h.n_swaps = 0;
		k = k-2;

	   	tstart = std::chrono::system_clock::now();	
		
		for(unsigned int x=0;x<n;x++){
			h.update_key(els[x], k--);	
		}

	   	tend = std::chrono::system_clock::now();
		elapsed_seconds = tend-tstart;			
		
		swaps[i] = h.n_swaps;
		times[i] = elapsed_seconds.count();
		e[i] = (i)*pow(2, i);
	}

	printf("%i,\t\t%u,\t\t%u,\t\t%lf\n", 0, swaps[0], e[0], times[0]);	
	for(int j=1;j<swaps.size();j++){
		printf("%i,\t\t%u,\t\t%u,\t\t%lf\n", j, swaps[j], e[j], times[j]-times[j-1]);
	}	
}


void test_insert(int hd){

	unsigned int n = pow(2, NUM_EXP)-1; //limit
	unsigned int i=2;
	
	NHeap h(hd, n);
	h.insert(n, n);
	
	h.n_swaps = 0;
	n--;
	unsigned int ninserts = 1;

   	tstart = std::chrono::system_clock::now();
   	unsigned int NI = pow(2,i)-1;
   	
	for(;n > 0 && i < NUM_EXP+1;n--){
		h.insert(n, n);
		ninserts++;
		if(NI == ninserts){
			swaps[i] = h.n_swaps;
		   	tend = std::chrono::system_clock::now();
			elapsed_seconds = tend-tstart;			
			times[i] = elapsed_seconds.count();
			e[i] = (i-1)*pow(2,i-1);
			i++;
			NI = ((NI+1)*2)-1;
		}
	}
	
	printf("%i,\t\t%u,\t\t%u,\t\t%lf\n", 0, swaps[0], e[0], times[0]);	
	for(int j=1;j<swaps.size();j++){
		printf("%i,\t\t%u,\t\t%u,\t\t%lf\n", j, swaps[j], e[j], times[j]-times[j-1]);
	}	
}


void test_scale(int hd){
	
	unsigned int n,m;

	Graph g = read_dimacs(std::cin, &n, &m);
	if(n <= 1)
		return;	

	int s = rand()%n;
	int t = rand()%n;
	while(s == t)
		t = rand()%n;
		
	tstart = std::chrono::system_clock::now();
	size_t mu = 0;
	unsigned int dst = dijkstra_nheap_mem(g, s, t, &mu, hd);
	//unsigned int dsswapst = dijkstra_nheap(g, s, t, hd);
	tend = std::chrono::system_clock::now();
	
	elapsed_seconds = tend-tstart;	
	
	printf("%lu,%f\n", mu/(1024*1024), elapsed_seconds.count());	
}



int main(int argc, char **argv){
	srand(time(0));
	
	
	
	if(op == 'i')
		test_insert(hd);
	else if(op == 'u')
		test_update(hd);
	else if(op == 'd')
		test_delete(hd);
	else if(op == 's')
		test_scale(hd);
	
}



void read_parameters(int argc, char **argv, char *op, int *hd){
	if(argc < 3){
		printf("usage: %s -t <test type> [-h <heap dimension>] [-n <number of tests>]\n\t-t test type: i, u, d\n\t-h heap dimension: natural numbers\n", argv[0]);
		exit(-1);
	}
	bool has_type = false;
	
	for(int i=1;i<argc;i++){
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
				case 't':
					i++;
					*op = argv[i][0];
			
			}
		}
	}
	
	
	int hd = 2; //default heap dimension
	if(argc == 3){
		hd = atoi(argv[2]);
	}
		
	char op = argv[1][0];	
}



