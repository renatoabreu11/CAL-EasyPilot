#include "EasyPilot.h"

void menuManager()
{
	vector<string> menuPrincipal;
	menuPrincipal.push_back("Menu principal");
	menuPrincipal.push_back("Escolher mapa");
	menuPrincipal.push_back("Sair");

	vector<string> escolherMapa;
	escolherMapa.push_back("Escolher mapa");
	escolherMapa.push_back("Esposende");
	escolherMapa.push_back("Mortosa");
	escolherMapa.push_back("Alpendorada");
	escolherMapa.push_back("Sair");

	EasyPilot gps;

	int select = menuOptions(menuPrincipal);

	while (true) {
		switch (select) {
		case 1:
			select = menuOptions(escolherMapa);

			switch (select) {
			case 1:
				gps.readOSM("Esposende");
				gps.graphInfoToGV();
				break;
			case 2:
				gps.readOSM("Mortosa");
				gps.graphInfoToGV();
				break;
			case 3:
				gps.readOSM("Alpendorada");
				gps.graphInfoToGV();
				break;
			case 4:
				break;
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

	menuManager();

	return 0;
}
