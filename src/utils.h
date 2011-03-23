#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define NAME_SIZE 30
#include "PowerPlant1.h"
#include "ProblemData.h"

#define MAX(x,y) (x > y)?x:y
#define MIN(x,y) (x < y)?x:y
#define SECONDS_IN_AN_HOUR    3600
#define SECONDS_IN_A_MINUTE   60

using namespace std;

class PowerPlant1;

double** allocDoubleMatrix(int l, int c);
int** allocIntMatrix(int l, int c);
int* allocIntVector(int c);
double* allocDoubleVector(int c);
void readIntVectorFromFile(FILE* f, int* vec, int size);
void readDoubleVectorFromFile(FILE* f, double* vec, int size);
double simple_round(double value);
int *** allocTripleIntArray(int firstDimension, int secondDimension, int thirdDimension);
// compare two power plants
int compareType1PowerPlants(const void *a, const void *b);
void writeDateToFile(FILE* outputFile);
void writeRunTimeToFile(FILE* outputFile);
void addValueToTime(int value, char *str);
bool canKeepRunning();

