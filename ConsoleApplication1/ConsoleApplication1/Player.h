#pragma once
#include "Pawn.h"
class Player :
	public Pawn
{
private:
	
	int coinsCollected;
	int playerNextX, playerNextY;
public:
	Player(int, int);
	~Player();

	void Move();

	int getCoinsCollected() {
		return coinsCollected;
	}

	
	void setPlayerNextMove(char);

	void collectCoin();

	char chooseCombat(char p = 'a');
};

