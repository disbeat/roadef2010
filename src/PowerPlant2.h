#pragma once

#include "utils.h"
#include "filePatterns.h"
#include "Profile.h"
#include "Outage.h"
#include "PowerOutput.h"
#include "ProblemData.h"
#include "Constraint13.h"
#include "Constraint15.h"
#include "Constraint20.h"
#include "Constraint21.h"
#include "ResourceLimiter.h"
#include <list>
#include <vector>
#include <string.h>
#include <stdlib.h>

class ResourceLimiter;
class PowerOutput;

using namespace std;

class PowerPlant2
{
	public:

	char name[NAME_SIZE];
	int index;
	int T, S;
	int initialStock;
	int K;
	int *DA;
	int ccMMAX;
	int *MMAX, *RMAX, *RMIN, *Q;
	int ccBO;
	int *BO;
	double *pmax;
	int *AMAX, *SMAX;
	double *C;
	double fuelPrice;
	Profile *currentProfile, *profiles;

	double averageCost;

	PowerOutput** outputPlan;

	Outage** outages;
	int numOutages;

	vector<Constraint13*>* ct13;

	// CT 14: array of allowed overlap betwen outages of diferent powerplants
	list<int> *ct14Indexes;
	int *ct14Overlap;

	// CT 15
	list<Constraint15*> * ct15List;
	// CT 16: array of minimum spacing between decoupling dates
	list<int> *decouplingIndexes;
	int *decouplingSpacing;

	// CT 17: array of minimum spacing between coupling dates
	list<int> *couplingIndexes;
	int *couplingSpacing;

	// CT 18: array of minimum spacing between coupling and decoupling dates
	list<int> *ct18Indexes;
	int *ct18Spacing;

	// CT 19
	list<ResourceLimiter*> *ct19List;

	list<Constraint20*> *ct20List;

	list<Constraint21*> *ct21List;



	void allocVars();

	int** weeksAvailable;

	// PUBLIC PART :)


	PowerPlant2(FILE* inputFile, char* name, int T, int S);
	PowerPlant2();
	~PowerPlant2(void);

	PowerPlant2 * clone();

	bool addOutage(Outage *outage);
	bool validateOutageStart(Outage *outage);
	double getFuelPrice();
	PowerOutput** getOutputPlan();

	Outage** getOutages();
	void setOutage(Outage* outage, int k);
	int getK();
	int getQ(int k);
	double* getC();
	double getProduction(int s, int t);
	double getFuelVariation(int s, int t);
	Outage* getOutage(int k);

	void addCt13(Constraint13* ct);
	vector<Constraint13*>* getCt13();
	list<int> * getCt14Indexes();
	int getCt14Overlap(int index);
	void setCt14Overlap(int index, int overlap);

	list<Constraint15*> * getCt15List();
	void addCt15(Constraint15 * ct);

	list<int> * getDecouplingIndexes();
	int getDecouplingSpacing(int index);
	void setDecouplingSpacing(int index, int spacing);

	list<int> * getCouplingIndexes();
	int getCouplingSpacing(int index);
	void setCouplingSpacing(int index, int spacing);

	list<int> * getCt18Indexes();
	int getCt18Spacing(int index);
	void setCt18Spacing(int index, int spacing);

	list<ResourceLimiter*> *getCt19List();
	list<Constraint20*> *getCt20List();
	list<Constraint21*> *getCt21List();

	int* getDA();
	int getDA(int k);
	int getBO(int k);
	int getRMIN(int k);
	int getRMAX(int k);
	int getAMAX(int k);
	int getSMAX(int k);
	int getMMAX(int k);

	double* getPmax();
	double getPmax(int t);
	Profile* getProfile(int k);

	char * getName();
	int getIndex();
	double getInitialStock();

	bool isOnOutage(int w);
	int getCampaign(int w);

	void invalidadeWeeks(int k, int begin, int end);
	void validadeWeeks(int k, int begin, int end);

	bool canOutage(int k, int h);

	double getAverageCost();

	bool addMaxProduction(int s, int t, int k);

	double calculateProfileDecreasingValue(int k, double fuel);
	void setProduction(int s, int t, double value);
	void setFuelVariation(int s, int t, double value);
	void addNullProduction(int s, int t);
	void calculateRemainingFuel(int s);
};
