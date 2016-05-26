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

int InaccessibleZone::getFirstID() const {
	return this->firstID;
}

int InaccessibleZone::getLastID() const {
	return this->lastID;
}

bool InaccessibleZone::operator==(const InaccessibleZone &rv) {
	if (this->firstID == rv.firstID && this->lastID == rv.lastID)
		return true;
	else
		return false;
}

string InaccessibleZone::toString() const {
	ostringstream label;
	label << "Connection with initial node at " << this->firstID
			<< " and end node at " << lastID;
	return label.str();
}

Toll::Toll(int vertexID, float cost) {
	this->vertexID = vertexID;
	this->cost = cost;
	this->weightAdd = round(100 * cost);
	this->applied = false;
}

float Toll::getCost() const {
	return cost;
}

int Toll::getVertexId() const {
	return vertexID;
}

int Toll::getWeightAdd() const {
	return weightAdd;
}

/**
 * @fn editDistance
 * @brief Calculates the edit (Levenshtein) distance between two strings. This is the number of deletions, substitutions and/or insertions needed
 * to make one string match the other.	It implements Wagner-Fischer dynamic algorithm.
 * @param s1 First string.
 * @param s2 Second string.
 */
int StringAlgorithms::editDistance(string pattern, string text) {
	int n = text.length();
	vector<int> d(n + 1);
	int old, neww;
	for (int j = 0; j <= n; j++)
		d[j] = j;
	int m = pattern.length();
	for (int i = 1; i <= m; i++) {
		old = d[0];
		d[0] = i;
		for (int j = 1; j <= n; j++) {
			if (pattern[i - 1] == text[j - 1])
				neww = old;
			else {
				neww = min(old, d[j]);
				neww = min(neww, d[j - 1]);
				neww = neww + 1;
			}
			old = d[j];
			d[j] = neww;
		}
	}
	return d[n];

}

bool StringAlgorithms::kmd(vector<string> toCompare, string toSearch) {
	int numRepetido = 0;
	int i, j;
	int pi[toSearch.length()];
	pi[0] = 0;

	//Pre processamento
	for (i = 1, j = 0; i < toSearch.length(); i++) {
		if (toSearch[j] == toSearch[i]) {
			pi[i] = j + 1;
			j++;
		} else {
			while (toSearch[j] != toSearch[i]) {
				if (j == 0) {
					pi[i] = 0;
					break;
				}
				j = pi[j - 1];
			}
			if (toSearch[j] == toSearch[i])
				pi[i] = j + 1;
		}
	}

	for(int k = 0; k < toCompare.size(); k++) {
		j = 0;
		for (i = 0; i < toCompare[k].length(); i++) {
			if (j == toSearch.length() - 1)
				return true;
			if (toCompare[k][i] == toSearch[j])
				j++;
			else if (j != 0)
				j = pi[j - 1];
		}
	}

	return false;
}
