//************************************************************
// Item Class
//************************************************************
#pragma once
#include "Hero.h"
#include <string>
#include <iostream>
using namespace std;

class Hero;


class Item
{
protected:
	Position _pos;
	char _icon;
	int type = 0;

public:
	Item();
	Item(int x, int y);
	Item(const Item& ref);

	// Set position
	void setPos(const Position& pos);
	void setPos(int x, int y);

	// Get position
	Position getPos() const;

	// Get Icon
	char getIcon() const;
	//get type
	int getType(void);
	//virtual function update
	virtual void update(Hero& hero)const = 0;
};

