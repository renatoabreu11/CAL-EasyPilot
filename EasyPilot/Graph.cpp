
#include "Graph.h"

template<class T>
Graph<T>::Graph(){

}

template<class T>
vector<Vertex<T>*> Graph<T>::getVertexSet(){
	return this->vertexSet;
}

template<class T>
void Graph<T>::setVertexSet(vector<Vertex<T>*> v){
	this->vertexSet = v;
}

template<class T>
void Graph<T>::dfs(){
	typename vector<Vertex<T>*>::iterator it = vertexSet.begin();
	for(; it != vertexSet.end(); it++){
		it->setVisited(false);
	}

	it = vertexSet.begin();
	for(; it != vertexSet.end(); it++){
		if(!it->getVisited()){
			dfs(*it);
		}
	}
}

template<class T>
void Graph<T>::dfs(Vertex<T> v){
	v.setVisited(true);

	//completar

	vector<Edge<T>*> e = v.getEdges();
	typename vector<Edge<T>*>::iterator it = e.begin();

	for(; it != e.end(); it++){
		if(it->getDest.getVisited() == false){
			dfs(it->getDest);
		}
	}


}
