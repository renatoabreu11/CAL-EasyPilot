
#ifndef SRC_EASYPILOT_H_
#define SRC_EASYPILOT_H_

#include "Graph.h"
#include "graphviewer.h"
#include "menu.h"
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

public:
	EasyPilot();
	virtual ~EasyPilot();
	bool readOSM(string filename);
	void graphInfoToGV();
	void eraseMap();
};

/*Utility functions and classes to adapt nodes position in GV*/

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
