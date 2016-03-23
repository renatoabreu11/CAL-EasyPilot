#include "Edge.h"

template<class T>
Edge<T>::Edge(){
	this->dest = NULL;
}

template<class T>
Edge<T>::Edge(Vertex<T> *dest, int weight){
	this->dest = dest;
	this->weight = weight;
}

template<class T>
Vertex<T>* Edge<T>::getDest() const
{
	return this->dest;
}

template<class T>
int Edge<T>::getWeight() const
{
	return this->weight;
}
