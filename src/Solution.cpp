#include "Solution.h"

Solution::Solution(){
	outageValidator = (int *)malloc( ProblemData::T *  sizeof(int) );
	csvStuff = new CsvStuff();

	totalProduction = allocDoubleMatrix(ProblemData::S, ProblemData::T);
	for(int scenario=0; scenario < ProblemData::S; scenario++)
		for(int timeStep=0; timeStep < ProblemData::T; timeStep++)
			totalProduction[scenario][timeStep] = 0.0;

	type1PowerPlants = new vector<PowerPlant1*>(ProblemData::J);
	type2PowerPlants = new vector<PowerPlant2*>(ProblemData::I);
}

Solution::~Solution(void){
	free(outageValidator);
	// TODO: free total production
	delete csvStuff;
}

void Solution::copySolution(Solution *source)
{
	for(int i=0; i < source->type2PowerPlants->size(); i++)
	{
		for(int k=0; k < ProblemData::K; k++)
		{
			(*this->type2PowerPlants)[i]->setOutage((*source->type2PowerPlants)[i]->getOutage(k), k);
		}
	}


	// 1) copy outages
//	list<Outage*>::iterator destinationIt = this->outages.begin();
//	list<Outage*>::iterator sourceIt = source->outages.begin();
//	Outage *outageSource, *outageDestination;
//
//	while(destinationIt != this->outages.end())
//	{
//		// copy start and refuel
//		outageSource = *sourceIt;
//		outageDestination = *destinationIt;
//
//		*outageDestination = *outageSource;
//
////		outageDestination->setStart(outageSource->getStart());
////		outageDestination->setRefuelAmount(outageSource->getRefuelAmount());
//		destinationIt++;
//		sourceIt++;
//	}




	// 2) copy pp 1 production
	//vector<PowerPlant1*> type1PowerPlants;
	PowerPlant1 * sourcePP1;
	for(unsigned int j=0; j < type1PowerPlants->size(); j++)
	{
		sourcePP1 = (*source->type1PowerPlants)[j];
		for(int s = 0; s < ProblemData::S; s++)
			for(int t = 0; t < ProblemData::T; t++)
				(*type1PowerPlants)[j]->setProduction(s, t, sourcePP1->getProduction(s, t));
	}

	// 3) copy pp 2 production
	PowerPlant2 * sourcePP2;
	for(unsigned int i=0; i < type2PowerPlants->size(); i++)
	{
		sourcePP2 = (*source->type2PowerPlants)[i];
		for(int s = 0; s < ProblemData::S; s++)
			for(int t = 0; t < ProblemData::T; t++)
			{
				(*type2PowerPlants)[i]->setProduction(s, t, sourcePP2->getProduction(s, t));
				(*type2PowerPlants)[i]->setFuelVariation(s, t, sourcePP2->getFuelVariation(s, t));
				PowerOutput **temp = (*type2PowerPlants)[i]->getOutputPlan();
				PowerOutput **temp2 = sourcePP2->getOutputPlan();
				temp[s]->setRemainingFuel(temp2[s]->getRemainingFuel());
			}
	}
}

PowerPlant1 * Solution::getType1PowerPlant(int plantIndex)
{
	return (*type1PowerPlants)[plantIndex];
}

bool Solution::addType1PowerPlantsProduction()
{
	double expectedDemand, totalCurrentProduction, ppCurrentProduction, pmax, maxSpareProduction, productionNeeded;
	int bestPPIndex;

	for(int scenario = 0; scenario < ProblemData::S; scenario++)
	{
			for(int timeStep = 0; timeStep < ProblemData::T; timeStep++)
			{
				expectedDemand = ProblemData::DEM[scenario][timeStep];
				//totalCurrentProduction = totalProduction[scenario][timeStep];

				totalCurrentProduction = getTotalProduction(scenario, timeStep);

				if(totalCurrentProduction > expectedDemand)
				{
					printf("????? totalCurrentProduction (%lf) > expectedDemand (%lf) on scenario %d time step %d\n", totalCurrentProduction, expectedDemand, scenario, timeStep);
					return false;
				}
				else if(totalCurrentProduction == expectedDemand)
					printf("Already perfect production on scenario %d time step %d\n", scenario, timeStep);
				else
				{
					productionNeeded = expectedDemand - totalCurrentProduction;
					for(int currentIndex = 0; currentIndex < ProblemData::J; currentIndex++)
					{
						bestPPIndex = ProblemData::sortedType1PP[scenario][timeStep][currentIndex];
						ppCurrentProduction = (*type1PowerPlants)[bestPPIndex]->getProduction(scenario, timeStep);
						pmax = (*type1PowerPlants)[bestPPIndex]->getPmax(scenario, timeStep);
						maxSpareProduction = pmax-ppCurrentProduction;

						// se a produção da pp em causa chega para satisfazer o demand
						if(maxSpareProduction >= productionNeeded)
						{
							(*type1PowerPlants)[bestPPIndex]->addProduction(scenario, timeStep, productionNeeded);
							productionNeeded = 0.0;
							break;
						}
						// se não chega continua
						else
						{
							(*type1PowerPlants)[bestPPIndex]->addProduction(scenario, timeStep, maxSpareProduction);
							productionNeeded -= maxSpareProduction;
						}
					}

					// verifica se não se conseguiu satisfazer o demand
					if(productionNeeded > 0.0)
					{
						printf("Not enough production to satisfy demand on scenario %d time step %d\n", scenario, timeStep);
						return false;
					}
				}
			}
	}

	return true;
}


