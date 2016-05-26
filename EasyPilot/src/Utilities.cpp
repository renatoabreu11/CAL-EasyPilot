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
int StringAlgorithms::editDistance(string s1, string s2)
{
	// Converting from string to cstring (array of chars)
	const char* cstr1 = s1.c_str();
	const char* cstr2 = s2.c_str();

	int cstr1_len = strlen(cstr1);
	int cstr2_len = strlen(cstr2);

	// If any of the strings is empty
	if(cstr1_len == 0)
		return cstr2_len;
	if(cstr2_len == 0)
		return cstr1_len;

	// Array d[i][j] that'll store the EDIT DISTANCE between s1[i] and s2[j].
	unsigned int distance[cstr1_len+1][cstr2_len+1];	// +1 so it would account for having an empty string

	// Initializing the array
	for (int i = 0; i <= cstr1_len; i++)
	{
		distance[i][0] = i;	// distance of first string to an EMPTY string
	}

	for (int j = 0; j <= cstr1_len; j++)
	{
		distance[0][j] = j;
	}

	// Calculating the distance

	for (int j = 1; j <= cstr2_len; j++)
	{
		for (int i = 1; i <= cstr1_len; i++)
		{
			if (cstr1[i-1] == cstr2[j-1])	// no operation is needed
				distance[i][j] = distance [i-1][j-1];
			else
			{
				distance[i][j] = min(
						distance[i-1][j] + 1, 	// deletion in csrt1 + cost
						min(
						distance[i][j-1] +1,	// insertion in cstr1 + cost
						distance[i-1][j-1] + 1	// substitution in cstr1 + cost
								 ));

			}
		}
	}

	return distance[cstr1_len][cstr2_len];
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
