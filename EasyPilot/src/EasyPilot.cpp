#include "EasyPilot.h"

//tentem passar esta struct para o ficheiro .h, a mim dá-me erro
//até amanhã, beijinhos e abraços <3
struct Link {
	unsigned node1Id, node2Id, roadId;
	Link(unsigned r, unsigned n1, unsigned n2) :
			roadId(r), node1Id(n1), node2Id(n2) {
	}
};

EasyPilot::EasyPilot() {
	map = "Test"; //default map
	destinyID = 1;
	sourceID = 0;
}

EasyPilot::~EasyPilot() {
	// TODO Auto-generated destructor stub
}

bool EasyPilot::readOSM() {
	string edgesFile = map + "Edges.txt";
	string connectionsFile = map + "Connections.txt";
	string nodesFile = map + "Nodes.txt";
	string poisFile = map + "POI.txt";	// points of interest

	ifstream nodes, edges, connections, pois;
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
	vector<Link *> links;

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
			links.push_back(new Link(roadId, node1Id, node2Id));
		}
	} catch (ifstream::failure &e) {
		cout << "Error while opening " << connectionsFile << endl;
	}
	connections.close();

	line.clear();
	aux.clear();
	string roadName;
	bool isTwoWay;

	edges.exceptions(ifstream::badbit | ifstream::failbit);

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
				if (links[i]->roadId == roadId) {
					int weight = graph.calculateEdgeWeight(links[i]->node1Id, links[i]->node2Id);
					graph.addEdge(links[i]->node1Id, links[i]->node2Id, weight, isTwoWay, roadId, roadName);
				}
			}

		}
	} catch (ifstream::failure &e) {
		cout << "Error while opening " << edgesFile << endl;
	}

	edges.close();

	/********
	 * POIs *
	 ********/

	string poiName;

	/*TODO: UNCOMMENT WHEN THERE IS A FILE FOR EVERY MAP*/
	//pois.exceptions(ifstream::badbit | ifstream::failbit);

	/*File format: "<nodeID - int>;<poi name - string>"*/

	pois.open(poisFile.c_str(), ifstream::in); // optional file

	if(pois.good())
	{
		while (!pois.eof()) {
			getline(pois, line);
			firstSemicolon = line.find(';');
			aux = line.substr(0, firstSemicolon);
			nodeId = atol(aux.c_str());
			aux = line.substr(firstSemicolon + 1, line.size());
			poiName = aux.c_str();

			if (poiName != "")
			{
				graph.getVertex(nodeId)->setName(poiName);	// gives the vertex a name
			}

		}
	}

	pois.close();

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

		if(vertex[i]->getName() != "")	// if it has a name
		{
			ostringstream label;
			label << i << " - " << vertex[i]->getName();
			gv->setVertexLabel(i, label.str());
		}
	}

	int srcNode, dstNode, counter = 0;
	for (int i = 0; i < graph.getNumVertex(); i++) {
		vector<Edge<unsigned> > adjEdges = vertex[i]->getAdj();
		for (unsigned int j = 0; j < adjEdges.size(); j++) {
			srcNode = graph.getVertexIndex(vertex[i]->getInfo());
			dstNode = graph.getVertexIndex(adjEdges[j].getDest()->getInfo());
			if (adjEdges[j].getTwoWays())
				gv->addEdge(counter, srcNode, dstNode, EdgeType::UNDIRECTED);
			else
				gv->addEdge(counter, srcNode, dstNode, EdgeType::DIRECTED);

			gv->setEdgeWeight(counter, adjEdges[j].getWeight());
			gv->setEdgeLabel(counter, adjEdges[j].getName());
			counter++;
		}
	}
	gv->rearrange();
}

bool EasyPilot::highlightNode(int id){
	if(id < 0 || id > graph.getNumVertex()){
		return false;
	} else{
		gv->setVertexColor(id, "yellow");
		updateMap();
		return true;
	}
}

bool EasyPilot::highlightNode(int id, string color){
	if(id < 0 || id > graph.getNumVertex()){
		return false;
	} else{
		gv->setVertexColor(id, color);
		updateMap();
		return true;
	}
}

bool EasyPilot::highlightEdge(int id){
	if(id < 0 || id > graph.getNumEdge()){
		return false;
	} else{
		gv->setEdgeColor(id, "pink");
		gv->setEdgeThickness(id, 10);
		updateMap();
		return true;
	}
}

void EasyPilot::updateMap()
{
	gv->rearrange();
}

void EasyPilot::eraseMap()
{
	gv->closeWindow();
	gv->rearrange();
	delete(gv);
}

bool EasyPilot::highlightPath(int srcId, int destId){
	if(srcId < 0 || srcId > graph.getNumVertex() || destId < 0 || destId > graph.getNumVertex()){
		return false;
	}

	//completar -> a ceninha do floyd dá erro.

	vector<Vertex<unsigned> *> g = graph.getVertexSet();
	vector<unsigned> path = graph.getfloydWarshallPath(302627489, 269543065);
	return true;
}

string EasyPilot::getMap() const{
	return map;
}

void EasyPilot::setMap(string m) {
	map = m;
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
	return (windowH - (round(windowH / (l.maxLat - l.minLat) * (lat - l.minLat))));
}

int resizeLong(double lon, LimitCoords l, float windowW) {
	return (round(windowW / (l.maxLong - l.minLong) * (lon - l.minLong)));
}
