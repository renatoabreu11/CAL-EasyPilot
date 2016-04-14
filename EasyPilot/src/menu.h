#ifndef MENU_H_
#define MENU_H_
#include <vector>
#include <string>
#include <windows.h>
#include <iostream>
using namespace std;

/**
 * Prints all the options and allows the user to choose one of them
 * @param vector of strings with all options. The first element is the title.
 */
int menuOptions(vector<string> options);

#endif /* MENU_H_ */
