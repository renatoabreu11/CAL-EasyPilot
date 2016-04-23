
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
	int input = -1, selection, srcNodeID, destNodeID, pointOfInterestID, inaccessibleID;
	while (running) {
		input = -1;
		vector<string> options;
		options.push_back("\nChoose:");
		options.push_back("Initial position");
		options.push_back("Destination");
		options.push_back("Add points of interest");
		options.push_back("Remove points of interest");
		options.push_back("Add inaccessible point");
		options.push_back("Remove inaccessible point");
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
			while (input == -1 || input == 0) {
				try {
					cout
							<< "\nType the node ID that you want to pass through:\n>>";
					cin >> pointOfInterestID;
					input = gps->addPointOfInterest(pointOfInterestID);
					if (input == 0) {
						cout << "\nNode with ID " << pointOfInterestID
								<< " already is selected as point of interest. Try again.\n";
					}
				} catch (InvalidInput& e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 4:
			while (input == -1 || input == 0) {
				try {
					cout
							<< "\nType the node ID that you want to remove from POI's:\n>>";
					cin >> pointOfInterestID;
					input = gps->removePointOfInterest(pointOfInterestID);
					if (input == 0) {
						cout << "\nNode with ID " << pointOfInterestID
								<< " is not selected as point of interest. Try again.\n";
					}
				} catch (InvalidInput& e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 5:
			while (input == -1 || input == 0) {
				try {
					cout << "\nType the node ID that is uneccessible: \n";
					cin >> inaccessibleID;
					input = gps->addInaccessibleZone(inaccessibleID);
					if(input == -1 || input == 0)
						cout << "\nNode with ID " << inaccessibleID << " is not selected as inaccessible point. Try again.\n";
				} catch(InvalidInput &e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 6:
			while (input == -1 || input == 0) {
				try {
					cout << "\nType the node ID that is no longer inaccessible:\n>>";
					cin >> inaccessibleID;
					input = gps->removeInaccessibleZone(inaccessibleID);
					if (input == 0) {
						cout << "\nNode with ID " << inaccessibleID << " is not selected as inaccessible point. Try again.\n";
					}
				} catch (InvalidInput& e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 7:
			running = false;
			break;
			/**
			 * outras opções para estas merdinhas
			 * outros critérios a utilizar poderão incluir menor distância,
			 * menor tempo de viagem, e ainda a existência ou não de pontos de portagem.
			 */
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
