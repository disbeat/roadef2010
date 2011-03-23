#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include "PowerPlant1.h"
#include "PowerPlant2.h"
#include "filePatterns.h"
#include "utils.h"
#include "Constraint13.h"
#include "Constraint15.h"
#include "Constraint19.h"
#include "Constraint20.h"
#include "ProblemData.h"
#include "Solution.h"
#include <list>
#include <vector>

using namespace std;

//char* filepath = "../data/data-A/data0.txt";
char filepath[500];
char solutionFile[500] = "OUTPUT.txt";

int solutionIndex = 0;

//PowerPlant1 * ppt1[100];
//PowerPlant2 * ppt2[100];

Solution *solution, *bestSolution = NULL;
double bestCost = 0.0;

// ########## MAIN SECTION DATA ##########



// main section reading
void readMainSection(FILE* inputFile){
	// read main variables
	fscanf(inputFile, begin_main_pt);
	fscanf(inputFile, timesteps_pt, &ProblemData::T);
	fscanf(inputFile, weeks_pt, &ProblemData::H);

	ProblemData::WEEK_SIZE = ProblemData::T / ProblemData::H;

	fscanf(inputFile, campaigns_pt, &ProblemData::K);
	fscanf(inputFile, scenario_pt, &ProblemData::S);
	fscanf(inputFile, epsilon_pt, &ProblemData::E);
	fscanf(inputFile, powerplant1_pt, &ProblemData::J);
	fscanf(inputFile, powerplant2_pt, &ProblemData::I);
	// read number of constraints per type
	fscanf(inputFile, constraint13_pt, &ProblemData::M[0]);
	fscanf(inputFile, constraint14_pt, &ProblemData::M[1]);
	fscanf(inputFile, constraint15_pt, &ProblemData::M[2]);
	fscanf(inputFile, constraint16_pt, &ProblemData::M[3]);
	fscanf(inputFile, constraint17_pt, &ProblemData::M[4]);
	fscanf(inputFile, constraint18_pt, &ProblemData::M[5]);
	fscanf(inputFile, constraint19_pt, &ProblemData::M[6]);
	fscanf(inputFile, constraint20_pt, &ProblemData::M[7]);
	fscanf(inputFile, constraint21_pt, &ProblemData::M[8]);

	// allocate space for durations and demands
	ProblemData::D = (int*) malloc(ProblemData::T * sizeof(int));
	ProblemData::DEM = allocDoubleMatrix(ProblemData::S, ProblemData::T);
	int index, scenarioIndex;

	// read duration for each timestep
	fscanf(inputFile, durations_pt);
	for (index = 0; index < ProblemData::T; index++)
		fscanf(inputFile, " %d", &ProblemData::D[index]);
	fscanf(inputFile, "\n");

	// read demand for each scenario and timestep
	for (scenarioIndex = 0; scenarioIndex < ProblemData::S; scenarioIndex++)
	{
		fscanf(inputFile, demand_pt);
		// read all timesteps of this scenario
		for (index = 0; index < ProblemData::T; index++)
			fscanf(inputFile, " %lf", &ProblemData::DEM[scenarioIndex][index]);
		fscanf(inputFile, "\n");
	}
	fscanf(inputFile, end_main_pt);
}

// type 1 power plant section reading
void readType1PowerPlants(FILE* inputFile){
	char tmpName[NAME_SIZE];
	int tmpType;
	// allocate space for all type 1 power plants
	ProblemData::type1PowerPlants = (PowerPlant1 **) malloc (ProblemData::J * sizeof(PowerPlant1*));
	// for all type 1 power plants read corresponding section
	for (int j=0; j < ProblemData::J; j++){
		fscanf(inputFile, begin_powerplant_pt);
		fscanf(inputFile, name_pt, &tmpName);
		fscanf(inputFile, type_pt, &tmpType);
		ProblemData::type1PowerPlants[j] = new PowerPlant1(inputFile, tmpName);
		fscanf(inputFile, end_powerplant_pt);
	}
}

