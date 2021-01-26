#include "pch.h"
#include "Player.h"
#include <iostream>


Player::Player(int startX, int startY) : Pawn(startX, startY)
{
	coinsCollected = 0;
	symbol = 'P';
}


Player::~Player()
{
}

void Player::Move() {
	// movement logic goes here
	if (getMoveCooldown() > 0) {
		moveCooldown--;
		return;
	}

	if (playerNextX >= 0 && playerNextX <= 25) {
		posX = playerNextX;
	}

	if (playerNextY >= 0 && playerNextY <= 25) {
		posY = playerNextY;
	}
}

void Player::setPlayerNextMove(char input)
{
	switch (input)
	{
	case 'w':
		if (posY == 0 && posX == 12) {
			playerNextX = 12;
			playerNextY = 25;
		}
		else {
			playerNextY = posY - 1;
			playerNextX = posX;
		}
		break;
	case 'a':
		playerNextX = posX - 1;
		playerNextY = posY;
		break;
	case 's':
		if (posY == 25 && posX == 12) {
			playerNextX = 12;
			playerNextY = 0;
		}
		else {
		playerNextY = posY + 1;
		playerNextX = posX;
		}
		break;
	case 'd':
		playerNextX = posX + 1;
		playerNextY = posY;
	}
}

void Player::collectCoin() {
	coinsCollected++;
	health = maxHealth;
}

char Player::chooseCombat(char p)
{
	std::cout << "\nPlease choose to (a)ttack, (t)hrow, or g(uard): ";
	std::cin >> p;

	if (p != 'a' && p != 't' && p != 'g') {
		std::cout << "You did not input a, t, or g so a has been chosen for you.\n";
		p = 'a';
	}

	return p;
}
