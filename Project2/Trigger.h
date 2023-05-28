//************************************************************
// Trigger Class that can recover Hero hp
//************************************************************
#pragma once
#include "Position.h"
#include "Hero.h"
#include "Item.h"
#include<string>
#include<iostream>
using namespace std;

class Hero;

class Trigger : public Item
{
private:
	int _exp = 5;
public:
	Trigger();
	Trigger(int x, int y);
	//get ExpAmount
	int getExpAmount() const;
	//overrider update function
	virtual void update(Hero& hero) const override;
};