//************************************************************
// Ghost.cpp
//************************************************************
#include "Ghost.h"

Ghost::Ghost()
{
	this->sPos.x = 5;
	this->sPos.y = 8;
	this->sIcon = 'G';
	this->dispIcon = 'G';
	this->power = 6;
	this->type = 2;
}

Ghost::Ghost(int x,int y)
{
	this->sPos.x = x;
	this->sPos.y = y;
	this->sIcon = 'G';
	this->dispIcon = 'G';
	this->power = 6;
	this->type = 2;
}

