#pragma once
#include "Constraint.h"
#include "Outage.h"
#include <stdio.h>
class Constraint21 :
	public Constraint
{
	int start, end;
	double maxPower;
	double currentPower;
public:
	Constraint21();
	~Constraint21(void);

	void setStart(int start);
	void setEnd(int end);
	void setMaxPower(double maxPower);
	int getStart();
	int getEnd();
	bool validate();
	void addPower(double value);

	void add(Outage * outage, double* pmax);
	void remove(Outage * outage, double* pmax);
	bool canAdd(Outage * outage, double* pmax);
};
