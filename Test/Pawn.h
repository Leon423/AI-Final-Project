#pragma once
class Pawn
{
protected:
	int health;
	int maxHealth;
	int moveCooldown;
	int posX, posY;
	char symbol;
public:
	Pawn(int, int);
	~Pawn();

	int getHealth() {
		return health;
	}

	int getMoveCooldown() {
		return moveCooldown;
	}

	int getPosX() {
		return posX;
	}

	int getPosY() {
		return posY;
	}

	void TakeDamage() {
		health--;
	}


	char getSymbol() {
		return symbol;
	}

	void setMoveCooldown(int i) {
		moveCooldown = i;
	}

	virtual void Move() = 0;
	virtual char chooseCombat(char) = 0;
};

