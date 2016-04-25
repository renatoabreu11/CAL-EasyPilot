/*
 * Utilities.h
 *
 *  Created on: Apr 25, 2016
 *      Author: Renato Abreu
 */

#ifndef SRC_UTILITIES_H_
#define SRC_UTILITIES_H_
#include "string"
#include "sstream"
using namespace std;

//stores the initial node ID and the last node ID. The edge between those points is inaccessible
class InaccessibleZone{
private:
	int firstID;
	int lastID;
public:
	InaccessibleZone(int node1, int node2);
	virtual ~InaccessibleZone(){ }
	int getFirstID() const;
	int getLastID() const;
	bool operator==(const InaccessibleZone& rv);
	string toString() const;
};

class Link{
public:
	unsigned roadId, node1Id, node2Id;
	Link() : roadId(0), node1Id(0), node2Id(0) {}
	Link(unsigned r, unsigned n1, unsigned n2) :
			roadId(r), node1Id(n1), node2Id(n2) {
	}
};


#endif /* SRC_UTILITIES_H_ */


