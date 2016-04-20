
#ifndef SRC_EASYPILOT_H_
#define SRC_EASYPILOT_H_

#include "Graph.h"
#include "graphviewer.h"
#include "iostream"
#include "fstream"
#include "string"
#include <float.h>

#define GV_WINDOW_WIDTH 1600 	// graph viewer x resolution
#define GV_WINDOW_HEIGHT 700	// gv y resolution

class EasyPilot {
private:
	Graph<unsigned> graph;
	GraphViewer * gv;
	string map;
	int sourceID;
	int destinyID;
	vector<int> pointsOfInterest; // definido pelo utilizador
	vector<int> inaccessibleZones; //o melhor será fazer um ficheiro com esta informação
public:
	EasyPilot();
	virtual ~EasyPilot();
	bool readOSM();
	void graphInfoToGV();
	bool highlightNode(int id);
	bool highlightNode(int id, string color);
	bool highlightEdge(int id);
	bool highlightPath(int srcId, int destId);
	void eraseMap();
	void updateMap();
	string getMap() const;
	void setMap(string m);
	int getsourceID() const;
	void setsourceID(string m);
	int getdestinyID() const;
	void setdestinyID(string m);
	bool addPointOfInterest(int id);
	void getPointsOfInterest() const;
};

/*Utility functions and classes to adapt nodes position in GV*/

//continua a fazer a documentação aleixo. Tás a trabalhar bem, gg

struct Link {
	Link() {
	}

	unsigned node1Id, node2Id, roadId;
	Link(unsigned r, unsigned n1, unsigned n2) :
			roadId(r), node1Id(n1), node2Id(n2) {
	}
};

/**
 * LimitCoords will store the highest and lowest longitude and latitude of the nodes
 */
struct LimitCoords
{
	double maxLat;
	double minLat;
	double maxLong;
	double minLong;
};

/**
 * Stores in a LimitCoords struct the max coordinate values of map graph g
 */
LimitCoords getLimitCoords(Graph<unsigned> g);
int resizeLat(double lat, LimitCoords l, float windowH);
int resizeLong(double lon, LimitCoords l, float windowW);

#endif /* SRC_EASYPILOT_H_ */
