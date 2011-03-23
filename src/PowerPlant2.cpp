#include "PowerPlant2.h"

void PowerPlant2::allocVars()
{
	K = ProblemData::K;
	S = ProblemData::S;
	T = ProblemData::T;

	DA = allocIntVector( ProblemData::K );
	MMAX = allocIntVector( ProblemData::K );
	RMAX = allocIntVector( ProblemData::K );
	RMIN = allocIntVector( ProblemData::K );
	Q = allocIntVector( ProblemData::K );
	BO = allocIntVector( ProblemData::K + 1 );
	pmax = allocDoubleVector( ProblemData::T );
	AMAX = allocIntVector( ProblemData::K );
	SMAX  = allocIntVector( ProblemData::K );
	C = allocDoubleVector( ProblemData::K );

	ct13 = new vector<Constraint13*>(ProblemData::K);
	for (int i=0; i<ProblemData::K; i++)
		(*ct13)[i] = NULL;

	ct14Overlap = allocIntVector(ProblemData::I);
	//initialization of overlap as -H

	for (int i = 0; i < ProblemData::I; i++)
		ct14Overlap[i] = - ProblemData::H;
	ct14Indexes = new list<int>();

	ct15List = new list<Constraint15*>();

	decouplingSpacing = allocIntVector(ProblemData::J);
	//initialization of spacing as 0

	for (int i = 0; i < ProblemData::J; i++)
		decouplingSpacing[i] = 0;
	decouplingIndexes = new list<int>();


	couplingSpacing = allocIntVector(ProblemData::J);
	//initialization of spacing as 0

	for (int i = 0; i < ProblemData::J; i++)
		couplingSpacing[i] = 0;
	couplingIndexes = new list<int>();

	ct18Spacing = allocIntVector(ProblemData::J);
	//initialization of spacing as 0

	for (int i = 0; i < ProblemData::J; i++)
		ct18Spacing[i] = 0;
	ct18Indexes = new list<int>();

	ct19List = new list<ResourceLimiter*>();
	ct20List = new list<Constraint20*>();
	ct21List = new list<Constraint21*>();

	outages = (Outage**) malloc(ProblemData::K *  sizeof(Outage*));
	for(int i=0; i < ProblemData::K; i++)
		outages[i] = NULL;


	outputPlan = (PowerOutput**) malloc(ProblemData::S *  sizeof(PowerOutput*));
	for (int s = 0; s < ProblemData::S; s++)
		outputPlan[s] = new PowerOutput(ProblemData::T);

	profiles = (Profile *) malloc(ProblemData::K * sizeof(Profile));

	weeksAvailable = allocIntMatrix(ProblemData::K, ProblemData::H);

	for ( int k = 0; k < ProblemData::K; k++)
		for ( int h = 0; h < ProblemData::H; h++ )
			weeksAvailable[k][h] = 1;

	Outage * outage;
	for (int k =0; k < ProblemData::K; k++)
	{
		outage = new Outage(-1,0,0,this->index, k);
		outages[k] = outage;
	}

}

PowerPlant2::PowerPlant2(FILE* inputFile, char* name, int T, int S)
{
	numOutages = 0;
	this->T = T;
	this->S = S;

	strcpy(this->name, name );
	fscanf(inputFile, index_pt, &this->index);
	fscanf(inputFile, stock_pt, &this->initialStock);
	fscanf(inputFile, campaigns_pt, &this->K);

	this->allocVars();

	fscanf(inputFile, durations_pt);
	readIntVectorFromFile(inputFile, this->DA, this->K);

	for ( int k = 0; k < ProblemData::K; k++)
		for ( int h = 0; h < DA[k]; h++ )
			weeksAvailable[k][ProblemData::H - h - 1] = 0;

	fscanf(inputFile, ccMaxModules_pt, &this->ccMMAX);

	fscanf(inputFile, maxModules_pt);
	readIntVectorFromFile(inputFile, this->MMAX, this->K);

	fscanf(inputFile, maxRefuel_pt);
	readIntVectorFromFile(inputFile, this->RMAX, this->K);

	fscanf(inputFile, minRefuel_pt);
	readIntVectorFromFile(inputFile, this->RMIN, this->K);

	fscanf(inputFile, refuelRatio_pt);
	readIntVectorFromFile(inputFile, this->Q, this->K);

	fscanf(inputFile, ccStockThreshold_pt, &this->ccBO);

	fscanf(inputFile, stockThreshold_pt);
	readIntVectorFromFile(inputFile, this->BO, ( this->K + 1 ));

	fscanf(inputFile, pmax_pt);
	readDoubleVectorFromFile(inputFile, this->pmax, this->T);

	fscanf(inputFile, maxStockBefore_pt);
	readIntVectorFromFile(inputFile, this->AMAX, this->K);

	fscanf(inputFile, maxStockAfter_pt);
	readIntVectorFromFile(inputFile, this->SMAX, this->K);

	fscanf(inputFile, refuelCost_pt);
	readDoubleVectorFromFile(inputFile, this->C, this->K);

	averageCost = 0.0;
	for(int k = 0; k < ProblemData::K; k++)
		averageCost += C[k];
	averageCost /= ProblemData::K;

	fscanf(inputFile, fuelPrice_pt, &this->fuelPrice);

	currentProfile = new Profile(inputFile, true, ccBO);

	for (int k = 0; k < K; k++){
		profiles[k] = *(new Profile(inputFile, false, BO[k]));
	}
}

