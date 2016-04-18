/*
 * main.cpp
 */

#include "EasyPilot.h"

int main(){
	EasyPilot * ep = new EasyPilot();
	ep->readOSM("Esposende");
	ep->graphInfoToGV();
}


