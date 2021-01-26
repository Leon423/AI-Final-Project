#include "pch.h"
#include "Pawn.h"


Pawn::Pawn(int startX, int startY)
{
	maxHealth = 10;
	health = maxHealth;
	posX = startX;
	posY = startY;
	moveCooldown = 0;

}


Pawn::~Pawn()
{
}
