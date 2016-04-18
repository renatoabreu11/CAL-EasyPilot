
#ifndef SRC_EASYPILOT_H_
#define SRC_EASYPILOT_H_

#include "Graph.h"
#include "graphviewer.h"
#include "menu.h"
#include "iostream"
#include "fstream"
#include "string"

class EasyPilot {
private:
	Graph<unsigned> graph;
	GraphViewer * gv;
public:
	EasyPilot();
	virtual ~EasyPilot();
	bool readOSM(string filename);
	void graphInfoToGV();
};

#endif /* SRC_EASYPILOT_H_ */