bool Solution::addOutage(Outage *outage){

	if (!canAdd(outage))
		return false;

	PowerPlant2 * ppt = (*type2PowerPlants)[outage->getPowerPlantIndex()];

	int ha = outage->getStart();
	int da = ppt->getDA(outage->getCampaign());

	for (int k = outage->getCampaign() + 1; k < ProblemData::K; k++)
		ppt->invalidadeWeeks(k, 0, outage->getEnd());

	int begin = 0, end = 0;
	for (int i = 0; i < ProblemData::I; i++)
	{
		if (i == outage->getPowerPlantIndex())
			continue;

		for (int k = 0; k < ProblemData::K; k++)
		{
			// CT14
			begin = MAX( 0, 				ha - ppt->getCt14Overlap(i) - (*type2PowerPlants)[i]->getDA(k) + 1 );
			end = MIN( ProblemData::H, 		ha + da + ppt->getCt14Overlap(i) );

			(*type2PowerPlants)[i]->invalidadeWeeks(k, begin, end);


			// CT16
			begin = MAX( 0, 				ha - ppt->getDecouplingSpacing(i) );
			end = MIN( ProblemData::H, 		ha + ppt->getDecouplingSpacing(i) + 1 );

			(*type2PowerPlants)[i]->invalidadeWeeks(k, begin, end);


			// CT17
			begin = MAX( 0, 				ha + da - ppt->getCouplingSpacing(i) - (*type2PowerPlants)[i]->getDA(k) + 1 );
			end = MIN( ProblemData::H, 		ha + da + ppt->getCouplingSpacing(i) - (*type2PowerPlants)[i]->getDA(k));

			(*type2PowerPlants)[i]->invalidadeWeeks(k, begin, end);

			// CT18
			begin = MAX( 0, 				ha + da - ppt->getCouplingSpacing(i));
			end = MIN( ProblemData::H, 		ha + da + ppt->getCouplingSpacing(i));

			(*type2PowerPlants)[i]->invalidadeWeeks(k, begin, end);

			begin = MAX( 0, 				ha - ppt->getCt18Spacing(i) - (*type2PowerPlants)[i]->getDA(k) + 1 );
			end = MIN( ProblemData::H, 		ha + ppt->getCt18Spacing(i) - (*type2PowerPlants)[i]->getDA(k));

			(*type2PowerPlants)[i]->invalidadeWeeks(k, begin, end);



		}
	}

	// CT15

	list<Constraint15*> *  ct15List = ppt->getCt15List();

	for ( list<Constraint15*>::iterator ct = ct15List->begin(); ct != ct15List->end(); ct++)
	{
		int ID = (*ct)->getID(), IF = (*ct)->getIF();

		if ( ha > IF || ha < ID - da + 1 )
			continue;

		list<int> * ppts = (*ct)->getPowerplants();
		for (list<int>::iterator i = ppts->begin(); i != ppts->end(); i++)
		{
			if (*i == ppt->getIndex())
				continue;

			for(int k = 0; k < ProblemData::K; k++)
			{
				begin = MAX( ID, 				ha - (*ct)->getSe() - (*type2PowerPlants)[*i]->getDA(k) + 1 );
				end = MIN( IF, 		ha + da + (*ct)->getSe()  );

				(*type2PowerPlants)[*i]->invalidadeWeeks(k, begin, end);
			}

		}
	}


	// CT19
	list<ResourceLimiter*>*resourceLimiterList = ppt->getCt19List();

	for ( list<ResourceLimiter*>::iterator rl = resourceLimiterList->begin(); rl != resourceLimiterList->end(); rl++)
	{
		(*rl)->add(outage->getCampaign(), outage->getStart());
	}


	// CT20
	list<Constraint20*>* ct20List = ppt->getCt20List();
	for ( list<Constraint20*>::iterator ct = ct20List->begin(); ct != ct20List->end(); ct++)
	{
		if ((*ct)->getWeek() >= outage->getStart() && (*ct)->getWeek() < outage->getEnd())
			(*ct)->add();
	}


	// CT21
	list<Constraint21*>* ct21List = ppt->getCt21List();
	for ( list<Constraint21*>::iterator ct = ct21List->begin(); ct != ct21List->end(); ct++)
	{
		(*ct)->add(outage, ppt->getPmax());

	}


	list<Outage*>::iterator it = outages.begin();

	while ( it != outages.end() )
	{
		if ((*it)->getStart() > outage->getStart())
		{
			it = outages.insert(it, outage);
			break;
		}
		it++;
	}

	if (it == outages.end()){
		outages.insert(it, outage);
	}



	int start = outage->getStart() * ProblemData::WEEK_SIZE;

	int t = start;

	double prevFuel = 0, prevProduction = 0, currentFuel = 0;
	prevFuel = ppt->getFuelVariation(0, t-1);
	prevProduction = ppt->getProduction(0, t-1);

	currentFuel = prevFuel - prevProduction * ProblemData::D[t-1];
	ppt->setFuelVariation(0, t, currentFuel);
	ppt->setProduction(0, t, 0);

	int k = outage->getCampaign();
	double value = (double)((double)(ppt->getQ(k) - 1.0)/(double)ppt->getQ(k))*(ppt->getFuelVariation(0,start) - ppt->getBO(k-1)) + outage->getRefuelAmount() + ppt->getBO(k);

	for (int t = start + 1; t < start + ppt->getDA(k)*ProblemData::WEEK_SIZE; t++)
	{
		ppt->setFuelVariation(0, t, value);
		ppt->setProduction(0, t, 0);
	}

	return true;
}


