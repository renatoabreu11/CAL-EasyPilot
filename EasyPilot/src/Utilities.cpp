/*
 * Utilities.cpp
 *
 *  Created on: Apr 25, 2016
 *      Author: Renato Abreu
 */

#include "Utilities.h"

InaccessibleZone::InaccessibleZone(int node1, int node2) {
	this->firstID = node1;
	this->lastID = node2;
}

int InaccessibleZone::getFirstID() const{
	return this->firstID;
}

int InaccessibleZone::getLastID() const{
	return this->lastID;
}

bool InaccessibleZone::operator==(const InaccessibleZone &rv) {
	if(this->firstID == rv.firstID && this->lastID == rv.lastID)
		return true;
	else if(this->firstID == rv.lastID && this->lastID == rv.firstID)
		return true;
	else return false;
}

string InaccessibleZone::toString() const {
	ostringstream label;
	label << "Connection with initial node at " << this->firstID << " and end node at " << lastID;
	return label.str();
}
