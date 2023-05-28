//************************************************************
// Wolf.h
//************************************************************
#include "Creature.h"
#include "Position.h"
#include "Hero.h"
#include <iostream>
using namespace std;
class Wolf :public Creature
{
public:
	Wolf();
	Wolf(int x, int y);
};