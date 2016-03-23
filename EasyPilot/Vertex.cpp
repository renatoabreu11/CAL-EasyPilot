#pragma once

#include "Vertex.h"

template<class T>
Vertex<T>::Vertex()
{

}

template<class T>
Vertex<T>::Vertex(T data)
{
	this->data = data;
	this->dist = INFINITY;
}

template<class T>
T Vertex<T>::getData() const
{
	return this->data;
}

template<class T>
int Vertex<T>::getDist() const
{
	return this->dist;
}

template<class T>
bool  Vertex<T>::getVisited(){
	return this->visited;
}

template<class T>
vector<Edge *> Vertex<T>::getEdges() const
{
	return this->edges;
}

template<class T>
void Vertex<T>::setData(T data)
{
	this->data = data;
}

template<class T>
void Vertex<T>::setVisited(bool b){
	this->visited = b;
}

template<class T>
void Vertex<T>::setDist(int dist)
{
	this->dist = dist;
}

template<class T>
void Vertex<T>::addEdge(Edge<T> *edge)
{
	this->edges.push_back(edge);
}

template<class T>
bool Vertex<T>::operator<(const Vertex<T> &vertex) const
{
	return this->dist > vertex.getDist();
}

template<class T>
bool Vertex<T>::operator==(const Vertex<T> &vertex) const
{
	return this->data == vertex.getData();
}

template<class T>
ostream& operator<<(ostream &out, const Vertex<T> &vertex)
{
	out << "Name: " << vertex.getData() << ", dist = " << vertex.getDist() << endl;
	return out;
}