PowerPlant2::PowerPlant2()
{
	this->allocVars();
}

PowerPlant2 * PowerPlant2::clone()
{

	PowerPlant2 * newPP = new PowerPlant2();



	// copy prodution plan
	for(int s = 0; s < ProblemData::S; s++)
		for(int t = 0; t < ProblemData::T; t++)
			newPP->setProduction(s, t, getProduction(s, t));

	// copy outages
	for(int i=0; i < ProblemData::K; i++)
	{
		newPP->getOutage(i)->setStart(getOutage(i)->getStart());
		newPP->getOutage(i)->setRefuelAmount(getOutage(i)->getRefuelAmount());
	}

	strcpy(newPP->name, name);
	newPP->index = index;
	newPP->T = T;
	newPP->S = S;
	newPP->initialStock = initialStock;
	newPP->K = K;
	newPP->DA = DA;
	newPP->ccMMAX = ccMMAX;
	newPP->MMAX = MMAX;
	newPP->RMAX = RMAX;
	newPP->RMIN = RMIN;
	newPP->Q = Q;
	newPP->ccBO = ccBO;
	newPP->BO = BO;
	newPP->pmax = pmax;
	newPP->AMAX = AMAX;
	newPP->SMAX = SMAX;
	newPP->C = C;
	newPP->fuelPrice = fuelPrice;
	newPP->currentProfile = currentProfile;
	newPP->profiles = profiles;
	newPP->numOutages = numOutages;
	newPP->ct13 = ct13;
	newPP->ct14Indexes = ct14Indexes;
	newPP->ct14Overlap = ct14Overlap;
	newPP->ct15List = ct15List;
	newPP->decouplingIndexes = decouplingIndexes;
	newPP->decouplingSpacing = decouplingSpacing;
	newPP->couplingIndexes = couplingIndexes;
	newPP->couplingSpacing = couplingSpacing;
	newPP->ct18Indexes = ct18Indexes;
	newPP->ct18Spacing = ct18Spacing;
	newPP->ct19List = ct19List;
	newPP->ct20List = ct20List;
	newPP->ct21List = ct21List;
	newPP->weeksAvailable = weeksAvailable;

	return newPP;
}

PowerPlant2::~PowerPlant2(void)
{
	free(DA);
	free(MMAX);
	free(RMAX);
	free(RMIN);
	free(Q);
	free(BO);
	free(pmax);
	free(AMAX);
	free(SMAX);
	free(C);
	free(outages);
	free(outputPlan);
	free(ct14Overlap);
	free(decouplingSpacing);
	free(couplingSpacing);
	delete(ct14Indexes);
	delete(ct15List);
	delete(decouplingIndexes);
	delete(couplingIndexes);
	delete(ct13);
	delete(ct19List);
	delete(ct20List);
	delete(ct21List);

	for (int i=0; i < K; i++){
		profiles[i].~Profile();
	}

	currentProfile->~Profile();

}

Profile* PowerPlant2::getProfile(int k)
{
	if (k == -1)
		return currentProfile;
	else
		return &profiles[k];
}


bool PowerPlant2::addOutage(Outage *outage){
	outages[outage->getCampaign()] = outage;
	return true;
}

int* PowerPlant2::getDA()
{
	return DA;
}

double* PowerPlant2::getPmax()
{
	return pmax;
}

double PowerPlant2::getPmax(int t)
{
	return pmax[t];
}

double PowerPlant2::getProduction(int s, int t)
{
	return this->outputPlan[s]->getProduction(t);
}

