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

#endif /* SRC_UTILITIES_H_ */