// type 2 power plant section reading
void readType2PowerPlants(FILE* inputFile){
	char tmpName[NAME_SIZE];
	int tmpType;
	// allocate space for all type 2 power plants
	ProblemData::type2PowerPlants = (PowerPlant2 **) malloc (ProblemData::I * sizeof(PowerPlant2*));
	// for all type 2 power plants read corresponding section
	for (int i=0; i < ProblemData::I; i++){
		fscanf(inputFile, begin_powerplant_pt);
		fscanf(inputFile, name_pt, &tmpName);
		fscanf(inputFile, type_pt, &tmpType);
		ProblemData::type2PowerPlants[i] = new PowerPlant2(inputFile, tmpName, ProblemData::T, ProblemData::S);
		fscanf(inputFile, end_powerplant_pt);
	}
}


void readCT13(FILE* inputFile)
{
	Constraint13 *ct = new Constraint13(inputFile);
	ProblemData::type2PowerPlants[ct->getPowerplant()]->addCt13(ct);
}


void readCT14(FILE* inputFile)
{
	list<int> setIndexes;
	int tmp;
	int Se;
	int type;

	fscanf(inputFile, beginConstraint_pt);
	fscanf(inputFile, type_pt, &type);
	fscanf(inputFile, index_pt, &tmp);
	fscanf(inputFile, set_pt);
	while(1){
		if (fscanf(inputFile, " %d", &tmp) <= 0)
			break;
		setIndexes.push_back(tmp);
	}

	fscanf(inputFile, "\n");

	fscanf(inputFile, spacing_pt, &Se);

	for (list<int>::iterator it = setIndexes.begin(); it != setIndexes.end(); it++)
	{
		for (list<int>::iterator it2 = setIndexes.begin(); it2 != setIndexes.end(); it2++)
		{
			if (it != it2)
			{
				ProblemData::type2PowerPlants[*it]->getCt14Indexes()->push_back(*it2);
				ProblemData::type2PowerPlants[*it]->setCt14Overlap(*it2 ,Se);

			}
		}
	}
	fscanf(inputFile, endConstraint_pt);

}


void readCT15(FILE* inputFile)
{
	list<int> powerplants;
	int tmp;
	int Se;
	int ID, IF;
	int type;

	fscanf(inputFile, beginConstraint_pt);
	fscanf(inputFile, type_pt, &type);
	fscanf(inputFile, index_pt, &tmp);
	fscanf(inputFile, set_pt);
	while(1){
		if (fscanf(inputFile, " %d", &tmp) <= 0)
			break;
		powerplants.push_back(tmp);
	}

	fscanf(inputFile, "\n");

	fscanf(inputFile, spacing_pt, &Se);
	fscanf(inputFile, start_pt, &ID);
	fscanf(inputFile, end_pt, &IF);

	Constraint15 * constraint = new Constraint15(ID, IF, Se, powerplants);

	for (list<int>::iterator it = powerplants.begin(); it != powerplants.end(); it++)
	{
		ProblemData::type2PowerPlants[*it]->addCt15(constraint);
	}

	fscanf(inputFile, endConstraint_pt);
}

void readCT16(FILE* inputFile)
{
	list<int> setIndexes;
	int tmp;
	int Se;
	int type;

	fscanf(inputFile, beginConstraint_pt);
	fscanf(inputFile, type_pt, &type);
	fscanf(inputFile, index_pt, &tmp);
	fscanf(inputFile, set_pt);
	while(1){
		if (fscanf(inputFile, " %d", &tmp) <= 0)
			break;
		setIndexes.push_back(tmp);
	}

	fscanf(inputFile, "\n");

	fscanf(inputFile, spacing_pt, &Se);

	for (list<int>::iterator it = setIndexes.begin(); it != setIndexes.end(); it++)
	{
		for (list<int>::iterator it2 = setIndexes.begin(); it2 != setIndexes.end(); it2++)
		{
			if (it != it2)
			{
				ProblemData::type2PowerPlants[*it]->getDecouplingIndexes()->push_back(*it2);
				ProblemData::type2PowerPlants[*it]->setDecouplingSpacing(*it2, Se);

			}
		}
	}
	fscanf(inputFile, endConstraint_pt);

}


