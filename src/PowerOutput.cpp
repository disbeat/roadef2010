#include "PowerOutput.h"

PowerOutput::PowerOutput(int T)
{
	this->T = T;

	this->productionPlan = allocDoubleVector(T);
	this->fuelVariation = allocDoubleVector(T);
}

PowerOutput::~PowerOutput(void)
{
	free(this->productionPlan);
	free(this->fuelVariation);
}


double* PowerOutput::getProductionPlan()
{
	return productionPlan;
}

double PowerOutput::getProduction(int t)
{
	return productionPlan[t];
}

double * PowerOutput::getFuelVariation()
{
	return fuelVariation;
}

double PowerOutput::getFuelVariation(int t)
{
	return fuelVariation[t];
}
double PowerOutput::getRemainingFuel()
{
	return remainingFuel;
}

void PowerOutput::setRemainingFuel(double remainingFuel)
{
	this->remainingFuel = remainingFuel;
}


void PowerOutput::setProduction(int t, double value)
{
	this->productionPlan[t] = value;
}

int PowerOutput::getT()
{
	return T;
}

void PowerOutput::setFuelVariation(int t, double value)
{
	this->fuelVariation[t] = value;
}
