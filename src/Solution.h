#pragma once

#include <list>
#include <vector>
#include <stdio.h>
#include "PowerPlant1.h"
#include "PowerPlant2.h"
#include "Outage.h"
#include "CsvStuff.h"
#include "filePatterns.h"
#include "utils.h"

//LIXO: to compile
#define MAX_CONCURRENT_OUTAGES 2343
#define MAX_STARTING_OUTAGES 2343

class PowerPlant1;
class PowerPlant2;
class CsvStuff;

using namespace std;

class Solution
{
	list<Outage*> outages;
	int *outageValidator;

	CsvStuff *csvStuff;
	double ** totalProduction;

public:

	vector<PowerPlant1*> *type1PowerPlants;
	vector<PowerPlant2*> *type2PowerPlants;

	Solution();
	~Solution(void);

	void copySolution(Solution *source);

	bool addOutage(Outage *outage);
	bool canAdd(Outage *outage);
	void removeOutage(Outage *outage);

	void addProduction(int s, int t, double value);
	void removeProduction(int s, int t, double value);


	bool checkCT1();
	bool checkCT2();
	bool checkCT3();
	bool checkCT4();
	bool checkCT5();
	bool checkCT4and5();
	bool checkCT6();
	bool checkCT7();
	bool checkCT8();
	bool checkCT9();
	bool checkCT10();
	bool checkCT11();
	bool checkCT12();
	bool checkCT13();
	bool checkCT14();
	bool checkCT15();
	bool checkCT16();
	bool checkCT17();
	bool checkCT18();
	bool checkCT19();
	bool checkCT20();
	bool checkCT21();

	double calculateProfileDecreasingValue(int i, int k, double fuel);

	double calculateObjective();
	double calculateRefuelCost();
	double calculateRemainingCost(int s);
	double calculateTotalProductionCost(int s);
	double calculatePowerPlantProductionCost(int s, int j);

	double getTotalProduction(int s, int t);

	void addType1PowerPlant(PowerPlant1* pp, int j);
	void addType2PowerPlant(PowerPlant2* pp, int i);


	void writeSolution(char *fileName, int teamID);

	void writeToCsvFile(char *fileName);

	PowerPlant1 * getType1PowerPlant(int plantIndex);

	bool addType1PowerPlantsProduction();
	int findAvailableWeekToOutage(int h, PowerPlant2 * ppt, int k);
};