void readCT17(FILE* inputFile)
{
	list<int> setIndexes;
	int tmp;
	int Se;
	int type;

	fscanf(inputFile, beginConstraint_pt);
	fscanf(inputFile, type_pt, &type);
	fscanf(inputFile, index_pt, &tmp);
	fscanf(inputFile, set_pt);
	while(1){
		if (fscanf(inputFile, " %d", &tmp) <= 0)
			break;
		setIndexes.push_back(tmp);
	}

	fscanf(inputFile, "\n");

	fscanf(inputFile, spacing_pt, &Se);

	for (list<int>::iterator it = setIndexes.begin(); it != setIndexes.end(); it++)
	{
		for (list<int>::iterator it2 = setIndexes.begin(); it2 != setIndexes.end(); it2++)
		{
			if (it != it2)
			{
				ProblemData::type2PowerPlants[*it]->getCouplingIndexes()->push_back(*it2);
				ProblemData::type2PowerPlants[*it]->setCouplingSpacing(*it2, Se);

			}
		}
	}
	fscanf(inputFile, endConstraint_pt);

}


void readCT18(FILE* inputFile)
{
	list<int> setIndexes;
	int tmp;
	int Se;
	int type;

	fscanf(inputFile, beginConstraint_pt);
	fscanf(inputFile, type_pt, &type);
	fscanf(inputFile, index_pt, &tmp);
	fscanf(inputFile, set_pt);
	while(1){
		if (fscanf(inputFile, " %d", &tmp) <= 0)
			break;
		setIndexes.push_back(tmp);
	}

	fscanf(inputFile, "\n");

	fscanf(inputFile, spacing_pt, &Se);

	for (list<int>::iterator it = setIndexes.begin(); it != setIndexes.end(); it++)
	{
		for (list<int>::iterator it2 = setIndexes.begin(); it2 != setIndexes.end(); it2++)
		{
			if (it != it2)
			{
				ProblemData::type2PowerPlants[*it]->getCt18Indexes()->push_back(*it2);
				ProblemData::type2PowerPlants[*it]->setCt18Spacing(*it2, Se);

			}
		}
	}
	fscanf(inputFile, endConstraint_pt);

}

void readCT19(FILE* inputFile)
{
	vector<int> setIndexes;
	int Q, tmp;
	Constraint19* ct = new Constraint19(inputFile);

	fscanf(inputFile, quantity_pt, &Q);
	ct->setQ(Q);

	fscanf(inputFile, set_pt);

	while( fscanf(inputFile, " %d", &tmp) > 0)
	{
		setIndexes.push_back(tmp);
	}
	fscanf(inputFile, "\n");

	for (unsigned int s = 0; s < setIndexes.size(); s++)
	{

		ResourceLimiter* resourceLimiter = new ResourceLimiter();
		resourceLimiter->setConstraint(ct);

		vector<int> starts, durations;
		int powerPlantIndex;

		fscanf(inputFile, beginPeriod_pt);
		fscanf(inputFile, powerplant_pt, &powerPlantIndex);

		fscanf(inputFile, ct19Start_pt);
		while( fscanf(inputFile, " %d", &tmp) > 0)
		{
			starts.push_back(tmp);
		}
		fscanf(inputFile, "\n");

		fscanf(inputFile, ct19Duration_pt);
		while( fscanf(inputFile, " %d", &tmp) > 0)
		{
			durations.push_back(tmp);
		}
		fscanf(inputFile, "\n");


		Period** periods = resourceLimiter->getPeriods();

		Period* p;
		for (unsigned int i = 0; i < starts.size(); i++)
		{
			p = new Period( starts[i], durations[i] );
			periods[i] = p;
		}

		ProblemData::type2PowerPlants[powerPlantIndex]->getCt19List()->push_back(resourceLimiter);

		fscanf(inputFile, endPeriod_pt);

	}


	fscanf(inputFile, endConstraint_pt);

}


