#pragma once
#include "constraint.h"
#include <stdio.h>
class Constraint20 :
	public Constraint
{
	int week, max, current;
public:
	Constraint20();
	~Constraint20(void);
	int getWeek();
	int getMax();
	int getCurrent();
	void setWeek(int week);
	void setMax(int max);
	void setCurrent(int current);
	bool canAdd();
	void add();
	void remove();

};
