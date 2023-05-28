//************************************************************
// Creature.h
//************************************************************
#pragma once
#include "Hero.h"
#include<iostream>
using namespace std;

class Hero;

class Creature {//creature class 
protected:
	Position	sPos;			// Creature position
	char sIcon = 'C';			// Creature icon
	int power;
	char dispIcon = 'C';
	int type = 0;
public:
	// Default constructor
	Creature(void);

	// Set position
	void setPos(Position pos);
	void setPos(int x, int y);

	// Set icon
	void setIcon(char& icon);

	// Get position
	Position getPos(void);

	// Get Icon
	char getIcon(void);

	int getType(void);

public:
	void update(Hero& hero);
};