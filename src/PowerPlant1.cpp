#include "PowerPlant1.h"
#include "utils.h"
#include "ProblemData.h"

PowerPlant1::PowerPlant1(FILE* f, char* name)
{
	strcpy(this->name,name);
	fscanf(f, index_pt, &this->index);
	fscanf(f, scenario_pt, &this->S);
	fscanf(f, timesteps_pt, &this->T);
	this->pmin = allocDoubleMatrix(this->S, this->T);
	this->pmax = allocDoubleMatrix(this->S, this->T);
	this->cost = allocDoubleMatrix(this->S, this->T);
	productionPlan = allocDoubleMatrix(S, T);
	// for each scenario read pmin, pmax and cost for each timestep
	for (int s=0; s < this->S; s++){
		// read all pmins per timestep
		fscanf(f, pmin_pt);
		readDoubleVectorFromFile(f, this->pmin[s], this->T);
		// read all pmaxs per timestep
		fscanf(f, pmax_pt);
		readDoubleVectorFromFile(f, this->pmax[s], this->T);
		// read all costs per timestep
		fscanf(f, cost_pt);
		readDoubleVectorFromFile(f, this->cost[s], this->T);

		for (int t=0; t < ProblemData::T; t++)
			productionPlan[s][t] = 0.0;
	}
}

PowerPlant1::PowerPlant1()
{
	this->pmin = allocDoubleMatrix(ProblemData::S, ProblemData::T);
	this->pmax = allocDoubleMatrix(ProblemData::S, ProblemData::T);
	this->cost = allocDoubleMatrix(ProblemData::S, ProblemData::T);
	productionPlan = allocDoubleMatrix(ProblemData::S, ProblemData::T);
	for (int s=0; s < ProblemData::S; s++)
		for (int t=0; t < ProblemData::T; t++)
			productionPlan[s][t] = 0.0;
}

PowerPlant1 * PowerPlant1::clone()
{
	PowerPlant1 * newPP = new PowerPlant1();
	strcpy(newPP->getName(), name);
	newPP->setIndex(index);
	newPP->setS(S);
	newPP->setT(T);
	newPP->setPmin(pmin);
	newPP->setPmax(pmax);
	newPP->setCost(cost);

//	printf("inner 3\n");
//	fflush(stdout);
	for(int s = 0; s < ProblemData::S; s++)
		for(int t = 0; t < ProblemData::T; t++)
			newPP->setProduction(s, t, getProduction(s, t));

//	printf("inner 4\n");
//	fflush(stdout);
	return newPP;
}

PowerPlant1::~PowerPlant1(void)
{
	for (int index = 0; index < this->S; index++)
	{
		free(pmin[index]);
		free(pmax[index]);
		free(cost[index]);
	}
	free(pmin);
	free(pmax);
	free(cost);
}

void PowerPlant1::setIndex(int i)
{
	index = i;
}

void PowerPlant1::setS(int s)
{
	S = s;
}

void PowerPlant1::setT(int t)
{
	T = t;
}

void PowerPlant1::setPmin(double **pmin)
{
	this->pmin = pmin;
}

void PowerPlant1::setPmax(double **pmax)
{
	this->pmax = pmax;
}

void PowerPlant1::setCost(double **cost)
{
	this->cost = cost;
}

double PowerPlant1::getCost(int s, int t)
{
	return cost[s][t];
}

double PowerPlant1::getProduction(int s, int t)
{
	return productionPlan[s][t];
}

double PowerPlant1::getPmin(int s, int t)
{
	return pmin[s][t];
}

double PowerPlant1::getPmax(int s, int t)
{
	return pmax[s][t];
}

double** PowerPlant1::getProductionPlan()
{
	return productionPlan;
}


char * PowerPlant1::getName()
{
	return name;
}

int PowerPlant1::getIndex()
{
	return index;
}

void PowerPlant1::addProduction(int s, int t, double value)
{
	productionPlan[s][t] += value;
}

void PowerPlant1::setProduction(int s, int t, double value)
{
	productionPlan[s][t] = value;
}