void readCT20(FILE* inputFile)
{
	int tmp, week, max;
	int type;
	fscanf(inputFile, beginConstraint_pt);
	fscanf(inputFile, type_pt, &type);
	fscanf(inputFile, index_pt, &tmp);
	fscanf(inputFile, week_pt, &week);
	Constraint20 * constraint = new Constraint20();
	fscanf(inputFile, set_pt);

	while( fscanf(inputFile, " %d", &tmp) > 0)
		ProblemData::type2PowerPlants[tmp]->getCt20List()->push_back(constraint);


	fscanf(inputFile, "\n");
	fscanf(inputFile, max_pt, &max);
	constraint->setWeek(week);
	constraint->setMax(max);
	fscanf(inputFile, endConstraint_pt);

}

void readCT21(FILE* inputFile)
{
	double max;
	int type;
	int end, start, tmp;
	fscanf(inputFile, beginConstraint_pt);
	fscanf(inputFile, type_pt, &type);
	fscanf(inputFile, index_pt, &tmp);
	Constraint21 * constraint = new Constraint21();
	fscanf(inputFile, set_pt);

	while( fscanf(inputFile, " %d", &tmp) > 0)
		ProblemData::type2PowerPlants[tmp]->getCt21List()->push_back(constraint);
	fscanf(inputFile, "\n");

	fscanf(inputFile, startend_pt, &start, &end);
	fscanf(inputFile, maxDouble_pt, &max);

	constraint->setMaxPower(max);
	constraint->setStart(start);
	constraint->setEnd(end);
	fscanf(inputFile, endConstraint_pt);

}

// constraints section reading
void readConstraints(FILE* inputFile){

	// CT 13
	for(int i=0; i<ProblemData::M[0]; i++)
		readCT13(inputFile);

	// CT 14
	for(int i=0; i<ProblemData::M[1]; i++)
		readCT14(inputFile);

	// CT 15
	for(int i=0; i<ProblemData::M[2]; i++)
		readCT15(inputFile);

	// CT 16
	for(int i=0; i<ProblemData::M[3]; i++)
		readCT16(inputFile);

	// CT 17
	for(int i=0; i<ProblemData::M[4]; i++)
		readCT17(inputFile);

	// CT 18
	for(int i=0; i<ProblemData::M[5]; i++)
		readCT18(inputFile);

	// CT 19
	for(int i=0; i<ProblemData::M[6]; i++)
		readCT19(inputFile);

	// CT 20
	for(int i=0; i<ProblemData::M[7]; i++)
		readCT20(inputFile);

	// CT 21
	for(int i=0; i<ProblemData::M[8]; i++)
		readCT21(inputFile);
}

void initializePpt1WithMinimumProduction()
{
	for (int j = 0; j < ProblemData::J; j++)
	{
		for (int s = 0; s < ProblemData::S; s++)
		{
			for (int t = 0; t < ProblemData::T; t++)
			{
				ProblemData::type1PowerPlants[j]->setProduction(s, t, ProblemData::type1PowerPlants[j]->getPmin(s, t));
				(*solution->type1PowerPlants)[j]->setProduction(s, t, ProblemData::type1PowerPlants[j]->getPmin(s, t));
				solution->addProduction(s, t, ProblemData::type1PowerPlants[j]->getPmin(s, t));
			}
		}
	}
}

