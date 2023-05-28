//************************************************************
// BloodPack Class that can recover Hero hp
//************************************************************
#pragma once
#include "Item.h"
#include "Position.h"
#include "Hero.h"
#include<string>
#include<iostream>
using namespace std;

class Hero;

class BloodPack : public Item
{
private:
	const int HP = 5;
public:
	BloodPack();
	BloodPack(int x, int y);
	//overrider update function
	virtual void update(Hero& hero) const override;
};