#ifndef SRC_MENUMANAGER_H_
#define SRC_MENUMANAGER_H_

#include <vector>
#include <string>
#include <windows.h>
#include <iostream>
#include "EasyPilot.h"

namespace std {

class MenuManager {
public:
	MenuManager();
	virtual ~MenuManager();
	void navigationOptions(EasyPilot *gps);
	void navigation(EasyPilot *gps);
	void mapSelection(EasyPilot *gps);
	void mainMenu(EasyPilot *gps);
	int menuOptions(vector<string> options);
};

class InvalidInput{
public:
    InvalidInput(){};
};

} /* namespace std */

#endif /* SRC_MENUMANAGER_H_ */
