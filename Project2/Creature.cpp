//************************************************************
// Creature.cpp
//************************************************************
#include "Creature.h"

// Default constructor
Creature::Creature(void) {
	this->sPos.x = 1;
	this->sPos.y = 1;
	this->sIcon = 'C';
	this->dispIcon = 'C';
	this->power = 5;
};

// Set position
void Creature::setPos(Position pos) { this->sPos = pos; }
void Creature::setPos(int x, int y) {
	this->sPos.x = x;
	this->sPos.y = y;
}

// Set icon
void Creature::setIcon(char& icon) { this->sIcon = icon; }

// Get position
Position Creature::getPos(void) { return this->sPos; }

// Get Icon
char Creature::getIcon(void) { return this->dispIcon; }

int Creature::getType(void) { return this->type; }

void Creature::update(Hero& hero) {
	Position hPos = hero.getPos();

	Position dir;
	if (canSee(this->sPos, hPos, dir)) {
		std::cout << "Creature has find the Hero in the ( " << dir.x << ", " << dir.y << " ) direction\n";
		this->dispIcon = '!';
	}
	else {
		std::cout << "Creature didn't find the Hero\n";
		this->dispIcon = sIcon;
	}

	if (!(hPos != sPos)) {
		hero.damage(power);
	}
}