void Solution::removeOutage(Outage *outage){

	PowerPlant2 * ppt = (*type2PowerPlants)[outage->getPowerPlantIndex()];

	ppt->setOutage(new Outage(-1, 0, 0, outage->getPowerPlantIndex(), outage->getCampaign()), outage->getCampaign());
	int ha = outage->getStart();
	int da = ppt->getDA(outage->getCampaign());

	for (int k = outage->getCampaign() + 1; k < ProblemData::K; k++)
			ppt->validadeWeeks(k, 0, outage->getEnd());

	int begin = 0, end = 0;
	for (int i = 0; i < ProblemData::I; i++)
	{
		if (i == outage->getPowerPlantIndex())
			continue;

		for (int k = 0; k < ProblemData::K; k++)
		{
			// CT14
			begin = MAX( 0, 				ha - ppt->getCt14Overlap(i) - (*type2PowerPlants)[i]->getDA(k) + 1 );
			end = MIN( ProblemData::H, 		ha + da + ppt->getCt14Overlap(i) );

			(*type2PowerPlants)[i]->validadeWeeks(k, begin, end);


			// CT16
			begin = MAX( 0, 				ha - ppt->getDecouplingSpacing(i) );
			end = MIN( ProblemData::H, 		ha + ppt->getDecouplingSpacing(i) + 1 );

			(*type2PowerPlants)[i]->validadeWeeks(k, begin, end);


			// CT17
			begin = MAX( 0, 				ha + da - ppt->getCouplingSpacing(i) - (*type2PowerPlants)[i]->getDA(k) + 1 );
			end = MIN( ProblemData::H, 		ha + da + ppt->getCouplingSpacing(i) - (*type2PowerPlants)[i]->getDA(k));

			(*type2PowerPlants)[i]->validadeWeeks(k, begin, end);

			// CT18
			begin = MAX( 0, 				ha + da - ppt->getCouplingSpacing(i));
			end = MIN( ProblemData::H, 		ha + da + ppt->getCouplingSpacing(i));

			(*type2PowerPlants)[i]->validadeWeeks(k, begin, end);

			begin = MAX( 0, 				ha - ppt->getCt18Spacing(i) - (*type2PowerPlants)[i]->getDA(k) + 1 );
			end = MIN( ProblemData::H, 		ha + ppt->getCt18Spacing(i) - (*type2PowerPlants)[i]->getDA(k));

			(*type2PowerPlants)[i]->validadeWeeks(k, begin, end);



		}
	}

	// CT15

	list<Constraint15*> *  ct15List = ppt->getCt15List();

	for ( list<Constraint15*>::iterator ct = ct15List->begin(); ct != ct15List->end(); ct++)
	{
		int ID = (*ct)->getID(), IF = (*ct)->getIF();

		if ( ha > IF || ha < ID - da + 1 )
			continue;

		list<int> * ppts = (*ct)->getPowerplants();
		for (list<int>::iterator i = ppts->begin(); i != ppts->end(); i++)
		{
			if (*i == ppt->getIndex())
				continue;

			for(int k = 0; k < ProblemData::K; k++)
			{
				begin = MAX( ID, 				ha - (*ct)->getSe() - (*type2PowerPlants)[*i]->getDA(k) + 1 );
				end = MIN( IF, 		ha + da + (*ct)->getSe()  );

				(*type2PowerPlants)[*i]->validadeWeeks(k, begin, end);
			}
		}
	}


	// CT19
	list<ResourceLimiter*>*resourceLimiterList = ppt->getCt19List();

	for ( list<ResourceLimiter*>::iterator rl = resourceLimiterList->begin(); rl != resourceLimiterList->end(); rl++)
	{
		(*rl)->remove(outage->getCampaign(), outage->getStart());
	}


	// CT20
	list<Constraint20*>* ct20List = ppt->getCt20List();
	for ( list<Constraint20*>::iterator ct = ct20List->begin(); ct != ct20List->end(); ct++)
	{
		if ((*ct)->getWeek() >= outage->getStart() && (*ct)->getWeek() < outage->getEnd())
			(*ct)->remove();
	}


	// CT21
	list<Constraint21*>* ct21List = ppt->getCt21List();
	for ( list<Constraint21*>::iterator ct = ct21List->begin(); ct != ct21List->end(); ct++)
	{
		(*ct)->remove(outage, ppt->getPmax());

	}


	outages.remove(outage);
}



