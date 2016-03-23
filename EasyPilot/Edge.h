#pragma once

#ifndef EDGE_H
#define EDGE_H

#include "Vertex.h"
#include <cstdint>
#include <vector>

using namespace std;

class Vertex;

class Edge
{
private:
	Vertex *from;
	Vertex *to;
	int weight;

public:
	Edge();
	Edge(Vertex *from, Vertex *to, int weight);

	Vertex *getFrom() const;
	Vertex *getTo() const;
	int getWeight() const;
};

#endif