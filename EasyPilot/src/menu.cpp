#include <vector>
#include <string>
#include <windows.h>
#include <iostream>
#include "menu.h"
using namespace std;

int menuOptions(vector<string> options){
	if (options[0] != "")
		cout << options[0];
	cout << "\n";

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



