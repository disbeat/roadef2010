#include "Constraint21.h"
#include "ProblemData.h"
#include "filePatterns.h"
#include "utils.h"

#define TYPE 21

Constraint21::Constraint21()
{
	currentPower = 0;
}

Constraint21::~Constraint21(void)
{
}

void Constraint21::setStart(int start)
{
	this->start = start;
}

void Constraint21::setEnd(int end)
{
	this->end = end;
}

void Constraint21::setMaxPower(double maxPower)
{
	this->maxPower = maxPower;
}

int Constraint21::getStart()
{
	return start;
}

int Constraint21::getEnd()
{
	return end;
}

bool Constraint21::validate()
{
	return currentPower <= maxPower;
}

void Constraint21::addPower(double value)
{
	currentPower += value;
}

void Constraint21::add(Outage * outage, double * pmax)
{
	double sum = 0.0;

	if (outage->getStart() >= start && outage->getStart() <= end)
	{
		int min = MIN(outage->getEnd(), end + 1);
		for (int h = outage->getStart(); h < min; h++)
			for (int t = h*ProblemData::WEEK_SIZE; t < (h+1)*ProblemData::WEEK_SIZE; t++)
				sum += pmax[t];
	}
	else if (outage->getEnd() >= start && outage->getEnd() <= end)
	{
		int min = MIN(outage->getEnd(), end + 1);
		for (int h = outage->getStart(); h < min; h++)
			for (int t = h*ProblemData::WEEK_SIZE; t < (h+1)*ProblemData::WEEK_SIZE; t++)
				sum += pmax[t];

	}
	else if (outage->getStart() < start && outage->getEnd() > end)
	{
		for (int h = start; h < end; h++)
			for (int t = h*ProblemData::WEEK_SIZE; t < (h+1)*ProblemData::WEEK_SIZE; t++)
				sum += pmax[t];
	}


	addPower(sum);

}

void Constraint21::remove(Outage * outage, double * pmax)
{
	double sum = 0.0;

	if (outage->getStart() >= start && outage->getStart() <= end)
	{
		int min = MIN(outage->getEnd(), end + 1);
		for (int h = outage->getStart(); h < min; h++)
			for (int t = h*ProblemData::WEEK_SIZE; t < (h+1)*ProblemData::WEEK_SIZE; t++)
				sum += pmax[t];
	}
	else if (outage->getEnd() >= start && outage->getEnd() <= end)
	{
		int min = MIN(outage->getEnd(), end + 1);
		for (int h = outage->getStart(); h < min; h++)
			for (int t = h*ProblemData::WEEK_SIZE; t < (h+1)*ProblemData::WEEK_SIZE; t++)
				sum += pmax[t];

	}
	else if (outage->getStart() < start && outage->getEnd() > end)
	{
		for (int h = start; h < end; h++)
			for (int t = h*ProblemData::WEEK_SIZE; t < (h+1)*ProblemData::WEEK_SIZE; t++)
				sum += pmax[t];
	}


	currentPower -= sum;

}


bool Constraint21::canAdd(Outage * outage, double * pmax)
{
	double sum = 0.0;

	if (outage->getStart() >= start && outage->getStart() <= end)
	{
		int min = MIN(outage->getEnd(), end+1);
		for (int h = outage->getStart(); h < min; h++)
			for (int t = h*ProblemData::WEEK_SIZE; t < (h+1)*ProblemData::WEEK_SIZE; t++)
				sum += pmax[t];
	}
	else if (outage->getEnd() >= start && outage->getEnd() <= end)
	{
		int min = MIN(outage->getEnd(), end+1);
		for (int h = outage->getStart(); h < min; h++)
			for (int t = h*ProblemData::WEEK_SIZE; t < (h+1)*ProblemData::WEEK_SIZE; t++)
				sum += pmax[t];

	}
	else if (outage->getStart() < start && outage->getEnd() > end)
	{
		for (int h = start; h <= end; h++)
			for (int t = h*ProblemData::WEEK_SIZE; t < (h+1)*ProblemData::WEEK_SIZE; t++)
				sum += pmax[t];
	}


	return currentPower + sum <= maxPower;

}