void scheduler(int curI, int curH, int k, bool outage, int outageWeek)
{
	// se já não há mais tempo
	if(canKeepRunning() == false)
	{
		bestSolution->writeSolution(solutionFile, 18);
		exit(0);
	}

	// se já estão todas tratadas
	if(curI == ProblemData::I)
	{
		// add type 1 pp production
		bool valid = solution->addType1PowerPlantsProduction();
		if(valid)
		{
			// first solution
			if(bestCost == 0.0)
			{
				bestSolution->copySolution(solution);
				bestCost = bestSolution->calculateObjective();
			}
			else
			{
				double cost = solution->calculateObjective();
				if(cost < bestCost)
				{
					bestCost = cost;
					bestSolution->copySolution(solution);
				}
			}
		}
/*		char solName[1000];
		sprintf(solName, "solution%d.txt",solutionIndex++);
		fflush(stdout);
		solution->writeSolution(solName, 18);
*/
//		printf("SOLUTION CREATED: %d | cost = %lf\n", solutionIndex++, solution->calculateObjective());

		initializePpt1WithMinimumProduction();

		return;
	}

//	printf("scheduler: i = %d, k = %d, h = %d, outage = %d, outageWeek = %d\n", curI, k, curH, outage, outageWeek );
	Outage *o = NULL;
	for (int i = curI; i < ProblemData::I; i++)
	{
//		printf("scheduler: i = %d, k = %d\n", i, k );
		for (int h = curH; h < ProblemData::H; h++)
		{
			if (outage && outageWeek == -1)
			{
				outageWeek = solution->findAvailableWeekToOutage(h, (*solution->type2PowerPlants)[i], k+1);
//				printf("scheduler: i = %d, k = %d, h = %d, outageWeek = %d\n", i, k, curH, outageWeek );

				if (outageWeek != -1)
				{
					if (outageWeek < h)
					{
						scheduler(i,outageWeek,k,outage,outageWeek);
						h = outageWeek;
					}
					else
					{
						scheduler(i,h,k,outage,outageWeek);
					}

					(*solution->type2PowerPlants)[i]->invalidadeWeeks(k+1, outageWeek, outageWeek+1);
					outageWeek = -1;
				}
				else
				{
					outageWeek = -2;
				}
			}

			if (outageWeek == h)
			{
				int timestep = h*ProblemData::WEEK_SIZE-1;
				double factor = ((*solution->type2PowerPlants)[i]->getQ(k+1) - 1.0) / (*solution->type2PowerPlants)[i]->getQ(k+1);
				double currentFuel = ( (*solution->type2PowerPlants)[i]->getFuelVariation(0, timestep) - (*solution->type2PowerPlants)[i]->getProduction(0, timestep)*ProblemData::D[timestep] );
				int refuel = MIN((*solution->type2PowerPlants)[i]->getRMAX(k+1), (int)((*solution->type2PowerPlants)[i]->getSMAX(k+1) - ( ( factor *(currentFuel - (*solution->type2PowerPlants)[i]->getBO(k) ) ) + (*solution->type2PowerPlants)[i]->getBO(k+1) ) ) );
				if (refuel < (*solution->type2PowerPlants)[i]->getRMIN(k+1))
				{
					(*solution->type2PowerPlants)[i]->invalidadeWeeks(k+1,h,h+1);
					outageWeek = -1;
				}
				else
				{
					o = new Outage(h, (*solution->type2PowerPlants)[i]->getDA(k+1), refuel, (*solution->type2PowerPlants)[i]->getIndex(), k+1);
					solution->addOutage(o);
					(*solution->type2PowerPlants)[i]->addOutage(o);
//					printf("solution added: i = %d, k = %d, h = %d\n", i, k+1, h);
					h = o->getEnd() - 1;
					outage = false;
					outageWeek = -1;
					k++;
					continue;
				}
			}

			for (int t = h*ProblemData::WEEK_SIZE; t < (h+1)*ProblemData::WEEK_SIZE; t++)
			{
				outage = (*solution->type2PowerPlants)[i]->addMaxProduction(0, t, k);
			}
		}
		curH = 0;
		k = -1;
		outage = false;
		outageWeek = -1;
	}
	for (int t = 0; t < ProblemData::T; t++)
	{
		for (int s = 1; s < ProblemData::S; s++)
		{
			for (int i = 0; i < ProblemData::I; i++)
			{
				(*solution->type2PowerPlants)[i]->setProduction(s,t, (*solution->type2PowerPlants)[i]->getProduction(0, t));
				(*solution->type2PowerPlants)[i]->setFuelVariation(s,t, (*solution->type2PowerPlants)[i]->getFuelVariation(0, t));
			}
		}
	}

	for (int i = 0; i < ProblemData::I; i++)
	{
		for (int s = 0; s < ProblemData::S; s++)
		{
			(*solution->type2PowerPlants)[i]->calculateRemainingFuel(s);
		}
	}

	scheduler(ProblemData::I, 0,0,0,0);

	if (o != NULL)
		solution->removeOutage(o);

}

