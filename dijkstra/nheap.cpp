#include "nheap.h"
#include <math.h>

using namespace std;

NHeap::NHeap(int n, int key, int e){
	this->heap.push_back(key);
	this->data.push_back(e);
	this->pos_heap[e] = 0;
	this->n = n;
}

void NHeap::insert(int key, int e){
	this->heap.push_back(key);
	this->data.push_back(e);
	//heapify
}

void NHeap::update(int nkey, int e){
	int i = pos_heap[e];
	this->heap[i] = key;
	if(heap[i] < heap[floor((i-1)/n)]){ //key is smaller than parent's
		//heapify
	}else{
		//check if key is greater than of children
	}
}

void NHeap::heapify_up(int e){
	int i = pos_heap[e];
	if(heap[i] >= heap[floor((i-1)/n)]){
		
	}
}

void NHeap::heapify_down(int e){
	int i = pos_heap[e];
	int min = i;
	//get smallest child
	for(int j=n*i+1;j<n;j++){
		if(heap[min] > heap[j])
			min = j;
	}
	
	if(min == i)
		return;	
	
	//swap key
	int temp = heap[min];
	heap[min] = heap[i];
	heap[i] = temp;
	//swp data
	temp = data[min];
	data[min] = data[i];
	data[i] = temp;
	
	heapify_down(data[min]);
}	


void deletemin();