bool Solution::canAdd(Outage * outage)
{
	PowerPlant2 * ppt = (*type2PowerPlants)[outage->getPowerPlantIndex()];

	if (!ppt->canOutage(outage->getCampaign(), outage->getStart()))
		return false;

	// CT19
	list<ResourceLimiter*>*resourceLimiterList = ppt->getCt19List();

	for ( list<ResourceLimiter*>::iterator rl = resourceLimiterList->begin(); rl != resourceLimiterList->end(); rl++)
	{
		if (!(*rl)->canAdd(outage->getCampaign(), outage->getStart()))
			return false;
	}


	// CT20
	list<Constraint20*>* ct20List = ppt->getCt20List();
	for ( list<Constraint20*>::iterator ct = ct20List->begin(); ct != ct20List->end(); ct++)
	{
		if ((*ct)->getWeek() >= outage->getStart() && (*ct)->getWeek() < outage->getEnd())
			if (!(*ct)->canAdd())
				return false;
	}


	// CT21
	list<Constraint21*>* ct21List = ppt->getCt21List();
	for ( list<Constraint21*>::iterator ct = ct21List->begin(); ct != ct21List->end(); ct++)
	{
		if (!(*ct)->canAdd(outage, ppt->getPmax()))
			return false;
	}

	return true;
}

/*
 *  Calculate objective function
 */
double Solution::calculateObjective()
{
	double soma = 0.0;

	for (int s = 0; s < ProblemData::S; s++)
	{
		soma += (calculateTotalProductionCost(s) - calculateRemainingCost(s));
	}

	soma = calculateRefuelCost() + soma / ProblemData::S;

	return soma;
}

double Solution::calculateRefuelCost()
{
	double soma = 0.0, campaignCost = 0.0;

	// para cada power plant do tipo 2
	for (unsigned int j = 0; j < type2PowerPlants->size(); j++)
	{
		// para cada campaign
		for (int k = 0; k < (*type2PowerPlants)[j]->getK(); k++)
		{
			if ((*type2PowerPlants)[j]->getOutages()[k] != NULL)
			{
				campaignCost = (*type2PowerPlants)[j]->getC()[k] * (*type2PowerPlants)[j]->getOutages()[k]->getRefuelAmount();
				soma += campaignCost;
				csvStuff->addCostOfRefuel(campaignCost, j, k);
			}
			else
				csvStuff->addCostOfRefuel(0.0, j, k);
		}
	}
	return soma;
}

double Solution::calculateRemainingCost(int s){

	double soma = 0.0, temp;

	for (unsigned int j = 0; j < type2PowerPlants->size(); j++)
	{
		temp = (*type2PowerPlants)[j]->getFuelPrice() * (*type2PowerPlants)[j]->getOutputPlan()[s]->getRemainingFuel();
		soma += temp;
		csvStuff->addSavedCost(temp, j, s);
	}

	return soma;
}

double Solution::calculateTotalProductionCost(int s){

	double soma = 0.0, temp;

	for (unsigned int j = 0 ; j < type1PowerPlants->size(); j++)
	{
		temp = calculatePowerPlantProductionCost(s, j);
		soma += temp;
		csvStuff->addCostOfProdution(temp, j, s);
	}

	return soma;

}

double Solution::calculatePowerPlantProductionCost(int s, int j){
	double soma = 0.0;
	PowerPlant1* powerPlant = (*type1PowerPlants)[j];

	for (int t = 0; t < ProblemData::T; t++)
	{
		soma += powerPlant->getCost(s,t) * powerPlant->getProduction(s,t) * ProblemData::D[t];
	}

	return soma;
}

/*
 * Constraint checking
 */

// check for the constraint 1
bool Solution::checkCT1()
{
	double soma;
	for (int s = 0; s < ProblemData::S; s++)
	{
		for (int t = 0; t < ProblemData::T; t++)
		{
			soma = 0.0;

			// sums power production of pp's for day t
			for (int i = 0; i < ProblemData::I; i++)
			{
				soma += (*type2PowerPlants)[i]->getOutputPlan()[s]->getProductionPlan()[t];
			}

			for (int j = 0; j < ProblemData::J; j++)
			{
				soma += ((*type1PowerPlants)[j])->getProductionPlan()[s][t];
			}

			// compares daily power with the daily demand
			if ( simple_round(soma) != simple_round(ProblemData::DEM[s][t]))
				return false;
		}

	}

	return true;
}