double PowerPlant2::getFuelVariation(int s, int t)
{
	return this->outputPlan[s]->getFuelVariation(t);
}

Outage* PowerPlant2::getOutage(int k)
{
	return this->outages[k];
}

void PowerPlant2::setOutage(Outage* outage, int k)
{
	this->outages[k] = outage;
}

double PowerPlant2::getInitialStock()
{
	return initialStock;
}

int PowerPlant2::getQ(int k)
{
	return Q[k];
}


double PowerPlant2::getFuelPrice()
{
	return fuelPrice;
}

PowerOutput** PowerPlant2::getOutputPlan()
{
	return this->outputPlan;
}

Outage** PowerPlant2::getOutages()
{
	return this->outages;
}

int PowerPlant2::getK()
{
	return K;
}


int PowerPlant2::getDA(int k)
{
	return DA[k];
}

int PowerPlant2::getBO(int k)
{
	if (k == -1)
		return ccBO;
	else
		return BO[k];
}

double* PowerPlant2::getC()
{
	return C;
}

void PowerPlant2::addCt13(Constraint13* ct)
{
	(*this->ct13)[ct->getCampaign()] = ct;

	if (ct->getTO() > 0)
		invalidadeWeeks(ct->getCampaign(), 0, ct->getTO());
	if (ct->getTA() > 0)
		invalidadeWeeks(ct->getCampaign(), ct->getTA(), ProblemData::H);
}


vector<Constraint13*>* PowerPlant2::getCt13()
{
	return ct13;
}

list<int> * PowerPlant2::getCt14Indexes()
{
	return ct14Indexes;
}

int PowerPlant2::getCt14Overlap(int index)
{
	return ct14Overlap[index];
}

void PowerPlant2::setCt14Overlap(int index, int overlap)
{
	if (overlap > ct14Overlap[index])
		ct14Overlap[index] = overlap;
}

list<Constraint15*> * PowerPlant2::getCt15List()
{
	return ct15List;
}
void PowerPlant2::addCt15(Constraint15 * ct)
{
	ct15List->push_back(ct);
}

list<int> * PowerPlant2::getDecouplingIndexes()
{
	return decouplingIndexes;
}

int PowerPlant2::getDecouplingSpacing(int index)
{
	return decouplingSpacing[index];
}

void PowerPlant2::setDecouplingSpacing(int index, int spacing)
{
	if (spacing > decouplingSpacing[index])
		decouplingSpacing[index] = spacing;
}

list<int> * PowerPlant2::getCouplingIndexes()
{
	return couplingIndexes;
}

int PowerPlant2::getCouplingSpacing(int index)
{
	return couplingSpacing[index];
}

void PowerPlant2::setCouplingSpacing(int index, int spacing)
{
	if (spacing > couplingSpacing[index])
		couplingSpacing[index] = spacing;
}


list<int> * PowerPlant2::getCt18Indexes()
{
	return ct18Indexes;
}

int PowerPlant2::getCt18Spacing(int index)
{
	return ct18Spacing[index];
}

void PowerPlant2::setCt18Spacing(int index, int spacing)
{
	if (spacing > ct18Spacing[index])
		ct18Spacing[index] = spacing;
}

list<ResourceLimiter*>* PowerPlant2::getCt19List()
{
	return ct19List;
}

list<Constraint20*> * PowerPlant2::getCt20List()
{
	return ct20List;
}

list<Constraint21*> * PowerPlant2::getCt21List()
{
	return ct21List;
}

char * PowerPlant2::getName()
{
	return name;
}

int PowerPlant2::getIndex()
{
	return index;
}

int PowerPlant2::getRMIN(int k)
{
	return RMIN[k];
}

int PowerPlant2::getRMAX(int k)
{
	return RMAX[k];
}

int PowerPlant2::getAMAX(int k)
{
	return AMAX[k];
}

int PowerPlant2::getSMAX(int k)
{
	return SMAX[k];
}

int PowerPlant2::getMMAX(int k)
{
	return MMAX[k];
}

// verifica se existe um outage numa dada semana
bool PowerPlant2::isOnOutage(int w)
{
	Outage * outage;
	int start;

	for(int outageIndex = 0; outageIndex < ProblemData::K; outageIndex++)
	{
		outage = getOutage(outageIndex);
		// shouldn't happen
		if(outage == NULL)
		{
			printf("outage com index %d e NULL\n", outageIndex);
			break;
		}

		start = outage->getStart();
		// se não houver mais outages ou se a semana for antes do outage actual
		if(start == -1 || w < start)
			break;
		// se a week dada está no intervalo do outage actual
		if(start <= w && w <= start + outage->getDuration())
			return true;
	}
	return false;
}

