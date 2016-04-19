#include "EasyPilot.h"
#include <unistd.h>

void loadMap(string location, EasyPilot &gps)
{
	cout << "Wait for the map to load...\n";

	gps.readOSM(location);
	gps.graphInfoToGV();

	cout << "Press enter to close the map\n";
	cin.ignore();
	gps.eraseMap();
}

void menuManager(EasyPilot *gps)
{
	vector<string> menuPrincipal;
	menuPrincipal.push_back("Menu principal");
	menuPrincipal.push_back("Escolher mapa");
	menuPrincipal.push_back("Sair");

	vector<string> escolherMapa;
	escolherMapa.push_back("Escolher mapa");
	escolherMapa.push_back("Esposende");
	escolherMapa.push_back("Murtosa");
	escolherMapa.push_back("Alpendorada");
	escolherMapa.push_back("Test");
	escolherMapa.push_back("Sair");

	int select = menuOptions(menuPrincipal);

	while (true) {
		switch (select) {
		case 1:
			select = menuOptions(escolherMapa);

			switch (select) {
			case 1:
				loadMap("Esposende", *gps);
				break;
			case 2:
				loadMap("Murtosa", *gps);
				break;
			case 3:
				loadMap("Alpendorada", *gps);
				break;
			case 4:
				loadMap("Test", *gps);
				break;
			default: break;
			}

			break;
		case 2:
			cout << "Bye!\n";
			exit(0);
		}

		select = menuOptions(menuPrincipal);
	}
}

int main() {
	EasyPilot gps;
	menuManager(&gps);

	return 0;
}
