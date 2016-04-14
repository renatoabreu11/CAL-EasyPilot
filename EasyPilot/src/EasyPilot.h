
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

public:
	EasyPilot();
	virtual ~EasyPilot();
	bool readOSM(string filename);
};

#endif /* SRC_EASYPILOT_H_ */