int PowerPlant2::getCampaign(int w)
{
	int campaign = -1, start;

	for(int outageIndex = 0; outageIndex < ProblemData::K; outageIndex++)
	{
			Outage * outage = this->getOutage(outageIndex);
			// shouldn't happen
			if(outage == NULL)
			{
				printf("outage com index %d e NULL\n", outageIndex);
				break;
			}

			start = outage->getStart();
			// se não houver mais outages ou se a semana for antes do outage actual
			if(start == -1 || w < start)
				break;

			campaign++;
	}
	return campaign;
}

void PowerPlant2::invalidadeWeeks(int k, int begin, int end)
{
	for ( int h = begin; h < end; h++ )
		weeksAvailable[k][h]--;
}

void PowerPlant2::validadeWeeks(int k, int begin, int end)
{
	for ( int h = begin; h < end; h++ )
		weeksAvailable[k][h]++;
}

void PowerPlant2::setFuelVariation(int s, int t, double value)
{
	this->outputPlan[s]->setFuelVariation(t, simple_round( value ));
}

bool PowerPlant2::canOutage(int k, int h)
{
	if (getFuelVariation(0, h * ProblemData::WEEK_SIZE - 1) > AMAX[k])
		return false;
	return weeksAvailable[k][h] > 0;
}

void PowerPlant2::setProduction(int s, int t, double value)
{
	outputPlan[s]->setProduction(t, simple_round( value ) );
}

double PowerPlant2::getAverageCost()
{
	return averageCost;
}

bool PowerPlant2::addMaxProduction(int s, int t, int k){

	double prevFuel = 0, prevProduction = 0, currentFuel = 0;
	if (t == 0)
	{
		prevFuel = initialStock;
		prevProduction = 0;
	}
	else
	{
		prevFuel = getFuelVariation(s, t-1);
		prevProduction = getProduction(s, t-1) * (double)ProblemData::D[t-1];
	}

	currentFuel = prevFuel - prevProduction;

	this->setFuelVariation(s, t, currentFuel);

	if (currentFuel < getBO(k))
	{
		double pb = calculateProfileDecreasingValue(k, currentFuel);

		if (currentFuel > pb * getPmax(t) * (double)ProblemData::D[t])
		{
			double production = (1 + ProblemData::E) * pb * getPmax(t);
			double nextFuel = currentFuel - production*ProblemData::D[t+1];
			if (nextFuel < 0)
				production = (1 - ProblemData::E) * pb * getPmax(t);
			this->setProduction(s, t, production);
		}else
			this->setProduction(s, t, 0);
		return true;
	}

	this->setProduction(s, t, getPmax(t));

	return false;
}


void PowerPlant2::addNullProduction(int s, int t)
{
	double prevFuel = 0, prevProduction = 0, currentFuel = 0;
	if (t == 0)
	{
		prevFuel = initialStock;
		prevProduction = 0;
	}
	else
	{
		prevFuel = getFuelVariation(s, t-1);
		prevProduction = getProduction(s, t-1);
	}

	currentFuel = prevFuel - prevProduction * ProblemData::D[t-1];

	this->setFuelVariation(s, t, currentFuel);

	this->setProduction(s, t, 0.0);
}


double PowerPlant2::calculateProfileDecreasingValue(int k, double fuel)
{
	Profile* profile = getProfile(k);

	int np = profile->getNP();

	double PB = 0.0;

	if (fuel > profile->getFPB(0) || fuel < 0.0)
		return -1;

	for(int i=1; i < np; i++)
	{
		if (fuel > profile->getFPB(i))
		{
			// PB = ((fuel - F(i))*C(i-1) + (F(i-1) - fuel)*C(i)) / (F(i-1) - F(i));
			PB = ( (fuel - profile->getFPB(i)) * profile->getCPB(i-1) +
					(profile->getFPB(i-1) - fuel)*profile->getCPB(i) ) /
					(profile->getFPB(i-1) - profile->getFPB(i));
			break;
		}
	}
	return PB;
}


void PowerPlant2::calculateRemainingFuel(int s)
{
	outputPlan[s]->setRemainingFuel(outputPlan[s]->getFuelVariation(ProblemData::T-1)
			- outputPlan[s]->getProduction(ProblemData::T-1) * ProblemData::D[ProblemData::T - 1]);
}
