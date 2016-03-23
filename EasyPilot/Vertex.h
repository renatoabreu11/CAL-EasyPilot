#pragma once

#ifndef VERTEX_H
#define VERTEX_H

#include "Edge.h"
#include <cstdint>
#include <string>
#include <vector>
#include <limits.h>

const int INFINITY = INT_MAX;

using namespace std;

template<class T> class Edge;

template<class T> class Vertex {
private:
	T data;
	int dist;
	bool visited;
	vector<Edge<T>*> edges;

public:
	Vertex();
	Vertex(T data);

	T getData() const;
	int getDist() const;
	vector<Edge *> getEdges() const;
	Vertex *getPath() const;
	bool getVisited();

	void setVisited(bool b);
	void setData(T data);
	void setDist(int dist);
	void addEdge(Edge<T> *edge);

	bool operator<(const Vertex<T> &vertex) const;
	bool operator==(const Vertex<T> &vertex) const;
};

template<class T>
ostream& operator<<(ostream &out, const Vertex<T> &vertex);

#endif
