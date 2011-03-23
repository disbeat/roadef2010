#include "Constraint13.h"
#include "filePatterns.h"

#define TYPE 13

Constraint13::Constraint13(FILE* inputFile)
{
	readHeader(inputFile);
	fscanf(inputFile, powerplant_pt, &powerplant);
	fscanf(inputFile, campaign_pt, &campaign);
	fscanf(inputFile, earliestStopTime_pt, &TO);
	fscanf(inputFile, latestStopTime_pt, &TA);
	fscanf(inputFile, endConstraint_pt);
}

Constraint13::~Constraint13(void)
{
}

int Constraint13::getCampaign()
{
	return campaign;
}

int Constraint13::getTO()
{
	return TO;
}

int Constraint13::getTA()
{
	return TA;
}
int Constraint13::getPowerplant()
{
	return powerplant;
}
