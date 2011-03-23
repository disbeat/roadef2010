#include "Constraint20.h"
#include "filePatterns.h"

#define TYPE 20

Constraint20::Constraint20()
{
	this->current = 0;
}

Constraint20::~Constraint20(void)
{
}

int Constraint20::getWeek()
{
	return week;
}

int Constraint20::getMax()
{
	return max;
}

void Constraint20::setWeek(int week)
{
	this->week = week;
}

void Constraint20::setMax(int max)
{
	this->max = max;
}

void Constraint20::setCurrent(int current)
{
	this->current = current;
}

int Constraint20::getCurrent()
{
	return current;
}

bool Constraint20::canAdd()
{
	return max > current;
}

void Constraint20::add()
{
	current++;
}

void Constraint20::remove()
{
	current--;
}
