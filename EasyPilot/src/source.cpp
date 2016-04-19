#include "EasyPilot.h"

void menuManager()
{
	vector<string> menuPrincipal;
	menuPrincipal.push_back("Menu principal");
	menuPrincipal.push_back("Escolher mapa");
	menuPrincipal.push_back("Sair");

	vector<string> escolherMapa;
	escolherMapa.push_back("Escolher mapa");
	escolherMapa.push_back("Porto");
	escolherMapa.push_back("Maia");
	escolherMapa.push_back("Algarve");
	escolherMapa.push_back("Sair");

	int select = menuOptions(menuPrincipal);

	while (true) {
		switch (select) {
		case 1:
			select = menuOptions(escolherMapa);

			switch (select) {
			case 1:
			case 2:
			case 3:
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
	EasyPilot * ep = new EasyPilot();
	ep->readOSM("Esposende");
	ep->graphInfoToGV();
	menuManager();

	return 0;
}
