/*
 * ResourceLimiter.h
 *
 *  Created on: 12/Dez/2009
 *      Author: Ivo
 */

#ifndef RESOURCELIMITER_H_
#define RESOURCELIMITER_H_

#include "Constraint19.h"
#include "Period.h"
#include "ProblemData.h"
#include <stdlib.h>

class ResourceLimiter {
	Constraint19 *constraint;
	Period **periods;
public:
	ResourceLimiter();
	virtual ~ResourceLimiter();

	Period** getPeriods();
	void setConstraint(Constraint19* ct);
	Constraint19 * getConstraint();

	bool validate(int k, int start);
	void add(int k, int start);
	void remove(int k, int start);
	bool canAdd(int k, int start);
};

#endif /* RESOURCELIMITER_H_ */
