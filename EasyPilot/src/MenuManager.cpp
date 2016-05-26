#include "MenuManager.h"

namespace std {

MenuManager::MenuManager() {
	mapSelecOption = 1;
	destSelecOption = 1;

}

MenuManager::~MenuManager() {
	// TODO Auto-generated destructor stub
}

}

void MenuManager::navigationOptions(EasyPilot *gps) {
	bool running = true;
	int input = -1, selection, srcNodeID, destNodeID, pointOfInterestID,
			POIsNavigationMethod, firstInaccessibleID, lastInaccessibleID,
			allowHW;
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
		options.push_back("Destination options");
		options.push_back("Back");
		selection = menuOptions(options);
		switch (selection) {
		case 1:
			while (input == -1) {
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
			if (destSelecOption == 1) {
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
			} else if (destSelecOption == 2) {
				ApproximateDestSelection(gps);
				break;
			} else if (destSelecOption == 3) {
				ExactDestSelection(gps);
				break;
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
					if (pointOfInterestID != (ret.size() - 1)) {
						input = gps->removePointOfInterest(pointOfInterestID);
					} else
						input = 1;
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
					cout
							<< "\nSelect the connection that is now inaccessible. To do that you must choose two adjacent nodes (connected with an edge).\n";
					cout << "\nType the first node ID\n>>";
					cin >> firstInaccessibleID;
					cout << "Type the last node ID\n>>";
					cin >> lastInaccessibleID;
					input = gps->addInaccessibleZone(firstInaccessibleID,
							lastInaccessibleID);
					if (input == -1)
						cout
								<< "\nInvalid nodes. Select another inaccessible zone.\n";
				} catch (InvalidInput &e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 7:
			while (input == -1) {
				try {
					cout
							<< "\nType the connection that is no longer inaccessible:";
					vector<string> op = gps->getInaccessibleZones();
					op.push_back("Back");
					input = menuOptions(op);
					if (input != op.size() - 1)
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
					cout
							<< "\nDo you want the [f]astest or the [c]heapest route?\n>>";
					getline(cin, option);
					if (option == "f" || option == "F") {
						gps->setTollWeight(false);
						input = 0;
					}
					if (option == "c" || option == "C") {
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
			while (input == -1) {
				try {
					cout
							<< "\nType 1 to allow highways, type 2 to block them:\n>>";
					cin >> allowHW;
					input = allowHW;
					if (allowHW == 1)
						gps->allowHighways(true);
					else if (allowHW == 2)
						gps->allowHighways(false);
				} catch (InvalidInput& e) {
					cout << "\nInvalid Input. Try again";
					cin.clear();
					cin.ignore(1000, '\n');
				}
			}
			break;
		case 10: {
			int selection;
			bool running = true;
			while (running) {
				vector<string> options;
				options.push_back("\nDestination selection options:");
				options.push_back("Default selection");
				options.push_back("Approximate string matching");
				options.push_back("Exact string matching");
				options.push_back("Back");
				selection = menuOptions(options);
				switch (selection) {
				case 1:
					destSelecOption = 1;
					cout
							<< "\nDefault selection is now the current destination selection.\n";
					running = false;
					break;
				case 2:
					destSelecOption = 2;
					cout
							<< "\nApproximate string matching is now the current destination selection.\n";
					running = false;
					break;
				case 3:
					destSelecOption = 3;
					cout
							<< "\nExact string matching is now the current destination selection.\n";
					running = false;
					break;
				case 4:
					running = false;
					break;
				}
			}
		}
			break;
		case 11:
			running = false;
			break;
		}
	}
}

void MenuManager::ApproximateDestSelection(EasyPilot *gps) {
	string input;
	cout << "\nType the desired destination:\n>> ";
	cin >> input;
	bool destSelected = false;

	StringAlgorithms *algorithm = new StringAlgorithms();
	vector<string> suggestions;
	suggestions.push_back("\n Destination Suggestions:");
	map<int, string, std::less<int> > destinationsDist;
	map<string, int> roads;
	gps->getRoadNames(roads);

	int dist = 0;
	for (map<string, int>::iterator it = roads.begin(); it != roads.end(); ++it) {
		dist = algorithm->editDistance(input, (*it).first);
		if (dist < 8) {
			destinationsDist.insert(std::pair<int, string>(dist, (*it).first));
		}
	}

	int id = 0;
	for (map<int, string>::iterator it = destinationsDist.begin();
			it != destinationsDist.end(); ++it) {
		if ((*it).first == 0) {
			id = roads.find((*it).second)->second;
			cout << "\n" << (*it).second
					<< " road with ID " << id << " is the choosen one.\n";
			gps->setdestinyID(id);
			destSelected = true;
		} else {
			suggestions.push_back((*it).second);
		}
	}

	if (!destSelected) {
		if (suggestions.size() == 1) {
			cout
					<< "\n There are no suggestions similar to the written road.\n";
		} else {
			suggestions.push_back("Back");
			int selection = 0;
			selection = menuOptions(suggestions);
			if (selection == suggestions.size() - 1) {
				cout << "\n" << gps->getdestinyID()
						<< " node remains the choosen one.\n";
			} else {
				id = roads.find(suggestions[selection])->second;
				gps->setdestinyID(id);
				cout << "\n" << suggestions[selection]
									<< " road with ID " << id << " is the choosen one.\n";
			}
		}
	}
}

void MenuManager::ExactDestSelection(EasyPilot *gps) {
	string typedRoadName;
	map<string, int> roadInfo;
	vector<string> roadNames;

	StringAlgorithms *algorithm = new StringAlgorithms();
	gps->getRoadNames(roadInfo);

	map<string, int>::iterator it;
	for (it = roadInfo.begin(); it != roadInfo.end(); it++)
		roadNames.push_back(it->first);

	cout << "\nType the desired road name:\n>>";
	cin >> typedRoadName;

	if (algorithm->kmd(roadNames, typedRoadName)) {
		it = roadInfo.find(typedRoadName);
		if (it == roadInfo.end())
			cout << "Falhou!\n";
		else {
			gps->setsourceID(it->second);
			cout << "You've set the origin to '" << it->first
					<< "' road, node --> " << it->second << endl;
		}
	} else
		cout << "Couldn't find '" << typedRoadName
				<< "' road, please try again\n";

}

void MenuManager::ApproximateDistrictSelection(EasyPilot *gps) {
	string input;
	cout << "\nType the desired district:\n>> ";
	cin >> input;
	bool mapSelected = false;

	StringAlgorithms *algorithm = new StringAlgorithms();
	vector<string> suggestions;
	suggestions.push_back("\n District Suggestions:");
	map<int, string, std::less<int> > districtsDist;
	districtsDist.insert(
			std::pair<int, string>(algorithm->editDistance(input, "Esposende"),
					"Esposende"));
	districtsDist.insert(
			std::pair<int, string>(
					algorithm->editDistance(input, "Alpendorada"),
					"Alpendorada"));
	districtsDist.insert(
			std::pair<int, string>(algorithm->editDistance(input, "Murtosa"),
					"Murtosa"));



	for (map<int, string>::iterator it = districtsDist.begin();
			it != districtsDist.end(); ++it) {
		if((*it).first == 0){
			cout << "\n" << (*it).second  << " map is the choosen one.\n";
			mapSelected = true;
		}
		else{
			if((*it).first <= 3){
				suggestions.push_back((*it).second);
			}
		}
	}

	if(!mapSelected){
		if(suggestions.size() == 1){
			cout << "\n There are no suggestions similar to the written district.\n";
		}
		else {
			suggestions.push_back("Back");
			int selection = 0;
			selection = menuOptions(suggestions);
			if (selection == suggestions.size() - 1) {
				cout << "\n" << gps->getMap()
						<< " map remains the choosen one.\n";
			} else {
				gps->setMap(suggestions[selection]);
				cout << "\n" << gps->getMap()
						<< " map is the choosen one.\n";
			}
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
			cout << "\nYellow node -> source or destination point.\n"
					<< "Green node -> point of interest.\n"
					<< "Cyan node -> toll.\n"
					<< "Red edge -> inaccessible zone.\n"
					<< "Pink edge and yellow nodes -> selected path.\n"
					<< "Orange edge -> blocked highway.\n"
					<< "\nPress ENTER to go back...";
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
	switch (mapSelecOption) {
	case 1: {
		int selection;
		vector<string> maps;
		maps.push_back("\nMap Selection");
		maps.push_back("Esposende");
		maps.push_back("Murtosa");
		maps.push_back("Alpendorada");
		maps.push_back("Back");
		selection = menuOptions(maps);

		if (selection < 4 && selection >= 1) {
			gps->setMap(maps[selection]);
			cout << "\n" << gps->getMap() << " map is the choosen one.\n";
		} else {
			cout << "\nInvalid Input \n";
			cout << "\n" << gps->getMap() << " map remains the choosen one.\n";
		}
		break;
	}
	case 2: {
		ApproximateDistrictSelection(gps);
		break;
	}
	case 3: {
		ExactDistrictSelection(gps);
		break;
	}
	}
}

void MenuManager::ExactDistrictSelection(EasyPilot *gps) {
	string typedDistrict;
	vector<string> districts;

	districts.push_back("Esposende");
	districts.push_back("Murtosa");
	districts.push_back("Alpendorada");

	cout << "\nType the desired district:\n>> ";
	cin >> typedDistrict;
	StringAlgorithms *algorithm = new StringAlgorithms();
	if (algorithm->kmd(districts, typedDistrict)) {
		gps->setMap(typedDistrict);
		cout << "\n" << gps->getMap() << " map is the choosen one.\n";
	} else
		cout << "There's no district named '" << typedDistrict << "' in the database...\n";
}

void MenuManager::inputOptions(EasyPilot *gps) {
	int selection;
	bool running = true;
	while (running) {
		vector<string> options;
		options.push_back("\nMap selection options:");
		options.push_back("Default selection"); //arranjem um nome melhor
		options.push_back("Approximate string matching");
		options.push_back("Exact string matching");
		options.push_back("Back");
		selection = menuOptions(options);
		switch (selection) {
		case 1:
			mapSelecOption = 1;
			cout
					<< "\nDefault map selection is now the current district selection.\n";
			running = false;
			break;
		case 2:
			mapSelecOption = 2;
			cout
					<< "\nApproximate string matching is now the current district selection.\n";
			running = false;
			break;
		case 3:
			mapSelecOption = 3;
			cout
					<< "\nExact string matching is now the current district selection.\n";
			running = false;
			break;
		case 4:
			running = false;
			break;
		}
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
		mainMenu.push_back("Input Options");
		mainMenu.push_back("Quit");
		selection = menuOptions(mainMenu);
		switch (selection) {
		case 1:
			navigation(gps);
			break;
		case 2:
			mapSelection(gps);
			break;
		case 3:
			inputOptions(gps);
			break;
		case 4:
			cout << "\nBye!\n";
			exit(0);
		}
	}
}

int MenuManager::menuOptions(vector<string> options) {
	if (options[0] != "")
		cout << options[0];
	cout << "\n\n";

	for (unsigned int i = 1; i < options.size(); i++) {
		cout << i << " - " << options[i] << endl;
	}

	int index;
	cout << endl;
	while (true) {
		cout << ">>";
		cin >> index;
		if (cin.fail()) {
			cin.clear(); //clear stream
			cin.ignore(1000, '\n');
			continue;
		}
		cin.clear();
		cin.ignore(1000, '\n');
		return index;
	}
}
