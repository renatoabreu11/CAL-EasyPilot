#include "EasyPilot.h"

EasyPilot::EasyPilot() {
	map = "Esposende"; //default map
	destinyID = 587;
	sourceID = 211;
	gv = NULL;
}

EasyPilot::~EasyPilot() {
	// TODO Auto-generated destructor stub
}

bool EasyPilot::readOSM() {
	string edgesFile = map + "Edges.txt";
	string connectionsFile = map + "Connections.txt";
	string nodesFile = map + "Nodes.txt";
	string pointOfInterestFile = map + "POI.txt";

	ifstream nodes, edges, connections, POIs;
	string line, aux;
	size_t firstSemicolon, lastSemicolon;
	unsigned nodeId;
	double latitudeInDegrees, longitudeInDegrees;

	nodes.exceptions(ifstream::badbit | ifstream::failbit);

	try {
		nodes.open(nodesFile.c_str(), ifstream::in);
		while (!nodes.eof()) {
			nodes >> line;
			firstSemicolon = line.find(';');
			lastSemicolon = line.find(';', firstSemicolon + 1);
			aux = line.substr(0, firstSemicolon);
			nodeId = atol(aux.c_str());
			aux = line.substr(firstSemicolon + 1, lastSemicolon - 1);
			latitudeInDegrees = atof(aux.c_str());
			aux = line.substr(lastSemicolon + 1, line.size());
			longitudeInDegrees = atof(aux.c_str());
			graph.addVertex(nodeId, longitudeInDegrees, latitudeInDegrees);
		}
	} catch (ifstream::failure &e) {
		cout << "Error while opening " << nodesFile << endl;
	}
	nodes.close();

	line.clear();
	aux.clear();
	unsigned node1Id, node2Id, roadId;
	vector<Link> links;
	Link l = Link();

	connections.exceptions(ifstream::badbit | ifstream::failbit);

	try {
		connections.open(connectionsFile.c_str(), ifstream::in);
		while (!connections.eof()) {
			connections >> line;
			firstSemicolon = line.find(';');
			lastSemicolon = line.find(';', firstSemicolon + 1);
			aux = line.substr(0, firstSemicolon);
			roadId = atol(aux.c_str());
			aux = line.substr(firstSemicolon + 1, lastSemicolon - 1);
			node1Id = atol(aux.c_str());
			aux = line.substr(lastSemicolon + 1, line.size());
			node2Id = atol(aux.c_str());
			l = Link(roadId, node1Id, node2Id);
			links.push_back(l);
		}
	} catch (ifstream::failure &e) {
		cout << "Error while opening " << connectionsFile << endl;
	}
	connections.close();

	string pointOfInterest, ident;
	unsigned id;

	POIs.exceptions(ifstream::badbit | ifstream::failbit);

	POIs.open(pointOfInterestFile.c_str(), ifstream::in);

	while (!POIs.eof()) {
		getline(POIs, line);
		firstSemicolon = line.find(';');
		lastSemicolon = line.find(';', firstSemicolon + 1);
		aux = line.substr(0, firstSemicolon);
		ident = aux.c_str();
		aux = line.substr(firstSemicolon + 1,
				lastSemicolon - firstSemicolon - 1);
		id = atol(aux.c_str());
		aux = line.substr(lastSemicolon + 1, line.size());
		pointOfInterest = aux.c_str();

		if (ident == "POI") {
			graph.getVertex(id)->setName(pointOfInterest);
		} else {
			inaccessibleZones.push_back(id);
		}
	}

	POIs.close();

	line.clear();
	aux.clear();
	string roadName;
	bool isTwoWay;
	bool blocked;

	edges.exceptions(ifstream::badbit | ifstream::failbit);
	int counter = 0;

	try {
		edges.open(edgesFile.c_str(), ifstream::in);
		while (!edges.eof()) {
			getline(edges, line);
			firstSemicolon = line.find(';');
			lastSemicolon = line.find(';', firstSemicolon + 1);
			aux = line.substr(0, firstSemicolon);
			roadId = atol(aux.c_str());
			aux = line.substr(firstSemicolon + 1,
					lastSemicolon - firstSemicolon - 1);
			roadName = aux.c_str();
			aux = line.substr(lastSemicolon + 1, line.size());
			if (aux == "False")
				isTwoWay = false;
			else
				isTwoWay = true;
			for (unsigned int i = 0; i < links.size(); i++) {
				if (links[i].roadId == roadId) {
					int weight = graph.calculateEdgeWeight(links[i].node1Id,
							links[i].node2Id);
					if(find(inaccessibleZones.begin(), inaccessibleZones.end(), counter) != inaccessibleZones.end())
						blocked = true;
					else blocked = false;
					graph.addEdge(links[i].node1Id, links[i].node2Id, weight,
							isTwoWay, blocked, counter, roadName);
					counter++;
				}
			}

		}
	} catch (ifstream::failure &e) {
		cout << "Error while opening " << edgesFile << endl;
	}

	edges.close();

	/***END OF READING TXT FILES***/

	return true;
}

