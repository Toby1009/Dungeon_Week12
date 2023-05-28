//************************************************************
// Item Class
//************************************************************
#include"Item.h"

Item::Item()
{
	_icon = ' ';
}
Item::Item(int x, int y)
{
	_pos.x = x;
	_pos.y = y;
	_icon = ' ';
}
Item::Item(const Item& ref)
{
	*this = ref;
}

// Set position
void Item::setPos(const Position& pos)
{ 
	this->_pos = pos; 
}
void Item::setPos(int x, int y)
{
	this->_pos.x = x;
	this->_pos.y = y;
}

// Get position
Position Item::getPos() const
{
	return this->_pos;
}

// Get Icon
char Item::getIcon() const
{
	return this->_icon; 
}
// Get Type
int Item::getType(void)
{
	return this->type;
}