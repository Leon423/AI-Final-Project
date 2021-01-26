// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

#include <sys/types.h>

#include <fcntl.h>

#pragma warning(disable:4996);

using namespace std;




/*
	prompt for a command
	parse command for conjuction operators and break into individual commands
	loop over individual commands
		call evaluate on the command to eventually be executed
		if conditional conjuction then use return code to decide whether to break from loop
	check for any finished backround children
 */
int main() {
	//master list of children

	while (true) {
		//prompt
		char cmdln[256];
		cout << "What can I do you for? ";
		fgets(cmdln, 256, stdin);
		vector<char*> tokens;

		//tokenize command line
		int i = -1, length = 0;
		char * ptr = strtok(cmdln, " ");
		while (ptr != NULL) {
			tokens.push_back(ptr);
			ptr = strtok(NULL, " ");
		}

		cout << tokens.back();

		//init iterators for parsing tokens
		vector<char*>::iterator
			tmpCmdBegin = tokens.begin(),
			tmpCmdEnd = tokens.end();
	}
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
