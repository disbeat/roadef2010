/*
 * CsvStuff.h
 *
 *  Created on: 13/Dez/2009
 *      Author: Ivo
 */

#ifndef CSVSTUFF_H_
#define CSVSTUFF_H_

#include <vector>
#include "ProblemData.h"

using namespace std;

class CsvStuff
{
	// [PP2][Campaign] => cost of refuel in PP 2 for each campaign
	vector<vector<double> > costsOfRefuel;
	// [PP2][Scenario] => saved costs of fuel in PP 2 for each scenario
	vector<vector<double> > savedCostsOfFuel;
	// [PP1][Scenario] => costs of prodution in PP 1 for each scenario
	vector<vector<double> > costsOfProdution;

public:
	void addCostOfRefuel(double cost, int pp2Index, int campaignIndex);
	void addSavedCost(double savedCost, int pp2Index, int scenarioIndex);
	void addCostOfProdution(double cost, int pp1Index, int scenarioIndex);
	void writeToCsvFile(char *fileName);

	CsvStuff();
	virtual ~CsvStuff();
};

#endif /* CSVSTUFF_H_ */