// check the bounds of production for type1 pp's
bool Solution::checkCT2()
{
	for (int j = 0; j < ProblemData::J; j++)
	{
		for (int s = 0; s < ProblemData::S; s++)
		{
			for (int t = 0; t < ProblemData::T; t++)
			{

				if (((*type1PowerPlants)[j])->getProduction(s,t) < ((*type1PowerPlants)[j])->getPmin(s,t)
				     || ((*type1PowerPlants)[j])->getProduction(s,t) > ((*type1PowerPlants)[j])->getPmax(s,t))
					return false;
			}
		}

	}

	return true;
}

// check no production during outage
bool Solution::checkCT3()
{
	int start, duration;

	for (int i = 0; i < ProblemData::I; i++)
	{
		for (int s = 0; s < ProblemData::S; s++)
		{
			for (int k = 0; k < ProblemData::K; k++)
			{
				start = (*type2PowerPlants)[i]->getOutage(k)->getStart() * ProblemData::WEEK_SIZE;

				if (start != -1)
				{
					duration = (*type2PowerPlants)[i]->getOutage(k)->getDuration() * ProblemData::WEEK_SIZE;

					for (int t = start; t < start + duration; t++)
						if ((*type2PowerPlants)[i]->getProduction(s, t) != 0.0)
							return false;
				}
			}

		}

	}

	return true;
}

// check minimum power
bool Solution::checkCT4()
{
	for (int i = 0; i < ProblemData::I; i++)
	{
		for (int s = 0; s < ProblemData::S; s++)
		{
			for (int t = 0; t < ProblemData::T; t++)
			{
				if ((*type2PowerPlants)[i]->getProduction(s, t) < 0.0)
					return false;
			}

		}

	}

	return true;
}


// check maximum power
bool Solution::checkCT5()
{
	for (int i = 0; i < ProblemData::I; i++)
	{
		for (int s = 0; s < ProblemData::S; s++)
		{
			for (int t = 0; t < ProblemData::T; t++)
			{
				if ((*type2PowerPlants)[i]->getProduction(s, t) > (*type2PowerPlants)[i]->getPmax(t))
					return false;
			}

		}

	}

	return true;
}


// check maximum power
bool Solution::checkCT4and5()
{
	for (int i = 0; i < ProblemData::I; i++)
	{
		for (int s = 0; s < ProblemData::S; s++)
		{
			for (int t = 0; t < ProblemData::T; t++)
			{
				if ((*type2PowerPlants)[i]->getProduction(s, t) > (*type2PowerPlants)[i]->getPmax(t) ||
						(*type2PowerPlants)[i]->getProduction(s, t) < 0.0)
					return false;
			}

		}

	}

	return true;
}


bool Solution::checkCT6()
{
	for (int i = 0; i < ProblemData::I; i++)
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[i];
		for (int w = 0; w < ProblemData::H; w++)
		{
			int k = ppt->getCampaign(w);
			if (ppt->isOnOutage(w))
			{
				w += ppt->getOutage(k)->getDuration()-1;
				continue;
			}

			for (int s = 0; s < ProblemData::S; s++)
			{
				for (int t = w*ProblemData::WEEK_SIZE; t < (w+1)*ProblemData::WEEK_SIZE; t++)
				{
					if (ppt->getOutputPlan()[s]->getFuelVariation(t)  < ppt->getBO(k))
					{
						double production = ppt->getProduction(s,t);
						double pb = calculateProfileDecreasingValue(i, k, ppt->getOutputPlan()[s]->getFuelVariation(t));
						if (ppt->getOutputPlan()[s]->getFuelVariation(t) >= pb * ppt->getPmax(t) * ProblemData::D[t])
						{

							if (production < (1.0 - ProblemData::E) * pb * ppt->getPmax(t) ||
									production > (1.0 + ProblemData::E) * pb * ppt->getPmax(t))
								return false;
						}
						else
						{
							if (production != 0)
								return false;
						}
					}
				}
			}
		}
	}

	return true;
}

bool Solution::checkCT7()
{
	for (int i = 0; i < ProblemData::I; i++)
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[i];
		for (int k = 0; k < ProblemData::K; k++)
		{
			Outage *outage = ppt->getOutage(k);
			if (outage != NULL && outage->getStart() != -1)
			{
				if (outage->getRefuelAmount() < ppt->getRMIN(k)
						|| outage->getRefuelAmount() > ppt->getRMAX(k))
					return false;
			}
		}
	}
	return true;
}

