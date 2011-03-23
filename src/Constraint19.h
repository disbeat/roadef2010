#pragma once
#include "constraint.h"
#include <stdio.h>

class Constraint19 :
	public Constraint
{
	int Q;
	// set not needed
	int *consumedResource;
public:
	Constraint19(FILE* inputFile);
	~Constraint19(void);
	int getQ();
	void setQ(int Q);

	bool validate(int start, int end);

	void add(int start, int end);
	void remove(int start, int end);
	bool canAdd(int start, int end);
};
