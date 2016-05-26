#ifndef SRC_MENUMANAGER_H_
#define SRC_MENUMANAGER_H_

#include <vector>
#include <string>
#include <windows.h>
#include <iostream>
#include "Utilities.h"
#include "EasyPilot.h"

namespace std {

class MenuManager {
private:
	int mapSelecOption;
	int destSelecOption;
public:
	MenuManager();
	virtual ~MenuManager();
	void navigationOptions(EasyPilot *gps);
	void navigation(EasyPilot *gps);
	void mapSelection(EasyPilot *gps);
	void mainMenu(EasyPilot *gps);
	void ApproximateDistrictSelection(EasyPilot *gps);
	void ExactDistrictSelection(EasyPilot *gps);
	void ApproximateDestSelection(EasyPilot *gps);
	void ExactDestSelection(EasyPilot *gps);
	void inputOptions(EasyPilot *gps);
	int menuOptions(vector<string> options);
};

class InvalidInput{
public:
    InvalidInput(){};
};

} /* namespace std */

#endif /* SRC_MENUMANAGER_H_ */
