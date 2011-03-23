#pragma once
#include "Constraint.h"
#include <stdio.h>

class Constraint13 :
	public Constraint
{
	int powerplant, campaign, TO, TA;
public:
	Constraint13(FILE* inputFile);
	~Constraint13(void);
	int getPowerplant();
	int getCampaign();
	int getTO();
	int getTA();
};
