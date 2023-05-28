//************************************************************
// Wolf.cpp
//************************************************************
#include "Wolf.h"

Wolf::Wolf()
{
	this->sPos.x = 2;
	this->sPos.y = 4;
	this->sIcon = 'W';
	this->dispIcon = 'W';
	this->power = 3;
	this->type = 1;
}
Wolf::Wolf(int x, int y)
{
	this->sPos.x = x;
	this->sPos.y = y;
	this->sIcon = 'W';
	this->dispIcon = 'W';
	this->power = 3;
	this->type = 1;
}
