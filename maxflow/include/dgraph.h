#ifndef __DGRAPH_H__
#define __DGRAPH_H__

/**
 * Code adapted from https://www.inf.ufrgs.br/~MRPRITT/doku.php?id=inf05016:2017-1-trabalhos
 * Implementation of Dijkstra's algorithm using n-heaps.
 * Definition of data strutuctures for Graph representation.
 * Carlo S. Sartori - 2017/1
 * CMP588 - Advanced Algorithms, UFRGS, Prof. Marcus Ritt
 */

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

const unsigned int MAX_FAT = 1e8; 


typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>::vertex_descriptor Node;
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>::edge_descriptor Edge;

//a vertex's data
struct VertexData{
  unsigned int v;
};
 
//an edge's data
struct EdgeData{
  unsigned int capacity;            //capacity is only positive
  unsigned int residual_capacity; 
  Edge reverse_edge;                //backward edge for FF algorithm (O(1) access)
};
 
//A graph is an adjacency list represented by vectors (vecS)
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, VertexData, EdgeData> Graph;


// Read a graph in DIMACS format from an input stream and return a Graph
Graph read_dimacs_max_flow(std::istream& in, unsigned int* n, unsigned int* m, unsigned int* s, unsigned int* t);

unsigned int fattest_path(Graph& g, unsigned int s, unsigned int t, unsigned int k=2);

//// Implementation of Dijkstra's algorithm with n-heaps for testing purposes (collects memory used, number of insertions, deletions, updates and execution time)
//unsigned int dijkstra_nheap_test(const Graph& g, unsigned int s, unsigned int t, unsigned int *n_ins, unsigned int *n_del, unsigned int *n_upd, long double *time, size_t *mem=NULL, unsigned int nh=2);

#endif //__DGRAPH_H__
