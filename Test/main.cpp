#include "Enemy.h"
#include "ngram_stat.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main () {
	srand(time(NULL));
	Enemy en(5,5);
	/*for (int i =0; i < 11; i++) {
		int random = rand() % 3;
		if (random % 3 == 0) {cout << "g\n";en.chooseCombat('g');}
		else if (random % 3 == 1) { cout << "a\n";en.chooseCombat('a');}
		else { cout << "t\n";en.chooseCombat('t');}
	}*/
	en.chooseCombat('a');
	en.chooseCombat('t');
	en.chooseCombat('g');
	en.chooseCombat('a');
	en.chooseCombat('t');
	en.chooseCombat('g');
	en.chooseCombat('a');
	en.chooseCombat('t');
	en.chooseCombat('g');
	en.chooseCombat('a');
	en.chooseCombat('t');
	cout << en.chooseCombat('a') << endl;
}
