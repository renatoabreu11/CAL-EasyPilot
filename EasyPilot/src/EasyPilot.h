
#ifndef SRC_EASYPILOT_H_
#define SRC_EASYPILOT_H_

#include "Graph.h"
#include "graphviewer.h"
#include "menu.h"
#include "iostream"
#include "fstream"
#include "string"

class EasyPilot {
public:
	EasyPilot();
	virtual ~EasyPilot();
	bool readOSM(string filename);

private:
	Graph<int> graph;
};

#endif /* SRC_EASYPILOT_H_ */
