#pragma once
#include "Pawn.h"
#include "ngram_stat.h"
#include <vector>
#include <list>
#include <stddef.h>

class Enemy :
	public Pawn
{
private:
	bool inCombat;
	std::vector<int> xVector;
	std::vector<int> yVector;
	//n-grams stuff
	const size_t MEMORY_SIZE = 10; 	//max number of choices to remember
	const size_t PAT_SIZE = 2;		//length of patterns to evaluate
	std::list<ngram_stat*> pStats;	//list of pattern stats
	char * initPat;					//pointer to a char, used to temporarily store patterns until the first patStat is generated
	int initPatCount;				//count of char in the initPat array
	char nGram(char pChoice);		//reserved for internal use only
public:
	Enemy(int startX, int startY);
	~Enemy();
	void Move();
	char chooseCombat(char pChoice);
	bool isInCombat() {
		return inCombat;
	}
	void Respawn(int newX, int newY) {
		posX = newX;
		posY = newY;
		health = maxHealth;
	}
	// clears the vector when we need to add a new path.
	void clearMoveVectors();
	// called from finalPath() in main application. Sends the moves starting at the goal and working backwords.
	// this means that the back of the vector will always be the next move we want.
	void addMoveToVector(int x, int y);

};

