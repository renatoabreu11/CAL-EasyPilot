#include "Edge.h"

Edge::Edge()
{

}

Edge::Edge(Vertex *from, Vertex *to, int weight)
{
	this->from = from;
	this->to = to;
	this->weight = weight;
}

Vertex* Edge::getFrom() const
{
	return this->from;
}

Vertex* Edge::getTo() const
{
	return this->to;
}

int Edge::getWeight() const
{
	return this->weight;
}