#include "utils.h"

double** allocDoubleMatrix(int l, int c){
	int index;
	double** tmp;
	tmp = (double **) malloc(l * sizeof(double*));

	for (index = 0; index < l; index++)
		tmp[index] = (double*) malloc(c * sizeof(double));
	return tmp;
}

int** allocIntMatrix(int l, int c){
	int index;
	int** tmp;
	tmp = (int **) malloc(l * sizeof(int*));

	for (index = 0; index < l; index++)
		tmp[index] = (int*) malloc(c * sizeof(int));
	return tmp;
}

int* allocIntVector(int c){
	int* tmp;

	tmp = (int *) malloc(c * sizeof(int));

	return tmp;
}

double* allocDoubleVector(int c){
	double* tmp;

	tmp = (double *) malloc(c * sizeof(double));

	return tmp;
}

void readIntVectorFromFile(FILE* f, int* vec, int size){
	for ( int i=0; i < size; i++ ){
		fscanf(f, " %d", &vec[i]);
	}
	fscanf(f, "\n");

}

void readDoubleVectorFromFile(FILE* f, double* vec, int size){
	for ( int i=0; i < size; i++ ){
		fscanf(f, " %lf", &vec[i]);
	}
	fscanf(f, "\n");

}

double simple_round(double value)
{
	return ((int)(round(value * 100))) / 100.0;
}

int *** allocTripleIntArray(int firstDimension, int secondDimension, int thirdDimension)
{
	int *** array = (int ***) malloc(sizeof(int **) * firstDimension);
	for(int i=0; i < firstDimension; i++)
		array[i] = allocIntMatrix(secondDimension, thirdDimension);

	return array;
}

// compare two power plants
int compareType1PowerPlants(const void *a, const void *b)
{
	int pp1Index = *((int *) a);
	int pp2Index = *((int *) b);


	double cost1 = ProblemData::type1PowerPlants[pp1Index]->getCost(ProblemData::sortingScenario, ProblemData::sortingTimeStep);
	double cost2 = ProblemData::type1PowerPlants[pp2Index]->getCost(ProblemData::sortingScenario, ProblemData::sortingTimeStep);

	if(cost1 > cost2)
		return 1;
	else if(cost1 == cost2)
		return 0;
	else
		return -1;
}

void writeDateToFile(FILE* outputFile)
{
	time_t rawtime;
	struct tm * timeinfo;
	char date[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(date, 80, "%d/%m/%Y %H:%M:%S", timeinfo);
	fprintf(outputFile, SOLUTION_DATE_PT, date);
}

void writeRunTimeToFile(FILE* outputFile)
{
	char runningTime[80] = "";
	int runTime = clock() / CLOCKS_PER_SEC;

//	printf("'%s'\n", runningTime);
//	fflush(stdout);
//	system("pause");

	addValueToTime(runTime / SECONDS_IN_AN_HOUR, runningTime);
	strcat(runningTime, ":");
	addValueToTime((runTime % SECONDS_IN_AN_HOUR) / SECONDS_IN_A_MINUTE, runningTime);
	strcat(runningTime, ":");
	addValueToTime((runTime % SECONDS_IN_AN_HOUR) % SECONDS_IN_A_MINUTE, runningTime);

	//printf("'%s'\n", runningTime);

	fprintf(outputFile, SOLUTION_RUNNING_TIME_PT, runningTime);
}

void addValueToTime(int value, char *str)
{
	char temp[3];
    if(value == 0)
    	sprintf(temp, "00");
    else if(value < 10)
		sprintf(temp, "0%d", value);
    else
    	sprintf(temp, "%d", value);
 	strcat(str, temp);
}

bool canKeepRunning()
{
	int runTime = clock() / CLOCKS_PER_SEC;

	if(runTime + ProblemData::runTimeMargin < ProblemData::maxRunTime)
		return true;
	else
		return false;
}
