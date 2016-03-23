#ifndef SRC_GRAPH_H_
#define SRC_GRAPH_H_

#include <vector>
#include "Vertex.h"

using namespace std;

template<class T> class Graph{
private:
	vector<Vertex<T>*> vertexSet;

public:
	Graph();
	vector<Vertex<T>*> getVertexSet();
	void setVertexSet(vector<Vertex<T>*> v);

	void dfs();
	void dfs(Vertex<T> v);
};



#endif /* SRC_GRAPH_H_ */