void EasyPilot::graphInfoToGV() {
	gv = new GraphViewer(GV_WINDOW_WIDTH, GV_WINDOW_HEIGHT, false);
	gv->createWindow(GV_WINDOW_WIDTH, GV_WINDOW_HEIGHT);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");

	LimitCoords l = getLimitCoords(graph);	// gets max and min coords

	vector<Vertex<unsigned> *> vertex = graph.getVertexSet();
	//Adds each node with resized coordinates to match window setup
	for (int i = 0; i < graph.getNumVertex(); i++) {
		int x = resizeLong(vertex[i]->getLongitude(), l, GV_WINDOW_WIDTH);
		int y = resizeLat(vertex[i]->getLatitude(), l, GV_WINDOW_HEIGHT);

		gv->addNode(i, x, y);
		if (i == sourceID) {
			gv->setVertexColor(sourceID, "orange");
		} else if (i == destinyID) {
			gv->setVertexColor(destinyID, "orange");
		}

		if (vertex[i]->getName() != "")	// if it has a name
				{
			ostringstream label;
			label << i << " - " << vertex[i]->getName();
			gv->setVertexLabel(i, label.str());
		}
	}

	int srcNode, dstNode;
	for (int i = 0; i < graph.getNumVertex(); i++) {
		vector<Edge<unsigned> > adjEdges = vertex[i]->getAdj();
		for (unsigned int j = 0; j < adjEdges.size(); j++) {
			srcNode = graph.getVertexIndex(vertex[i]->getInfo());
			dstNode = graph.getVertexIndex(adjEdges[j].getDest()->getInfo());
			if (adjEdges[j].getTwoWays())
				gv->addEdge(adjEdges[j].getId(), srcNode, dstNode, EdgeType::UNDIRECTED);
			else
				gv->addEdge(adjEdges[j].getId(), srcNode, dstNode, EdgeType::DIRECTED);

			gv->setEdgeWeight(adjEdges[j].getId(), adjEdges[j].getWeight());

			gv->setEdgeLabel(adjEdges[j].getId(), adjEdges[j].getName());
			if (adjEdges[j].getBlocked()) {
				gv->setEdgeColor(adjEdges[j].getId(), "red");
				gv->setEdgeThickness(adjEdges[j].getId(), 10);
			}
		}
	}
	gv->rearrange();
}

int EasyPilot::highlightNode(int id, string color) {
	if (id < 0 || id > graph.getNumVertex()) {
		return -1;
	} else {
		gv->setVertexColor(id, color);
		updateMap();
		return 1;
	}
}

int EasyPilot::highlightEdge(int id, string color, int thickness) {
	if (id < 0 || id > graph.getNumEdge()) {
		return -1;
	} else {
		gv->setEdgeColor(id, color);
		gv->setEdgeThickness(id, thickness);
		updateMap();
		return 1;
	}
}

void EasyPilot::updateMap() {
	gv->rearrange();
}

void EasyPilot::eraseMap() {
	this->destinyID = 0;
	this->sourceID = 1;
	this->inaccessibleZones.clear();
	this->pointsOfInterest.clear();
	this->nodePath.clear();
	this->edgePath.clear();
	gv->closeWindow();
	graph.clearGraph();
	gv = NULL;
}

void EasyPilot::highlightPath(unsigned nodeStartID, unsigned nodeDestinationID) {
	graph.dijkstraShortestPath(nodeStartID);
	vector<unsigned> graphPath = graph.getPath(nodeStartID, nodeDestinationID);

	unsigned nodeID;
	for (unsigned int i = 0; i < graphPath.size(); i++) {
		nodeID = graph.getVertexIndex(graphPath[i]);
		highlightNode(nodeID, "yellow");
		nodePath.push_back(nodeID);

		if (i + 1 < graphPath.size()) {
			vector<Edge<unsigned> > adj = graph.getVertex(graphPath[i])->getAdj();
			for (unsigned int j = 0; j < adj.size(); j++) {
				if (adj[j].getDest()->getInfo() == graph.getVertex(graphPath[i + 1])->getInfo()) {
					highlightEdge(adj[j].getId(), "pink", EDGE_THICKNESS);
					edgePath.push_back(adj[j].getId());
					break;
				}
			}
		}
	}
}