void generateSolution()
{
	vector<int> ppt2Order;
	solution = new Solution();
	bestSolution = new Solution();

	//TODO: substituir pela ordenação, por exemplo, averageCost
	for (int i = 0; i < ProblemData::I; i++)
		ppt2Order.push_back(i);

	//PowerPlant1 **pp1 = (PowerPlant1 **)malloc(ProblemData::J * sizeof(PowerPlant1*));
	for (int j = 0; j < ProblemData::J; j++)
	{
//		printf("cloning type 1 pp #%d ...", j);
//		fflush(stdout);
		solution->addType1PowerPlant(ProblemData::type1PowerPlants[j]->clone(), j);
		bestSolution->addType1PowerPlant(ProblemData::type1PowerPlants[j]->clone(), j);
//		printf(" done\n");
//		fflush(stdout);
	}
//	printf("Done clonning type 1 pps\n");
//	fflush(stdout);

	for (int i = 0; i < ProblemData::I; i++)
	{
//		printf("cloning type 2 pp #%d ...", i);
//		fflush(stdout);

		solution->addType2PowerPlant(ProblemData::type2PowerPlants[i]->clone(), i);
		bestSolution->addType2PowerPlant(ProblemData::type2PowerPlants[i]->clone(), i);

//		printf(" done\n");
//		fflush(stdout);
	}

//	printf("Done clonning type 2 pps\n");
//	fflush(stdout);

	initializePpt1WithMinimumProduction();
	scheduler(0, 0, -1, false, -1);
	bestSolution->writeSolution(solutionFile, 18);

//	printf("ALL DONE!\n");
//	fflush(stdout);
}

/*void pseudoStuff(int currentPP, int currentCampaign)
{
	// se já não há mais tempo
	if(canKeepRunning() == false)
	{
		bestSolution->writeSolution(solutionFile, 18);
		exit(0);
	}

	// se já estão todas tratadas
	if(currentPP == ProblemData::I)
	{
		// add type 1 pp production
		bool valid = currentSolution->addType1PowerPlantsProduction();
		if(valid)
		{
			// first solution
			if(bestCost == 0.0)
				bestSolution->copySolution(currentSolution);
			else
			{
				double cost = currentSolution->calculateObjective();
				if(cost < bestCost)
				{
					bestCost = cost;
					bestSolution->copySolution(currentSolution);
				}
			}
		}
		return;
	}

	// se já acabou as campaigns muda de power plant
	if(currentCampaign == ProblemData::K)
	{
		pseudoStuff(currentPP+1, -1);
	}
}*/


int main(int argc, char** argv)
{
	if (argc == 1)
		return -1;

	// se -i for o único argumento devolve o ID da equipa
	if(argc == 2 && !strcmp(argv[1], "-i"))
	{
		printf("S18\n");
		return 18;
	}

	for(int i=1; i < argc; i += 2)
	{
		if(!strcmp(argv[i], "-t"))
		{
			// set max run time
			ProblemData::maxRunTime = atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i], "-n"))
		{
			// set data set
			strcpy(ProblemData::dataSet, argv[i+1]);
			strcpy(filepath, argv[i+1]);
		}
		else if(!strcmp(argv[i], "-r"))
		{
			// set solution file
			strcpy(solutionFile, argv[i+1]);
		}

		else if(!strcmp(argv[i], "-i"))
		{
			// print team ID
			printf("S18\n");
			i--;
		}
		else
			printf("Unknown arg '%s'\n", argv[i]);
	}

	// margem no run time
	ProblemData::runTimeMargin = 60;

