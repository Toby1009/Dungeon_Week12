//************************************************************
// Trigger.cpp
//************************************************************
#include "Trigger.h"
#include "Item.h"

//Intent:constructor
//Pre:icon and type
//Pos:constructor
Trigger::Trigger() :_exp(10)
{
	this->_icon = 'T';
	this->type = 0;
}
//Intent:constructor
//Pre:icon and type
//Pos:constructor
Trigger::Trigger(int x, int y) :_exp(10),Item(x,y)
{
	this->_icon = 'T';
	this->type = 0;
}
//Intent:update hero
//Pre:hero
//Pos:update hero
void Trigger::update(Hero &hero)const
{
	if (hero.getPos().x == _pos.x && hero.getPos().y == _pos.y)
	{
		hero.gainEXP(_exp);
	}
}
//Intent:get exp amount
//Pre:exp
//Pos:get exp amount
int Trigger::getExpAmount() const 
{ 
	return this->_exp;
}