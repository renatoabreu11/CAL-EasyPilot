#include "EasyPilot.h"
#include "MenuManager.h"
#include <unistd.h>

int main() {
	EasyPilot gps;
	MenuManager menu;
	menu.mainMenu(&gps);
	return 0;
}