bool Solution::checkCT8()
{
	for (int i = 0; i < ProblemData::I; i++)
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[i];
		for (int s = 0; s < ProblemData::S; s++)
		{
			if (ppt->getProduction(s,0) != ppt->getInitialStock())
				return false;
		}
	}
	return true;
}

bool Solution::checkCT9()
{
	for (int i = 0; i < ProblemData::I; i++)
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[i];
		double current = ppt->getInitialStock();

		for (int w = 0; w < ProblemData::H; w++)
		{
			int k = ppt->getCampaign(w);
			if (ppt->isOnOutage(w))
			{
				current += ppt->getOutage(k)->getRefuelAmount();
				w += ppt->getOutage(k)->getDuration()-1;
				continue;
			}

			for (int s = 0; s < ProblemData::S; s++)
			{
				for (int t = w*ProblemData::WEEK_SIZE; t < (w+1)*ProblemData::WEEK_SIZE; t++)
				{
					if (t != 0)
					{
						current -= ppt->getProduction(s, t-1)*ProblemData::D[t-1];

						if (ppt->getOutputPlan()[s]->getFuelVariation(t) != current)
							return false;
					}
				}
			}
		}
	}
	return true;
}


bool Solution::checkCT10()
{
	for( int i = 0; i < ProblemData::I; i++ )
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[i];
		for( int k = 0; k < ProblemData::K; k++ )
		{
			Outage * outage = ppt->getOutage(k);
			if ( outage == NULL || outage->getStart() == -1 )
				continue;

			int start = outage->getStart() * ProblemData::WEEK_SIZE;
			int end = (outage->getStart() + outage->getDuration()) * ProblemData::WEEK_SIZE;

			for( int s = 0; s < ProblemData::S; s++ )
			{
				double value = ((ppt->getQ(k) - 1)/ppt->getQ(k))*(ppt->getFuelVariation(s,start) - ppt->getBO(k-1)) + outage->getRefuelAmount() + ppt->getBO(k);

				for( int t = start+1; t < end; t++ )
				{
					if ( ppt->getFuelVariation(s,t) > value + ProblemData::E || ppt->getFuelVariation(s,t) < value - ProblemData::E )
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}


bool Solution::checkCT11()
{
	for( int i = 0; i < ProblemData::I; i++ )
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[i];
		for( int k = 0; k < ProblemData::K; k++ )
		{
			Outage * outage = ppt->getOutage(k);
			if ( outage == NULL || outage->getStart() == -1 )
				continue;

			int start = outage->getStart() * ProblemData::WEEK_SIZE;

			for( int s = 0; s < ProblemData::S; s++ )
			{

				if ( ppt->getFuelVariation(s,start) > ppt->getAMAX(k) || ppt->getFuelVariation(s,start+1) > ppt->getSMAX(k) )
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool Solution::checkCT12()
{
	int currentStart = 0;
	for( int i = 0; i < ProblemData::I; i++ )
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[i];
		for ( int k = 0; k < ProblemData::K; k++ )
		{
			Outage * outage = ppt->getOutage(k);
			if ( outage == NULL || outage->getStart() == -1 )
				break;
			for ( int s = 0; s < ProblemData::S; s++)
			{
				double sum = 0;

				for ( int t = currentStart; t < outage->getStart() * ProblemData::WEEK_SIZE; t++ )
				{
					sum += (ppt->getPmax(t) - ppt->getProduction(s,t)) * ProblemData::D[t];

				}

				if ( sum > ppt->getMMAX(k) )
					return false;
			}

			currentStart = ( outage->getStart() + outage->getDuration() ) * ProblemData::WEEK_SIZE;
		}
	}
	return true;
}

bool Solution::checkCT13()
{
	for (int i = 0; i < ProblemData::I; i++)
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[i];
		vector<Constraint13*> *all = ppt->getCt13();
		for(int c = 0; c < all->size(); c++)
		{
			Constraint13* ct = (*all)[c];

			if (ct == NULL)
				continue;
			int k = ct->getCampaign();

			int first = ct->getTO();
			int last = ct->getTA();

			if (last == -1)
				last = ProblemData::H;

			Outage * outage = ppt->getOutage(k);

			if (outage->getStart() < first || outage->getStart() + outage->getDuration() > last)
				return false;

		}

		for (int k = 1; k < ProblemData::K; k++)
		{
			if (ppt->getOutage(k)->getStart() < ppt->getOutage(k-1)->getStart() + ppt->getOutage(k-1)->getDuration())
				return false;
		}
	}
	return true;
}

bool Solution::checkCT14()
{
	for(list<Outage*>::iterator it = outages.begin(); it != outages.end(); it++)
	{
		int end = (*it)->getStart() + (*it)->getDuration();
		list<Outage*>::iterator front = it;
		front++;
		while( front != outages.end() )
		{
			if ((*front)->getStart() - end <  (*type2PowerPlants)[(*it)->getPowerPlantIndex()]->getCt14Overlap((*front)->getPowerPlantIndex()))
				return false;
			front++;
		}
	}
	return true;
}

bool Solution::checkCT16()
{
	for(list<Outage*>::iterator it = outages.begin(); it != outages.end(); it++)
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[(*it)->getPowerPlantIndex()];
		list<Outage*>::iterator front = it;
		front++;
		while( front != outages.end() )
		{
			if (abs((*front)->getStart() - (*it)->getStart()) <  ppt->getDecouplingSpacing((*front)->getPowerPlantIndex()))
				return false;

			front++;
		}
	}
	return true;
}

bool Solution::checkCT17()
{
	for(list<Outage*>::iterator it = outages.begin(); it != outages.end(); it++)
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[(*it)->getPowerPlantIndex()];
		list<Outage*>::iterator front = it;
		front++;
		while( front != outages.end() )
		{
			if (abs((*front)->getEnd()  - (*it)->getEnd()) <  ppt->getCouplingSpacing((*front)->getPowerPlantIndex()))
				return false;

			front++;
		}
	}
	return true;
}

bool Solution::checkCT18()
{
	for(list<Outage*>::iterator it = outages.begin(); it != outages.end(); it++)
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[(*it)->getPowerPlantIndex()];
		list<Outage*>::iterator front = it;
		front++;
		while( front != outages.end() )
		{
			if (abs((*front)->getStart()  - (*it)->getEnd()) <  ppt->getCt18Spacing((*front)->getPowerPlantIndex()))
				return false;

			front++;
		}
	}
	return true;
}

bool Solution::checkCT19()
{
	for (int i = 0; i < ProblemData::I; i++)
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[i];
		list<ResourceLimiter*> *ctList = ppt->getCt19List();

		for(list<ResourceLimiter*>::iterator it = ctList->begin(); it != ctList->end(); it++)
		{
			for(int k = 0; k < ProblemData::K; k++)
			{
				Outage * outage = ppt->getOutage(k);

				if (outage == NULL || outage->getStart() == -1)
					break;

				if (!(*it)->validate(k, outage->getStart()))
					return false;

			}
		}
	}
	return true;

}