//	printf("ProblemData::maxRunTime = %d\n", ProblemData::maxRunTime);
//	printf("ProblemData::dataSet = '%s'\n", ProblemData::dataSet);
//	printf("Solution file = '%s'\n", solutionFile);

	// ########## READING SECTION ##########
	FILE* inputFile = fopen(filepath, "r");
	if(inputFile == NULL)
	{
		printf("Data file not found... %s\n", filepath);
		exit(-1);
	}
	// main section
	readMainSection(inputFile);
	// type 1 power plant section
	readType1PowerPlants(inputFile);
	// type 2 power plant section
	readType2PowerPlants(inputFile);
	// constraints section
	readConstraints(inputFile);
	// close file
	fclose(inputFile);

	//ORDENAR POWERPLANTS 1 POR CUSTO
	ProblemData::sortType1PowerPlants();

	generateSolution();

/*
	// ########################### sort type 1 power plant ###########################
	ProblemData::sortType1PowerPlants();
	//ProblemData::printSortedPowerPlants();
	clock_t start = clock();
	bool valid = solution->addType1PowerPlantsProduction();
	if(valid)
		printf("Solution is valid in meeting demand\n\n");
	else
		printf("Solution is NOT valid in meeting demand!!!!!!!!!!!!\n\n");
	printf("\nTime to add type 1 PP productions: %lf\n\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);
	fflush(stdout);
	solution->writeSolution("abc.txt", 18);
	// ########################### END
*/

/*	int availableOutageTimestamp[ProblemData::I][ProblemData::K][ProblemData::T];
	for (int i=0; i < ProblemData::I; i++)
	{
		for (int k=0; k < ProblemData::K; k++)
			for (int t=0; t < ProblemData::T; t++)
				availableOutageTimestamp[i][k][t] = 1;

		vector<Constraint13*> *all = (*type2PowerPlants)[i].getCt13();
		for(int c = 0; c < all->size(); c++)
		{
			Constraint13* ct = (*all)[c];

			if (ct == NULL)
				continue;
			int k = ct->getCampaign();

			int first = ct->getTO();
			if (first == -1)
				first = 0;
			first *= ProblemData::WEEK_SIZE;
			int last = ct->getTA();

			if (last == -1)
				last = ProblemData::T;
			else
				last = (last + (*type2PowerPlants)[i].getDA()[k]) * ProblemData::WEEK_SIZE;

			for(int t = 0; t < first; t++)
				availableOutageTimestamp[i][k][t]--;

			for(int t = last; t < ProblemData::T; t++)
				availableOutageTimestamp[i][k][t]--;

		}

		printf("powerplant %d\n", i);
		for (int k=0; k < ProblemData::K; k++)
		{
			printf("campaign %d\t", k);
			for (int t=0; t < ProblemData::T; t++)
				printf(" %d", availableOutageTimestamp[i][k][t]);
			printf("\n");
		}

		printf("\n\n");



	}

	*/


	//generateSolutionFromInput();
	// ########## ALGORITHM SECTION ##########
	// insert winning algorithm here

	// free allocated memory => WHO CARES ABOUT DEALLOCATING MEMORY? ;)
//	int index;
//	free(ProblemData::D);
//	for (index = 0; index < ProblemData::S; index++)
//	{
//		free(ProblemData::DEM[index]);
//	}
//	free(ProblemData::DEM);
//
//	for (int j=0; j < ProblemData::J; j++){
//		type1PowerPlants[j].~PowerPlant1();
//	}
//	free(type1PowerPlants);
//
//	for (int i=0; i < ProblemData::I; i++){
//		(*type2PowerPlants)[i].~PowerPlant2();
//	}
//	free(type2PowerPlants);


	return 0;
}

