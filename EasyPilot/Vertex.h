#pragma once

#ifndef VERTEX_H
#define VERTEX_H

#include "Edge.h"
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

class Edge;

class Vertex
{
private:
	string name;
	int dist;
	vector<Edge *> edges;
	Vertex *path;

public:
	Vertex();
	Vertex(string name);

	string getName() const;
	int getDist() const;
	vector<Edge *> getEdges() const;
	Vertex *getPath() const;
	
	void setName(string name);
	void setDist(int dist);
	void addEdges(Edge *edge);
	void setPath(Vertex **vertex);

	bool operator<(const Vertex &vertex) const;
	bool operator==(const Vertex &vertex) const;
};

ostream& operator<<(ostream &out, const Vertex &vertex);

#endif