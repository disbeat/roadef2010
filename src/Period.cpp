/*
 * Period.cpp
 *
 *  Created on: 12/Dez/2009
 *      Author: Ivo
 */

#include "Period.h"

Period::Period(int start, int duration)
{
	this->start = start;
	this->duration = duration;
}

Period::~Period() {
}

int Period::getStart()
{
	return start;
}

int Period::getDuration()
{
	return duration;
}