bool Solution::checkCT20()
{
	for (int i = 0; i < ProblemData::I; i++)
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[i];
		list<Constraint20*> *ctList = ppt->getCt20List();

		for(list<Constraint20*>::iterator it = ctList->begin(); it != ctList->end(); it++)
		{
			if ((*it)->getCurrent() > (*it)->getMax())
				return false;
		}
	}
	return true;
}

bool Solution::checkCT21()
{
	for (int i = 0; i < ProblemData::I; i++)
	{
		PowerPlant2 * ppt = (*type2PowerPlants)[i];
		list<Constraint21*> *ctList = ppt->getCt21List();

		for(list<Constraint21*>::iterator it = ctList->begin(); it != ctList->end(); it++)
		{
			if (!(*it)->validate())
				return false;
		}
	}
	return true;
}


double Solution::calculateProfileDecreasingValue(int i, int k, double fuel)
{
	Profile* profile = (*type2PowerPlants)[i]->getProfile(k);

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
					(profile->getFPB(i-1) - profile->getFPB(i-1));
			break;
		}
	}
	return PB;
}


/*
 * SOLUTION OUTPUT
 */


void Solution::writeSolution(char *fileName, int teamID)
{
	// create file
	FILE* outputFile = fopen(fileName, "w");
	if(outputFile == NULL)
		return;

	// main section
	fprintf(outputFile, begin_main_pt);
	fprintf(outputFile, TEAM_IDENTIFIER_PT, teamID);
	//fprintf(outputFile, SOLUTION_DATE_PT, date);
	// "05/01/2010 00:00:00"
	writeDateToFile(outputFile);
	//fprintf(outputFile, SOLUTION_RUNNING_TIME_PT, runningTime);
	// "00:27:27"
	writeRunTimeToFile(outputFile);
	fprintf(outputFile, DATA_SET_PT, ProblemData::dataSet);
	fprintf(outputFile, COST_PT, calculateObjective());
	fprintf(outputFile, end_main_pt);

	// outages section
	fprintf(outputFile, BEGIN_OUTAGES_PT);

	for(unsigned int powerPlantIndex = 0; powerPlantIndex < type2PowerPlants->size(); powerPlantIndex++)
	{
		fprintf(outputFile, name_pt, (*type2PowerPlants)[powerPlantIndex]->getName());
		fprintf(outputFile, index_pt, (*type2PowerPlants)[powerPlantIndex]->getIndex());
		Outage** outages = (*type2PowerPlants)[powerPlantIndex]->getOutages();

		// print all start weeks
		fprintf(outputFile, OUTAGES_DATES_PT);

		for(int outageIndex = 0; outageIndex < ProblemData::K; outageIndex++)
			fprintf(outputFile, " %d", outages[outageIndex]->getStart());
		fprintf(outputFile, "\n");

		// print all reloaded fuel
		fprintf(outputFile, RELOADED_FUEL_PT);

		for(int outageIndex = 0; outageIndex < ProblemData::K; outageIndex++)
			fprintf(outputFile, " %.2f", outages[outageIndex]->getRefuelAmount());
		fprintf(outputFile, "\n");
	}
	fprintf(outputFile, END_OUTAGES_PT);

	// power output section
	fprintf(outputFile, BEGIN_POWER_OUTPUT_PT);

	// for all scenarios
	for(int scenarioIndex = 0; scenarioIndex < ProblemData::S; scenarioIndex++)
	{
		fprintf(outputFile, scenario_pt, scenarioIndex);

		// for all type 1 power plants
		fprintf(outputFile, BEGIN_TYPE1_PP_PT);

		for(unsigned int type1PPIndex = 0; type1PPIndex < type1PowerPlants->size(); type1PPIndex++)
		{
			fprintf(outputFile, "name %s %d", (*type1PowerPlants)[type1PPIndex]->getName(), (*type1PowerPlants)[type1PPIndex]->getIndex());
			//double** produtionPlan = (*type1PowerPlants)[type1PPIndex]->getProductionPlan();
			double* produtionPlan = (*type1PowerPlants)[type1PPIndex]->getProductionPlan()[scenarioIndex];

			// for all time steps
			for(int timeStep = 0; timeStep < ProblemData::T; timeStep++)
				fprintf(outputFile, " %.2f", produtionPlan[timeStep]);
			fprintf(outputFile, "\n");
		}
		fprintf(outputFile, END_TYPE1_PP_PT);

		// for all type 2 power plants
		fprintf(outputFile, BEGIN_TYPE2_PP_PT);

		for(unsigned int type2PPIndex = 0; type2PPIndex < type2PowerPlants->size(); type2PPIndex++)
		{
			fprintf(outputFile, "name %s %d", (*type2PowerPlants)[type2PPIndex]->getName(), (*type2PowerPlants)[type2PPIndex]->getIndex());
			PowerOutput* powerOutput = (*type2PowerPlants)[type2PPIndex]->getOutputPlan()[scenarioIndex];
			double * productionPlan = powerOutput->getProductionPlan();
			double * fuelVariation = powerOutput->getFuelVariation();

			// for all time steps
			for(int timeStep = 0; timeStep < ProblemData::T; timeStep++)
				fprintf(outputFile, " %.2f", productionPlan[timeStep]);
			fprintf(outputFile, "\n");

			fprintf(outputFile, FUEL_VARIATION_PT);
			// for all time steps
			for(int timeStep = 0; timeStep < ProblemData::T; timeStep++)
				fprintf(outputFile, " %.2f", fuelVariation[timeStep]);
			fprintf(outputFile, "\n");

			fprintf(outputFile, REMAINING_FUEL_PT, powerOutput->getRemainingFuel());
		}
		fprintf(outputFile, END_TYPE2_PP_PT);
	}

	fprintf(outputFile, END_POWER_OUTPUT_PT);
	fclose(outputFile);
}

