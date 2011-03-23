#include "ProblemData.h"

int ProblemData::T = 0;
int ProblemData::H = 0;
int ProblemData::K = 0;
int ProblemData::S = 0;
double ProblemData::E = 0;
int ProblemData::J = 0;
int ProblemData::I = 0;
int* ProblemData::D = 0;
int ProblemData::M[N_CONSTRAINT] = {0,0,0,0,0,0,0,0,0};
double** ProblemData::DEM = NULL;
int ProblemData::WEEK_SIZE = 0;
int ProblemData::maxRunTime = 0;
int ProblemData::runTimeMargin = 60;
char ProblemData::dataSet[500];
int *** ProblemData::sortedType1PP = NULL;
int ProblemData::sortingScenario = 0;
int ProblemData::sortingTimeStep = 0;
PowerPlant1 **ProblemData::type1PowerPlants = NULL;
PowerPlant2 **ProblemData::type2PowerPlants = NULL;




void ProblemData::sortType1PowerPlants()
{
	// init sortedType1PP
	sortedType1PP = allocTripleIntArray(ProblemData::S, ProblemData::T, ProblemData::J);
	for(int i = 0; i < ProblemData::S; i++)
		for(int k = 0; k < ProblemData::T; k++)
			for(int m = 0; m < ProblemData::J; m++)
				sortedType1PP[i][k][m] = m;

	// sort
//	printf("Sorting ...\n\n");
//	fflush(stdout);
	clock_t start;
	start = clock();
	for(int i = 0; i < ProblemData::S; i++)
	{
		//printf("\tSorting scenario: %d ...\n\n", i);
		//scenarioStart = clock();

		ProblemData::sortingTimeStep = 0;
		for(int k = 0; k < ProblemData::T; k++)
		{
			//printf("\t\tSorting time step: %d ...", k);
			//timeStepStart = clock();
			qsort(sortedType1PP[i][k], ProblemData::J, sizeof(int), compareType1PowerPlants);
			//printf(" done! Sorting time: %lf\n\n", ((double) (clock() - timeStepStart)) / CLOCKS_PER_SEC);
			ProblemData::sortingTimeStep++;
		}

		//printf("\tSorting time: %lf\n\n", ((double) (clock() - scenarioStart)) / CLOCKS_PER_SEC);
		ProblemData::sortingScenario++;
	}

//	printf("\nTotal sorting time: %lf\n\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);
//	fflush(stdout);
}

void ProblemData::printSortedPowerPlants()
{
	int index;
	double cost;

	for(int i = 0; i < ProblemData::S; i++)
	{
		printf("\n\nScenario %d:\n", i);
		for(int k = 0; k < ProblemData::T; k++)
		{
			printf("\n\tTime step %d:\n", k);
			for(int m = 0; m < ProblemData::J; m++)
			{
				index = sortedType1PP[i][k][m];
				cost = ProblemData::type1PowerPlants[index]->getCost(i, k);
				printf("\t\tcost: %f     index: %d\n", cost, index);
			}

//			fflush(stdout);
//			system("pause");
		}
	}
}