void EasyPilot::HighLightShortestPath() {

	/*
	 * Elimina os pontos inacessiveis do grafo e do gv.
	 * No entanto, o caminho calculado é muito estranho...
	 * */
//	inaccessibleZones.push_back(208);
//	for(int i = 0; i < inaccessibleZones.size(); i++){
//		graph.removeVertex(graph.getVertexSet()[inaccessibleZones[i]]->getInfo());
//		gv->removeNode(inaccessibleZones[i]);
//		updateMap();
//	}

	vector<Vertex<unsigned> *> g = graph.getVertexSet();
	unsigned node1ID;
	unsigned node2ID;

	if (pointsOfInterest.size() == 0) {
		node1ID = g[sourceID]->getInfo();
		node2ID = g[destinyID]->getInfo();
		highlightPath(node1ID, node2ID);
	} else {
		for (int i = 0; i < pointsOfInterest.size() + 1; i++) {
			if (i == 0) {
				node1ID = g[sourceID]->getInfo();
				node2ID = graph.getVertexSet()[pointsOfInterest[0]]->getInfo();
				highlightPath(node1ID, node2ID);
			} else if (i == pointsOfInterest.size()) {
				node1ID = graph.getVertexSet()[pointsOfInterest[i - 1]]->getInfo();
				node2ID = g[destinyID]->getInfo();
				highlightPath(node1ID, node2ID);
			} else {
				node1ID = graph.getVertexSet()[pointsOfInterest[i - 1]]->getInfo();
				node2ID = graph.getVertexSet()[pointsOfInterest[i]]->getInfo();
				highlightPath(node1ID, node2ID);
			}
		}
	}
}

void EasyPilot::resetPath() {
	vector<int>::iterator it = nodePath.begin();
	for (; it != nodePath.end(); it++) {
		if (*it != sourceID && *it != destinyID)
			gv->setVertexColor(*it, "blue");

		nodePath.erase(it);
		it--;
	}
	nodePath.clear();

	vector<int>::iterator itr = edgePath.begin();
	for (; itr != edgePath.end(); itr++) {
		highlightEdge(*itr, "black", DEFAULT_EDGE_THICKNESS);

		edgePath.erase(itr);
		itr--;
	}
	edgePath.clear();
}

string EasyPilot::getMap() const {
	return map;
}

void EasyPilot::setMap(string m) {
	map = m;
}

int EasyPilot::getsourceID() const {
	return sourceID;
}

int EasyPilot::setsourceID(int id) {
	gv->setVertexColor(sourceID, "blue");
	if (highlightNode(id, "orange") == -1)
		return -1;
	else
		sourceID = id;
	return 1;
}

int EasyPilot::getdestinyID() const {
	return destinyID;
}

int EasyPilot::setdestinyID(int id) {
	gv->setVertexColor(destinyID, "blue");
	if (highlightNode(id, "orange") == -1)
		return -1;
	else
		destinyID = id;
	return 1;
}

int EasyPilot::addPointOfInterest(int id) {
	if (find(pointsOfInterest.begin(), pointsOfInterest.end(), id) != pointsOfInterest.end()) {
		return 0;
	} else {
		if (highlightNode(id, "green") == -1 || id == sourceID
				|| id == destinyID)
			return -1;
		else {
			pointsOfInterest.push_back(id);
		}
		return 1;
	}
}

int EasyPilot::removePointOfInterest(int id) {
	vector<int>::iterator it = find(pointsOfInterest.begin(), pointsOfInterest.end(), id);
	if (it == pointsOfInterest.end()) {
		return 0;
	} else {
		if (highlightNode(id, "blue") == -1 || id == sourceID
				|| id == destinyID)
			return -1;
		else {
			pointsOfInterest.erase(it);
		}
		return 1;
	}
}

int EasyPilot::addInaccessibleZone(int id)
{
	if(find(inaccessibleZones.begin(), inaccessibleZones.end(), id) != inaccessibleZones.end())
		return 0;
	else if(id == sourceID || id == destinyID)
		return -1;
	else
		inaccessibleZones.push_back(id);

	return 1;
}

int EasyPilot::removeInaccessibleZone(int id)
{
	vector<int>::iterator it = find(inaccessibleZones.begin(), inaccessibleZones.end(), id);
	if(it == inaccessibleZones.end())
		return 0;
	else
		inaccessibleZones.erase(it);

	return 1;
}

/***UTILITY FUNCTIONS***/

LimitCoords getLimitCoords(Graph<unsigned> g) {
	LimitCoords l;

//Initializes minimum value with highest possible
	double minLat = FLT_MAX;
	double minLong = FLT_MAX;

	double maxLat = -FLT_MAX;
	double maxLong = -FLT_MAX;

	vector<Vertex<unsigned> *> vertex = g.getVertexSet();
	for (int i = 0; i < g.getNumVertex(); i++) {
		if (vertex[i]->getLatitude() < minLat)
			minLat = vertex[i]->getLatitude();

		if (vertex[i]->getLongitude() < minLong)
			minLong = vertex[i]->getLongitude();

		if (vertex[i]->getLatitude() > maxLat)
			maxLat = vertex[i]->getLatitude();

		if (vertex[i]->getLongitude() > maxLong)
			maxLong = vertex[i]->getLongitude();
	}

	l.maxLat = maxLat;
	l.maxLong = maxLong;
	l.minLat = minLat;
	l.minLong = minLong;

	return l;
}

int resizeLat(double lat, LimitCoords l, float windowH) {
	return (windowH
			- (round(windowH / (l.maxLat - l.minLat) * (lat - l.minLat))));
}

int resizeLong(double lon, LimitCoords l, float windowW) {
	return (round(windowW / (l.maxLong - l.minLong) * (lon - l.minLong)));
}
