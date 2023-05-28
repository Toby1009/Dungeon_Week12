//************************************************************
// BloodPack.cpp
//************************************************************
#include "BloodPack.h"
//Intent:constructor
//Pre:icon and type
//Pos:constructor
BloodPack::BloodPack()
{
	this->_icon = 'P';
	this->type = 1;
}
//Intent:constructor
//Pre:icon and type
//Pos:constructors
BloodPack::BloodPack(int x, int y) : Item(x,y)
{
	this->_icon = 'P';
	this->type = 1;
}
//Intent:update hero
//Pre:hero
//Pos:update hero
void BloodPack::update(Hero& hero) const
{
	if (hero.getPos().x == _pos.x && hero.getPos().y == _pos.y)
	{
		hero.setHP(hero.getHP() + HP);
	}
}