void Solution::writeToCsvFile(char *fileName)
{
	csvStuff->writeToCsvFile(fileName);
}


void Solution::addType1PowerPlant(PowerPlant1* pp, int j)
{
	(*type1PowerPlants)[j] = pp;
}


void Solution::addProduction(int s, int t, double value)
{
	totalProduction[s][t] += value;
}

void Solution::removeProduction(int s, int t, double value)
{
	totalProduction[s][t] -= value;
}
void Solution::addType2PowerPlant(PowerPlant2* pp, int i)
{
	(*type2PowerPlants)[i] = pp;
}


double Solution::getTotalProduction(int s, int t)
{
	double soma = 0.0;

	for (int j = 0; j < ProblemData::J; j++)
	{
		soma +=  simple_round((*type1PowerPlants)[j]->getProduction(s, t));
	}

	for (int i = 0; i < ProblemData::I; i++)
	{
		soma += simple_round((*type2PowerPlants)[i]->getProduction(s, t));
	}

	return simple_round(soma);
}


int Solution::findAvailableWeekToOutage(int h, PowerPlant2 * ppt, int k)
{
	int radius = 0;
	int back, front;

	Outage * o = new Outage(h, ppt->getDA(k), ppt->getRMAX(k), ppt->getIndex(), k);

	if (canAdd(o))
		return h;

	while (1)
	{
		back = h - radius;
		front = h + radius;

		if (front < ProblemData::H)
		{
			o = new Outage(front, ppt->getDA(k), ppt->getRMAX(k), ppt->getIndex(), k);
			if (canAdd(o))
				return front;
		}
		else if (back >= 0)
		{
			o = new Outage(back, ppt->getDA(k), ppt->getRMAX(k), ppt->getIndex(), k);
			if (canAdd(o))
				return back;
		}else
			break;

		radius++;
	}
	return -1;
}
