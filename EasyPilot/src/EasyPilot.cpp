
#include "EasyPilot.h"

struct Link{
	unsigned node1Id, node2Id, roadId;
	Link(unsigned r, unsigned n1, unsigned n2) : roadId(r), node1Id(n1), node2Id(n2){}
};

EasyPilot::EasyPilot() {
}

EasyPilot::~EasyPilot() {
	// TODO Auto-generated destructor stub
}

bool EasyPilot::readOSM(string filename) {
	string edgesFile = filename + "Edges.txt";
	string connectionsFile = filename + "Connections.txt";
	string nodesFile = filename + "Nodes.txt";

	ifstream nodes, edges, connections;
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

	connections.exceptions( ifstream::badbit | ifstream::failbit);

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
			aux = line.substr(firstSemicolon + 1, lastSemicolon - firstSemicolon - 1);
			roadName = aux.c_str();
			aux = line.substr(lastSemicolon + 1, line.size());
			if(aux == "False")
				isTwoWay = false;
			else isTwoWay = true;
			for(unsigned int i = 0; i < links.size(); i++){
				if(links[i]->roadId == roadId){
					graph.addEdge(links[i]->node1Id, links[i]->node2Id, 1, isTwoWay, roadId, roadName);
				}
			}

		}
	} catch (ifstream::failure &e) {
		cout << "Error while opening " << edgesFile << endl;
	}

	edges.close();

	return true;
}

void EasyPilot::graphInfoToGV(){
	gv = new GraphViewer(GV_WINDOW_WIDTH, GV_WINDOW_HEIGHT, true);
	gv->createWindow(GV_WINDOW_WIDTH, GV_WINDOW_HEIGHT);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");

//	LimitCoords l = getLimitCoords(graph);	// gets max and min coords
//	cout << l.maxLat << ',' << l.minLat << ',' << l.maxLong << ',' << l.minLong << endl;

	vector<Vertex<unsigned> * > vertex = graph.getVertexSet();
	for (int i = 0; i < graph.getNumVertex(); i++)
		gv->addNode(i, vertex[i]->getLongitude(), vertex[i]->getLatitude());

	int srcNode, dstNode, counter = 0;
	for (int i = 0; i < graph.getNumVertex(); i++) {
		vector<Edge<unsigned>  > adjEdges = vertex[i]->getAdj();
		for (unsigned int j = 0; j < adjEdges.size(); j++) {
			srcNode = graph.getVertexIndex(vertex[i]->getInfo());
			dstNode = graph.getVertexIndex(adjEdges[j].getDest()->getInfo());
			if (adjEdges[j].getTwoWays()){
				gv->addEdge(counter, srcNode, dstNode, EdgeType::UNDIRECTED);
			}
			else
				gv->addEdge(counter, srcNode, dstNode, EdgeType::DIRECTED);

			gv->setEdgeLabel(counter,
					adjEdges[j].getName());
			counter++;
		}
	}
	gv->rearrange();
}


/***UTILITY FUNCTIONS***/

LimitCoords getLimitCoords(Graph<unsigned> g)
{
	LimitCoords l;

	//Initializes minimum value with highest possible
	double minLat = FLT_MAX;
	double minLong = FLT_MAX;

	double maxLat = -FLT_MAX;
	double maxLong = -FLT_MAX;

	vector<Vertex<unsigned> * > vertex = g.getVertexSet();
	for (int i = 0; i < g.getNumVertex(); i++)
	{
		if(vertex[i]->getLatitude() < minLat)
			minLat = vertex[i]->getLatitude();

		if(vertex[i]->getLongitude() < minLong)
			minLong = vertex[i]->getLongitude();

		if(vertex[i]->getLatitude() > maxLat)
			maxLat = vertex[i]->getLatitude();

		if(vertex[i]->getLongitude() > maxLong)
			maxLong = vertex[i]->getLongitude();
	}

	l.maxLat = maxLat;
	l.maxLong = maxLong;
	l.minLat = minLat;
	l.minLong = minLong;

	return l;
}

double resizeLat(double lat, LimitCoords l, float windowH)
{
	return (windowH/(l.maxLat-l.minLat)*lat);
}

double resizeLong(double lon, LimitCoords l, float windowW)
{
	return (windowW/(l.maxLong-l.minLong)* lon);
}
