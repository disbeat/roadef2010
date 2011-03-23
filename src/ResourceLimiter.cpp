/*
 * ResourceLimiter.cpp
 *
 *  Created on: 12/Dez/2009
 1*      Author: Ivo
 */

#include "ResourceLimiter.h"

ResourceLimiter::ResourceLimiter() {
	periods = (Period**) malloc(ProblemData::K * sizeof(Period*));
}

ResourceLimiter::~ResourceLimiter() {
	for (int k = 0; k < ProblemData::K; k++)
	{
		if (periods[k] != NULL)
			free(periods[k]);
	}
	free(periods);
}

void ResourceLimiter::setConstraint(Constraint19* ct)
{
	constraint = ct;
}


bool ResourceLimiter::validate(int k, int start)
{
	return constraint->validate(start + periods[k]->getStart(), start + periods[k]->getStart() + periods[k]->getDuration());
}

void ResourceLimiter::add(int k, int start)
{
	constraint->add(start + periods[k]->getStart(), start + periods[k]->getStart() + periods[k]->getDuration());
}

void ResourceLimiter::remove(int k, int start)
{
	constraint->remove(start + periods[k]->getStart(), start + periods[k]->getStart() + periods[k]->getDuration());
}

bool ResourceLimiter::canAdd(int k, int start)
{
	return constraint->canAdd(start + periods[k]->getStart(), start + periods[k]->getStart() + periods[k]->getDuration());
}

Period** ResourceLimiter::getPeriods()
{
	return periods;
}

Constraint19 * ResourceLimiter::getConstraint()
{
	return constraint;
}
