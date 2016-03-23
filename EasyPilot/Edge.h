#pragma once

#ifndef EDGE_H
#define EDGE_H

#include "Vertex.h"
#include <cstdint>
#include <vector>

using namespace std;

template<class T> class Vertex<T>;

template<class T> class Edge
{
private:
	Vertex<T> *dest;
	double weight;

public:
	Edge();
	Edge(Vertex<T> *dest, int weight);

	Vertex<T> *getDest() const;
	int getWeight() const;
};

#endif
