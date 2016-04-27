
#include "MenuManager.h"

namespace std {

MenuManager::MenuManager() {
	// TODO Auto-generated constructor stub

}

MenuManager::~MenuManager() {
	// TODO Auto-generated destructor stub
}

}

void MenuManager::navigationOptions(EasyPilot *gps) {
	bool running = true;
	int input = -1, selection, srcNodeID, destNodeID, pointOfInterestID, POIsNavigationMethod, firstInaccessibleID, lastInaccessibleID, allowHW;
	while (running) {
		input = -1;
		vector<string> options;
		options.push_back("\nChoose:");
		options.push_back("Initial position");
		options.push_back("Destination");
		options.push_back("Add points of interest");
		options.push_back("Remove points of interest");
		options.push_back("Select points of interest navigation method");
		options.push_back("Add inaccessible point");
		options.push_back("Remove inaccessible point");
		options.push_back("Type of route");
		options.push_back("Allow Highways");
		options.push_back("Back");
		selection = menuOptions(options);
		switch (selection) {
		case 1:
			while (input == -1 ) {
				try {
					cout << "\nType the node ID from where to start:\n>>";
					cin >> srcNodeID;
					input = gps->setsourceID(srcNodeID);
				} catch (InvalidInput& e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 2:
			while (input == -1) {
				try {
					cout << "\nType the node ID from where to end:\n>>";
					cin >> destNodeID;
					input = gps->setdestinyID(destNodeID);
				} catch (InvalidInput& e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 3:
			while (input == -1) {
				try {
					cout
							<< "\nType the node ID that you want to pass through:\n>>";
					cin >> pointOfInterestID;
					input = gps->addPointOfInterest(pointOfInterestID);
				} catch (InvalidInput& e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 4:
			while (input == -1) {
				try {
					cout
							<< "\nSelect the node ID that you want to remove from POI's:\n";
					vector<string> ret = gps->getPointsOfInterest();
					ret.push_back("Back");
					pointOfInterestID = menuOptions(ret);
					if(pointOfInterestID != (ret.size() - 1)){
						input = gps->removePointOfInterest(pointOfInterestID);
					}else input = 1;
				} catch (InvalidInput& e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 5:
			while (input == -1) {
				try {
					cout
							<< "\nType 1 for user input order, or 2 for shortest way possible:\n>>";
					cin >> POIsNavigationMethod;
					input = gps->setPOIsNavigation(POIsNavigationMethod);
				} catch (InvalidInput& e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 6:
			while (input == -1) {
				try {
					cout << "\nSelect the connection that is now inaccessible. To do that you must choose two adjacent nodes (connected with an edge).\n";
					cout << "\nType the first node ID\n>>";
					cin >> firstInaccessibleID;
					cout << "Type the last node ID\n>>";
					cin >> lastInaccessibleID;
					input = gps->addInaccessibleZone(firstInaccessibleID, lastInaccessibleID);
					if(input == -1)
						cout << "\nInvalid nodes. Select another inaccessible zone.\n";
				} catch(InvalidInput &e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 7:
			while (input == -1) {
				try {
					cout << "\nType the connection that is no longer inaccessible:";
					vector<string> op = gps->getInaccessibleZones();
					op.push_back("Back");
					input = menuOptions(op);
					if(input != op.size() - 1)
						gps->removeInaccessibleZone(input);
				} catch (InvalidInput& e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 8:
			while (input == -1) {
				try {
					string option;
					cout << "\nDo you want the [f]astest or the [c]heapest route?\n>>";
					getline(cin, option);
					if(option == "f" || option == "F")
					{
						gps->setTollWeight(false);
						input = 0;
					}
					if(option == "c" || option == "C")
					{
						gps->setTollWeight(true);
						input = 0;
					}

					if (input == -1)
						cout << "\nType either 'f' or 'c'.\n";

					gps->updateMap();
				} catch (InvalidInput& e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 9:
			while(input == -1) {
				try {
					cout << "\nType 1 to allow highways, type 2 to block them:\n>>";
					cin >> allowHW;
					input = allowHW;
					if(allowHW == 1)
						gps->allowHighways(true);
					else if(allowHW == 2)
						gps->allowHighways(false);
				} catch (InvalidInput& e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
		case 10:
			running = false;
			break;
		}
	}
}

void MenuManager::navigation(EasyPilot *gps) {
	cout << "\nWait for the map to load...\n";
	cout.flush();

	gps->readOSM();
	gps->graphInfoToGV();

	bool running = true;
	int selection;
	while (running) {
		vector<string> navigation;
		navigation.push_back("\nNavigation");
		navigation.push_back("Path visualization");
		navigation.push_back("Path reset");
		navigation.push_back("Navigation criteria");
		navigation.push_back("Navigation legend");
		navigation.push_back("Back");
		selection = menuOptions(navigation);
		switch (selection) {
		case 1:
			gps->HighLightShortestPath();
			break;
		case 2:
			gps->resetPath();
			break;
		case 3:
			navigationOptions(gps);
			break;
		case 4:
			cout << "\nYellow node -> source or destination point.\n" <<
			"Green node -> point of interest.\n" <<
			"Cyan node -> toll.\n" <<
			"Red edge -> inaccessible zone.\n" <<
			"Pink edge and yellow nodes -> selected path.\n" <<
			"Orange edge -> blocked highway.\n" <<
			"\nPress ENTER to go back...";
			cin.ignore();
			break;
		case 5:
			gps->eraseMap();
			running = false;
			break;
		}
	}
}

void MenuManager::mapSelection(EasyPilot *gps) {
	int selection;
	vector<string> maps;
	maps.push_back("\nMap Selection");
	maps.push_back("Esposende");
	maps.push_back("Murtosa");
	maps.push_back("Alpendorada");
	maps.push_back("Test");
	maps.push_back("Back");
	selection = menuOptions(maps);

	if (selection < 5 && selection >= 1) {
		gps->setMap(maps[selection]);
		cout << "\n" << gps->getMap() << " map is the choosen one.\n";
	} else{
		cout << "\nInvalid Input \n";
		cout << "\n" << gps->getMap() << " map remains the choosen one.\n";
	}
}

void MenuManager::mainMenu(EasyPilot *gps) {
	bool running = true;
	int selection;
	while (running) {
		cout << "\n             EasyPilot                   \n\n";
		vector<string> mainMenu;
		mainMenu.push_back("Main Menu");
		mainMenu.push_back("Navigation");
		mainMenu.push_back("Map Selection");
		mainMenu.push_back("Quit");
		selection = menuOptions(mainMenu);
		switch(selection){
		case 1:
			navigation(gps);
			break;
		case 2:
			mapSelection(gps);
			break;
		case 3:
			cout << "\nBye!\n";
			exit(0);
		}
	}
}

int MenuManager::menuOptions(vector<string> options){
	if (options[0] != "")
		cout << options[0];
	cout << "\n\n";

	for(unsigned int i = 1; i < options.size() ;i++){
		cout << i << " - " << options[i] << endl;
	}

	int index;
	cout << endl;
	while(true)
	{
		cout << ">>";
		cin >> index;
		if (cin.fail()){
			cin.clear(); //clear stream
			cin.ignore(1000, '\n');
			continue;
		}
		cin.clear();
		cin.ignore(1000, '\n');
		return index;
	}
}
