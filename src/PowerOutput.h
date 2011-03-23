#pragma once

#include "utils.h"
#include <stdlib.h>

class PowerOutput
{
	int T;
	double *productionPlan;
	double *fuelVariation;
	double remainingFuel;

public:
	double getRemainingFuel();
	PowerOutput(int T);
	~PowerOutput(void);
	double * getProductionPlan();
	double getProduction(int t);
	double * getFuelVariation();
	double getFuelVariation(int t);
	void setRemainingFuel(double remainingFuel);
	void setProduction(int t, double value);
	void setFuelVariation(int t, double value);

	int getT();
};
