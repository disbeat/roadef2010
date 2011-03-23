#pragma once
#include "Constraint.h"
#include <stdio.h>
#include <list>

using namespace std;

class Constraint15 :
	public Constraint
{
	int ID, IF, Se;
	list<int> powerplants;
public:
	Constraint15(int ID, int IF, int Se, list<int> powerplants);
	~Constraint15(void);
	int getID();
	int getIF();
	int getSe();
	list<int>* getPowerplants();
};
