#include "Edge.h"
#include "Vertex.h"
#include <queue>
#include <cstdint>
#include <iostream>

using namespace std;

bool check(priority_queue<Vertex> q, Vertex *w)
{
	priority_queue<Vertex> a = q;

	while (!a.empty())
	{
		Vertex v = a.top();
		a.pop();

		if (&v == w)
			return true;
	}

	return false;
}

int main()
{
	Vertex v1("v1"), v2("v2"), v3("v3"), v4("v4"), 
		v5("v5"), v6("v6"), v7("v7");

	//Edge *a = new Edge(&v1, &v2, 2);

	v1.addEdges(new Edge(&v1, &v2, 2));
	v1.addEdges(new Edge(&v1, &v4, 1));

	v2.addEdges(new Edge(&v2, &v4, 3));
	v2.addEdges(new Edge(&v2, &v5, 1));

	v3.addEdges(new Edge(&v3, &v1, 4));
	v3.addEdges(new Edge(&v3, &v6, 5));

	v4.addEdges(new Edge(&v4, &v3, 2));
	v4.addEdges(new Edge(&v4, &v6, 8));
	v4.addEdges(new Edge(&v4, &v7, 4));
	v4.addEdges(new Edge(&v4, &v5, 2));

	v5.addEdges(new Edge(&v5, &v7, 6));

	v7.addEdges(new Edge(&v7, &v6, 1));

	vector<Vertex> vertexes;
	vertexes.push_back(v1);
	vertexes.push_back(v2);
	vertexes.push_back(v3);
	vertexes.push_back(v4);
	vertexes.push_back(v5);
	vertexes.push_back(v6);
	vertexes.push_back(v7);

	for (int i = 0; i < vertexes.size(); i++)
	{
		//vertexes[i].setPath(nullptr);
		vertexes[i].setDist(9999);
	}

	vertexes[0].setDist(0);

	priority_queue<Vertex> q;
	q.push(vertexes[0]);

	while (!q.empty())
	{
		Vertex v = q.top();
		q.pop();
		for (int i = 0; i < v.getEdges().size(); i++)
		{
			Vertex *w = v.getEdges()[i]->getTo();
			
			//cout << w->getDist() << endl;
			//cout << *w;

			if (v.getDist() + v.getEdges()[i]->getWeight() < w->getDist())
			{
				cout << v.getDist() + v.getEdges()[i]->getWeight() << endl;
				w->setDist(v.getDist() + v.getEdges()[i]->getWeight());
				w->setPath(&v);

				if (!check(q, w))
					q.push(*w);
			}
		}
	}

	for (unsigned int i = 0; i < vertexes.size(); i++)
		cout << vertexes[i] << endl;

	cout << vertexes[2].getPath();
}