#pragma once

#include "stdlib.h"
#include "stdio.h"

class Profile
{
	int index;
	int NP;
	int *fPB;
	double *cPB;
public:
	Profile(FILE* inputFile, bool isCurrent, int BO);
	~Profile(void);

	int getNP();
	int getFPB(int i);
	double getCPB(int i);
};
