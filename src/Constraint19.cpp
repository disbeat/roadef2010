#include "Constraint19.h"
#include "filePatterns.h"
#include "stdlib.h"
#include "ProblemData.h"

using namespace std;

#define TYPE 19

Constraint19::Constraint19(FILE* inputFile)
{
	readHeader(inputFile);
	consumedResource = (int*)malloc(ProblemData::H * sizeof(int));
	for(int h=0; h<ProblemData::H; h++)
		consumedResource[h] = 0;
}

Constraint19::~Constraint19(void)
{
	free(consumedResource);
}

int Constraint19::getQ()
{
	return Q;
}

void Constraint19::setQ(int Q)
{
	this->Q = Q;
}


bool Constraint19::validate(int start, int end)
{
	for (int h = start; h < end; h++)
	{
		if (consumedResource[h] > Q)
			return false;
	}
	return true;
}

void Constraint19::add(int start, int end)
{
	for (int h = start; h < end; h++)
		consumedResource[h]++;
}

void Constraint19::remove(int start, int end)
{
	for (int h = start; h < end; h++)
		consumedResource[h]--;
}

bool Constraint19::canAdd(int start, int end)
{
	for (int h = start; h < end; h++)
	{
		if (consumedResource[h] >= Q)
			return false;
	}
	return true;
}
