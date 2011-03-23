#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "filePatterns.h"
#include "utils.h"
#include <string.h>

class PowerPlant1
{
	char name[NAME_SIZE];
	int index;
	// number of scenarios
	int S;
	// number of timesteps
	int T;
	double **pmin, **pmax, **cost;

	double **productionPlan;

public:
	PowerPlant1(FILE* f, char* name);
	PowerPlant1();
	~PowerPlant1(void);
	double** getProductionPlan();
	double getProduction(int s, int t);
	void addProduction(int s, int t, double value);
	double getCost(int s, int t);
	double getPmin(int s, int t);
	double getPmax(int s, int t);
	char * getName();
	int getIndex();

	void setIndex(int i);
	void setS(int s);
	void setT(int t);
	void setPmin(double **pmin);
	void setPmax(double **pmax);
	void setCost(double **cost);
	void setProduction(int s, int t, double value);

	PowerPlant1 * clone();
};
