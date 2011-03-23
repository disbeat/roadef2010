#pragma once
#include <stdio.h>

class Constraint
{

protected:
	int index, type;
public:
	//Constraint(FILE* inputFile);
	Constraint();
	virtual ~Constraint(void);
	void readHeader(FILE* inputFile);
};
