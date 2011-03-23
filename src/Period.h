/*
 * Period.h
 *
 *  Created on: 12/Dez/2009
 *      Author: Ivo
 */

#ifndef PERIOD_H_
#define PERIOD_H_

class Period {
	int start, duration;
public:
	Period(int start, int duration);
	virtual ~Period();

	int getStart();
	int getDuration();
};

#endif /* PERIOD_H_ */
