#include "Constraint15.h"
#include "filePatterns.h"

#define TYPE 15

Constraint15::Constraint15(int ID, int IF, int Se, list<int> powerplants)
{
	this->ID = ID;
	this->IF = IF;
	this->Se = Se;
	this->powerplants = powerplants;
}

Constraint15::~Constraint15(void)
{
}

int Constraint15::getID()
{
	return ID;
}

int Constraint15::getIF()
{
	return IF;
}


list<int>* Constraint15::getPowerplants()
{
	return &powerplants;
}


int Constraint15::getSe()
{
	return Se;
}
