#include "EasyPilot.h"

EasyPilot::EasyPilot() {
	map = "Esposende"; //default map
	destinyID = 389;
	sourceID = 329;
	gv = NULL;
	POIsNavigationMethod = 2;
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
	unsigned firstID, lastID;
	float cost;
	int vertexID;

	POIs.exceptions(ifstream::badbit | ifstream::failbit);

	POIs.open(pointOfInterestFile.c_str(), ifstream::in);

	if(POIs.peek() != ifstream::traits_type::eof())	// if it's not an empty file
	{
		while (!POIs.eof()) {
			getline(POIs, line);
			firstSemicolon = line.find(';');
			lastSemicolon = line.find(';', firstSemicolon + 1);
			aux = line.substr(0, firstSemicolon);
			ident = aux.c_str();
			if (ident == "POI") {
				aux = line.substr(firstSemicolon + 1,
						lastSemicolon - firstSemicolon - 1);
				firstID = atol(aux.c_str());
				aux = line.substr(lastSemicolon + 1, line.size());
				pointOfInterest = aux.c_str();
				graph.getVertex(firstID)->setName(pointOfInterest);
			}
			else if (ident == "IZ") {
				aux = line.substr(firstSemicolon + 1,
						lastSemicolon - firstSemicolon - 1);
				firstID = atol(aux.c_str());
				aux = line.substr(lastSemicolon + 1, line.size());
				lastID = atol(aux.c_str());
				InaccessibleZone iz = InaccessibleZone(firstID, lastID);
				inaccessibleZones.push_back(iz);
			}

			else if (ident == "T") {

				aux = line.substr(firstSemicolon + 1,
						lastSemicolon - firstSemicolon - 1);
				vertexID = atoi(aux.c_str());
				aux = line.substr(lastSemicolon + 1, line.size());
				cost = atof(aux.c_str());
				Toll t = Toll(vertexID, cost);
				Tolls.push_back(t);
			}

			else {
				cout << "Wrongly defined line in "
						<< pointOfInterestFile.c_str() << ".\n";
			}
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
					InaccessibleZone iz = InaccessibleZone(graph.getVertexIndex(links[i].node1Id), graph.getVertexIndex(links[i].node2Id));
					if(find(inaccessibleZones.begin(), inaccessibleZones.end(), iz) != inaccessibleZones.end())
						blocked = true;
					else blocked = false;
					graph.addEdge(links[i].node1Id, links[i].node2Id, weight,
							isTwoWay, blocked, counter, roadName);
					counter++;
					if (isTwoWay == true) {
						graph.addEdge(links[i].node2Id, links[i].node1Id,
								weight, isTwoWay, blocked, counter, roadName);
						counter++;
					}
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
	ostringstream ss;
	ss << map << ".png";
	gv->setBackground(ss.str());
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
			gv->setVertexColor(sourceID, "yellow");
		} else if (i == destinyID) {
			gv->setVertexColor(destinyID, "yellow");
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

	/**COLOR TOLLS**/

	for(unsigned i = 0; i < Tolls.size(); i++)
	{
		gv->setVertexColor(Tolls[i].getVertexId(), "cyan");
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

double EasyPilot::getWeightOfPath(unsigned nodeStartID, unsigned nodeDestinationID) {
	vector<unsigned> graphPath = graph.getfloydWarshallPath(nodeStartID, nodeDestinationID);
	unsigned nodeID;
	double totalWeight = 0;

	for (unsigned int i = 0; i < graphPath.size(); i++) {
		nodeID = graph.getVertexIndex(graphPath[i]);

		if (i + 1 < graphPath.size()) {
			vector<Edge<unsigned> > adj = graph.getVertex(graphPath[i])->getAdj();
			for (unsigned int j = 0; j < adj.size(); j++) {
				if (adj[j].getDest()->getInfo() == graph.getVertex(graphPath[i + 1])->getInfo()) {
					totalWeight += adj[j].getWeight();
					break;
				}
			}
		}
	}

	return totalWeight;
}

void EasyPilot::highlightPath(unsigned nodeStartID, unsigned nodeDestinationID) {
	vector<unsigned> graphPath = graph.getfloydWarshallPath(nodeStartID, nodeDestinationID);
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

bool alreadyProcessed(int nodeToSearch, vector<int> POIsAlreadySeen) {
	for(unsigned int i = 0; i < POIsAlreadySeen.size(); i++) {
		if(POIsAlreadySeen[i] == nodeToSearch)
			return true;
	}

	return false;
}

vector<int> EasyPilot::sortPOIsByWeight(const vector<Vertex<unsigned> *> &g) {
	unsigned node1ID, node2ID;
	vector<int> POIsByWeightOrder;

	while(POIsByWeightOrder.size() != pointsOfInterest.size()) {
		if(POIsByWeightOrder.empty())
			node1ID = g[sourceID]->getInfo();
		else
			node1ID = POIsByWeightOrder[POIsByWeightOrder.size() - 1];

		int secondNodeToUse;
		double weight, weightCloser = INT_INFINITY;

		for(unsigned int i = 0; i < pointsOfInterest.size(); i++) {
			node2ID = graph.getVertexSet()[pointsOfInterest[i]]->getInfo();
			weight = getWeightOfPath(node1ID, node2ID);

			if(weight < weightCloser && !alreadyProcessed(node2ID, POIsByWeightOrder)) {
				weightCloser = weight;
				secondNodeToUse = graph.getVertexIndex(node2ID);
			}
		}

		POIsByWeightOrder.push_back(secondNodeToUse);
		cout << secondNodeToUse << endl;
	}

	return POIsByWeightOrder;
}

void EasyPilot::HighLightShortestPath() {
	vector<Vertex<unsigned> *> g = graph.getVertexSet();
	unsigned node1ID;
	unsigned node2ID, auxNode;

	graph.floydWarshallShortestPath();

	node1ID = g[sourceID]->getInfo();
	node2ID = g[destinyID]->getInfo();
	vector<unsigned> ret = graph.bfs(graph.getVertex(node1ID));
	if (find(ret.begin(), ret.end(), node2ID) == ret.end()){
		cout << "\nImpossible to go from point " << sourceID << " to point "
				<< destinyID << endl;
		return;
	}

	if (pointsOfInterest.size() == 0) {
		highlightPath(node1ID, node2ID);
	} else {
		vector<int>::iterator it = pointsOfInterest.begin();
		for(; it != pointsOfInterest.end(); it++){
			ret = graph.bfs(graph.getVertex(node1ID));
			auxNode = g[*it]->getInfo();
			if (find(ret.begin(), ret.end(), auxNode) == ret.end()) {
				cout << "Point of interest with node " << *it
						<< " removed because it is inaccessible";
				pointsOfInterest.erase(it);
				it--;
			}
		}
		if (pointsOfInterest.size() == 0) {
			highlightPath(node1ID, node2ID);
			return;
		}

		vector<int> POIs;
		if(POIsNavigationMethod == 2)
			POIs = sortPOIsByWeight(g);
		else
			POIs = pointsOfInterest;

		for (unsigned int i = 0; i < POIs.size() + 1; i++) {
			if (i == 0) {
				node1ID = g[sourceID]->getInfo();
				node2ID = graph.getVertexSet()[POIs[0]]->getInfo();
				highlightPath(node1ID, node2ID);
				cout << "Going to interest point '" << POIs[0] << "'\nPress ENTER to move to the next one...";
			} else if (i == POIs.size()) {
				node1ID = g[POIs[i-1]]->getInfo();
				node2ID = g[destinyID]->getInfo();
				highlightPath(node1ID, node2ID);
				cout << "Going to destination point '" << graph.getVertexIndex(node2ID) << "'\nPress ENTER to finish your trip...";
			} else {
				node1ID = g[POIs[i-1]]->getInfo();
				node2ID = g[POIs[i]]->getInfo();
				highlightPath(node1ID, node2ID);
				cout << "Going to point interest point '" << POIs[i] << "'\nPress ENTER to move to the next one...";
			}
			cin.ignore();
		}
	}
}

void EasyPilot::resetPath() {
	bool changeColorPOIs = true;
	vector<int>::iterator it = nodePath.begin();

	for (; it != nodePath.end(); it++) {
		if (*it != sourceID && *it != destinyID) {
			for(unsigned int i = 0; i < pointsOfInterest.size(); i++)
				if(*it == pointsOfInterest[i]) {
					changeColorPOIs = false;
					break;
				}

			if(changeColorPOIs)
				gv->setVertexColor(*it, "blue");
			else
				gv->setVertexColor(*it, "green");
		}

		nodePath.erase(it);
		it--;
		changeColorPOIs = true;
	}
	nodePath.clear();

	vector<int>::iterator itr = edgePath.begin();
	for (; itr != edgePath.end(); itr++) {
		highlightEdge(*itr, "black", DEFAULT_EDGE_THICKNESS);

		edgePath.erase(itr);
		itr--;
	}
	edgePath.clear();

	/**RECOLOR TOLLS**/

	for(unsigned i = 0; i < Tolls.size(); i++)
		gv->setVertexColor(Tolls[i].getVertexId(), "cyan");
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
	if (id < 0 || id > graph.getNumVertex()) {
		cout << "Node id out of range. Try again\n";
		return -1;
	}

	gv->setVertexColor(sourceID, "blue");
	highlightNode(id, "yellow");
	sourceID = id;

	cout << sourceID << endl;
	vector<Vertex<unsigned> *> v = graph.getVertexSet();
	unsigned src = v[sourceID]->getInfo();
	vector<unsigned> ret;
	unsigned auxNode;
	vector<int>::iterator it = pointsOfInterest.begin();
	for (; it != pointsOfInterest.end(); it++) {
		ret = graph.bfs(graph.getVertex(src));
		cout << *it << endl;
		auxNode = v[*it]->getInfo();
		cout << auxNode << endl;
		if (find(ret.begin(), ret.end(), auxNode) == ret.end()) {
			cout << "Point of interest with node " << *it
					<< " removed because it is inaccessible";
			gv->setVertexColor(*it, "blue");
			pointsOfInterest.erase(it);
			it--;
		}
	}

	return 1;
}

int EasyPilot::getdestinyID() const {
	return destinyID;
}

int EasyPilot::setdestinyID(int id) {
	if (id < 0 || id > graph.getNumVertex()) {
		cout << "Node id out of range. Try again\n";
		return -1;
	}

	vector<Vertex<unsigned> * > v = graph.getVertexSet();
	unsigned dst = v[id]->getInfo();
	vector<unsigned> ret = graph.bfs(v[sourceID]);
	if (find(ret.begin(), ret.end(), dst) == ret.end()) {
		cout << "The destiny node ID is unreachable. Try again\n";
		return -1;
	} else {
		gv->setVertexColor(destinyID, "blue");
		highlightNode(id, "yellow");
		destinyID = id;
	}
	return 1;
}

int EasyPilot::addPointOfInterest(int id) {
	if (find(pointsOfInterest.begin(), pointsOfInterest.end(), id)
			!= pointsOfInterest.end()) {
		cout << "\nNode with ID " << id
				<< " already is selected as point of interest. Try again.\n";
		return -1;
	} else {
		if (id < 0 || id > graph.getNumVertex() || id == sourceID
				|| id == destinyID) {
			cout << "Invalid node. Try again!\n";
			return -1;
		} else {
			vector<Vertex<unsigned> *> v = graph.getVertexSet();
			unsigned dst = v[id]->getInfo();
			vector<unsigned> ret = graph.bfs(v[sourceID]);
			if (find(ret.begin(), ret.end(), dst) == ret.end()) {
				cout << "The point of interest specified is unreachable. Try again\n";
				return -1;
			}
			highlightNode(id, "green");
			pointsOfInterest.push_back(id);
		}
		return 1;
	}
}

int EasyPilot::removePointOfInterest(int id) {
	if(id < 1 || id > pointsOfInterest.size()){
		return -1;
	}else{
		vector<int>::iterator it;
		it = find(pointsOfInterest.begin(), pointsOfInterest.end(), pointsOfInterest[id-1]);
		highlightNode(pointsOfInterest[id-1], "blue");
		pointsOfInterest.erase(it);
		return 1;
	}
}

vector<string> EasyPilot::getPointsOfInterest() const{
	vector<string> ret;
	ret.push_back("");
	ostringstream label;

	for(unsigned int i = 0; i < this->pointsOfInterest.size(); i++){
		label << "Node with id " << pointsOfInterest[i];
		ret.push_back(label.str());
		label.clear();
		label.str("");
	}
	return ret;
}

int EasyPilot::setPOIsNavigation(int method) {
	if (method < 1 || method > 2)
		return -1;

	POIsNavigationMethod = method;
	return 1;
}

int EasyPilot::addInaccessibleZone(int firstID, int lastID)
{
	InaccessibleZone iz = InaccessibleZone(firstID, lastID);
	if(firstID == sourceID || lastID == destinyID)
		return -1;
	else if(find(inaccessibleZones.begin(), inaccessibleZones.end(), iz) != inaccessibleZones.end()){
		return -1;
	}
	else{
		Vertex<unsigned>* v1 = graph.getVertexSet()[firstID];
		Vertex<unsigned>* v2 = graph.getVertexSet()[lastID];
		vector<Edge<unsigned> > adjV1 = v1->getAdj();
		for(unsigned int i = 0; i < adjV1.size(); i++){
			if(adjV1[i].getDest()->getInfo() == v2->getInfo()){
				this->highlightEdge(adjV1[i].getId(), "red", 10);
				graph.setEdgeBlocked(adjV1[i].getId(), true);
				inaccessibleZones.push_back(iz);
				return 1;
			}
		}
	}

	return -1;
}

void EasyPilot::removeInaccessibleZone(int id) {
	InaccessibleZone iz = inaccessibleZones[id - 1];
	vector<InaccessibleZone>::iterator it;
	it = find(inaccessibleZones.begin(),inaccessibleZones.end(), iz);
	Vertex<unsigned>* v1 = graph.getVertexSet()[iz.getFirstID()];
	Vertex<unsigned>* v2 = graph.getVertexSet()[iz.getLastID()];
	vector<Edge<unsigned> > adjV1 = v1->getAdj();
	for (unsigned int i = 0; i < adjV1.size(); i++) {
		if (adjV1[i].getDest()->getInfo() == v2->getInfo()) {
			this->highlightEdge(adjV1[i].getId(), "black",
					DEFAULT_EDGE_THICKNESS);
			graph.setEdgeBlocked(adjV1[i].getId(), false);
			inaccessibleZones.erase(it);
			return;
		}
	}
}

vector<string> EasyPilot::getInaccessibleZones() const{
	vector<string> ret;
	ret.push_back("");
	for(unsigned int i = 0; i < inaccessibleZones.size(); i++){
		ret.push_back(inaccessibleZones[i].toString());
	}
	return ret;
}

/**
 * @brief If apply is TRUE, the toll weights will be applied to the map. Otherwise, they won't.
 * @param bool apply Chooses if weight is applied.
 */
void EasyPilot::setTollWeight(bool apply)
{
	for (unsigned i = 0; i < this->Tolls.size(); i++)
	{
		if (apply)
			this->graph.applyTollWeight(Tolls[i], this->gv);
		else
			this->graph.removeTollWeight(Tolls[i], this->gv);
	}
}

void EasyPilot::allowHighways(bool b)
{
	for (unsigned int i = 0; i < Tolls.size(); i++) {
			int node1ID = Tolls[i].getVertexId();
			vector<Edge<unsigned> > adj = graph.getVertexSet()[node1ID]->getAdj();

			for (unsigned int j = 0; j < adj.size(); j++) {
				if (!b) {
					graph.setEdgeBlocked(adj[j].getId(), true);
					this->highlightEdge(adj[j].getId(), "orange",
					EDGE_THICKNESS);
				} else {
					graph.setEdgeBlocked(adj[j].getId(), false);
					this->highlightEdge(adj[j].getId(), "black",
					DEFAULT_EDGE_THICKNESS);
				}
			}
		}
}

void EasyPilot::getRoadNames(std::map<string, int> &roadsInfo){
	roadsInfo = graph.getEdgesNames();
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
