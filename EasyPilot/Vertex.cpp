#pragma once

#include "Vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(string name)
{
	this->name = name;
	this->dist = 9999;
}

string Vertex::getName() const
{
	return this->name;
}

int Vertex::getDist() const
{
	return this->dist;
}

vector<Edge *> Vertex::getEdges() const
{
	return this->edges;
}

Vertex* Vertex::getPath() const
{
	return this->path;
}

void Vertex::setName(string name)
{
	this->name = name;
}

void Vertex::setDist(int dist)
{
	this->dist = dist;
}

void Vertex::addEdges(Edge *edge)
{
	this->edges.push_back(edge);
}

void Vertex::setPath(Vertex *vertex)
{
	this->path = vertex;
}

bool Vertex::operator<(const Vertex &vertex) const
{
	return this->dist > vertex.getDist();
}

bool Vertex::operator==(const Vertex &vertex) const
{
	return this->name == vertex.getName();
}

ostream& operator<<(ostream &out, const Vertex &vertex)
{
	out << "Name: " << vertex.getName() << ", dist = " << vertex.getDist() << endl;
	return out;
}