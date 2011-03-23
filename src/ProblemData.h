#pragma once

#include <cstdlib>
#include <ctime>
#include "utils.h"
#include "Solution.h"
#include "PowerPlant1.h"
#include "PowerPlant2.h"

#define N_CONSTRAINT 9

class Solution;
class PowerPlant1;
class PowerPlant2;


class ProblemData
{
public:
	// number of timesteps
	static int T;
	// number of weeks
	static int H;
	// number of campaigns
	static int K;
	// number of scenarios
	static int S;
	// value of epsilon
	static double E;
	// number of type 1 power plants
	static int J;
	// number of type 2 power plants
	static int I;
	// number of constraints for each type [13 .. 21]
	static int M[N_CONSTRAINT];
	// duration of each timestep t
	static int* D;
	// demand to satisfy for each timestep t and scenario s
	static double** DEM;
	// duration of a week in timesteps
	static int WEEK_SIZE;

	static int maxRunTime;
	static int runTimeMargin;
	static char dataSet[500];

	// sorted type 1 power plants
	static int *** sortedType1PP;
	// current sorting scenario and time step
	static int sortingScenario;
	static int sortingTimeStep;

	// type 1 power plants
	static PowerPlant1 **type1PowerPlants;

	// type 2 power plants
	static PowerPlant2 **type2PowerPlants;


	static void sortType1PowerPlants();
	static void printSortedPowerPlants();
};
