/*
 * CsvStuff.cpp
 *
 *  Created on: 13/Dez/2009
 *      Author: Ivo
 */

#include "CsvStuff.h"

CsvStuff::CsvStuff()
{
	for(int i=0; i < ProblemData::I; i++)
		costsOfRefuel.push_back(vector<double>(ProblemData::K));

	for(int i=0; i < ProblemData::I; i++)
	{
		savedCostsOfFuel.push_back(vector<double>(ProblemData::S));
		for(int s = 0; s < ProblemData::S; s++)
			savedCostsOfFuel[i][s] = 0.0;
	}

	for(int i=0; i < ProblemData::J; i++)
	{
		costsOfProdution.push_back(vector<double>(ProblemData::S));
		for(int s = 0; s < ProblemData::S; s++)
			costsOfProdution[i][s] = 0.0;
	}
}

CsvStuff::~CsvStuff()
{
}

void CsvStuff::addCostOfRefuel(double cost, int pp2Index, int campaignIndex)
{
	costsOfRefuel[pp2Index][campaignIndex] = cost;
}

void CsvStuff::addSavedCost(double savedCost, int pp2Index, int scenarioIndex)
{
	savedCostsOfFuel[pp2Index][scenarioIndex] += savedCost;
}

void CsvStuff::addCostOfProdution(double cost, int pp1Index, int scenarioIndex)
{
	costsOfProdution[pp1Index][scenarioIndex] += cost;
}

void CsvStuff::writeToCsvFile(char *fileName)
{
	// create file
	FILE* outputFile = fopen(fileName, "w");
	if(outputFile == NULL)
		return;

	// ########## CUSTO DE REFUEL ##########
	fprintf(outputFile, "Custo de refuel por campaign\n\n");
	for(int campaignIndex = 0; campaignIndex < ProblemData::K; campaignIndex++)
		fprintf(outputFile, "; Campaign %d", campaignIndex);
	fprintf(outputFile, "\n");

	for(int pp2Index = 0; pp2Index < ProblemData::I; pp2Index++)
	{
		fprintf(outputFile, "Power Plant %d", pp2Index);
		for(int campaignIndex = 0; campaignIndex < ProblemData::K; campaignIndex++)
		{
			fprintf(outputFile, "; %.2f", costsOfRefuel[pp2Index][campaignIndex]);
		}
		fprintf(outputFile, "\n");
	}

	// ########## CUSTO POUPADO DE FUEL ##########
	fprintf(outputFile, "\n\nCusto poupado de fuel por cenario\n\n");
	for(int scenarioIndex = 0; scenarioIndex < ProblemData::S; scenarioIndex++)
		fprintf(outputFile, "; Scenario %d", scenarioIndex);
	fprintf(outputFile, "\n");

	for(int pp2Index = 0; pp2Index < ProblemData::I; pp2Index++)
	{
		fprintf(outputFile, "Power Plant %d", pp2Index);
		for(int scenarioIndex = 0; scenarioIndex < ProblemData::S; scenarioIndex++)
		{
			fprintf(outputFile, "; %.2f", savedCostsOfFuel[pp2Index][scenarioIndex]);
		}
		fprintf(outputFile, "\n");
	}

	// ########## CUSTO DE PRODUCTION ##########
	fprintf(outputFile, "\n\nCusto de producao por cenario\n\n");
	for(int scenarioIndex = 0; scenarioIndex < ProblemData::S; scenarioIndex++)
		fprintf(outputFile, "; Scenario %d", scenarioIndex);
	fprintf(outputFile, "\n");

	for(int pp1Index = 0; pp1Index < ProblemData::J; pp1Index++)
	{
		fprintf(outputFile, "Power Plant %d", pp1Index);
		for(int scenarioIndex = 0; scenarioIndex < ProblemData::S; scenarioIndex++)
		{
			fprintf(outputFile, "; %.2f", costsOfProdution[pp1Index][scenarioIndex]);
		}
		fprintf(outputFile, "\n");
	}

	fclose(outputFile);
}

