#include "pch.h"
#include "Enemy.h"
#include "ngram_stat.h"
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstring>

Enemy::Enemy(int startX, int startY) : Pawn(startX, startY)
{
	symbol = 'E';
	srand(time(NULL));
	initPat = new char[PAT_SIZE];
	initPatCount = 0;
}


Enemy::~Enemy()
{
}

void Enemy::Move() {
	//enemy movement stuff goes here.

	if (moveCooldown > 0) {
		moveCooldown--;
		return;
	}

	if (xVector.empty() || yVector.empty()) {
		// we have reached our goal so we are good
		return;
	}
	int newX = xVector.back();
	int newY = yVector.back();
	xVector.pop_back();
	yVector.pop_back();

	if (abs(newX - posX) > 1 || abs(newY - posY) > 1) {
		// an error has occured so rip.
		std::cout << "\nTHIS SHOULD NEVER HAPPEN!\n";
		//newX = posX;
		//newY = posY;
	}
	
	posX = newX;
	posY = newY;
}

char Enemy::chooseCombat(char pChoice)
{
	return nGram(pChoice);		//return value of ngram descision
}

void Enemy::clearMoveVectors()
{
	xVector.erase(xVector.begin(), xVector.end());
	yVector.erase(yVector.begin(), yVector.end());
}

void Enemy::addMoveToVector(int x, int y)
{
	xVector.push_back(x);
	yVector.push_back(y);

}

/*
	Builds a memory of previous player choices and attempts to predicate the players next move by looking for patterns
 */
char Enemy::nGram(char pChoice) {
	//init vars for counting th number of instance of the various choices for the current pattern
	int aCount = 0, tCount = 0, gCount = 0, total, randChoice;

	//loop over all the stats in pStats and compare to the current pattern
	//std::cout << "Patterns: ";
	for (ngram_stat * stat : pStats) {
		//std::cout << stat->getPattern() << stat->getChoice() << " ";
		//if the pattern for this stat matches the current pattern
		if (strcmp(stat->getPattern(),pStats.back()->getNextPattern()) == 0) {
			//increment the count for the current stats choice
			switch (stat->getChoice()) {
				case 'a':
					aCount++;
					break;
				case 't':
					tCount++;
					break;
				case 'g':
					gCount++;
					break;
			}
		}
	}
	std::cout << std::endl;
	//sum all the counts
	total = aCount + tCount + gCount;

	if (pStats.empty()) {
		//if the initial pattern is not full, add choice to init array and increment count
		if (initPatCount < PAT_SIZE){
			initPat[initPatCount] = pChoice;
			initPatCount++;

		//if array is full, create a new stat in pstats using the initial pattern and the choice
		} else if (initPatCount = PAT_SIZE) {
			pStats.push_back(new ngram_stat(pChoice,initPat,PAT_SIZE));
		}
	//if pstats not empty then use last stat to get the current pattern and the choice to add another stat
	} else {
		pStats.push_back(new ngram_stat(pChoice,pStats.back()->getNextPattern(),PAT_SIZE));
	}

	//if max number of stats has been exceeded remove the oldest stat
	if (pStats.size() > MEMORY_SIZE-PAT_SIZE)
		pStats.pop_front();

	//if the current pattern was not found, make a choice at random since we have no clue
	if (total == 0) {
		randChoice = rand() % 3;
		switch (randChoice) {
			case 0:
				return 'a';
			case 1:
				return 'g';
			case 2:
				return 't';
		}
	}

	randChoice = rand() % 30; 								//get a random value between 0 and 29

	char choiceToCounter;									//the choice we are betting the player will make next

	//create upper bounds for the choices by multiplying 30 and the percentage the choice represents for this pattern
	//lower bounds are as follows
	//	a: 0
	//	t: abound
	//	g: tbound
	//	bounds are used to weight our choices based on the probabilities of the player's next choice
	int
		aBound = 30 * (aCount / (float)total),				
		tBound = aBound + 30 * (tCount / (float)total),
		gBound = tBound + 30 * (gCount / (float)total);

	//std::cout << aCount << " " << tCount << " " << gCount << std::endl;
	//std::cout << aBound << " " << tBound << " " << gBound << std::endl;
	//choice we are betting to counter is the choice for which the random number falls into its bounds
	if (randChoice < aBound) choiceToCounter = 'a';
	else if (randChoice >= aBound && randChoice < tBound) choiceToCounter = 't';
	else if (randChoice >= tBound && randChoice < gBound) choiceToCounter = 'g';

	//return the move that defeats what we have choosen to counter
	switch (choiceToCounter) {
		case 'a':
			return 'g';
		case 't':
			return 'a';
		case 'g':
			return 't';
	}
}
