#include "Constraint.h"
#include "filePatterns.h"

Constraint::Constraint()
{

}

/*Constraint::Constraint(FILE* inputFile)
{
	fscanf(inputFile, beginConstraint_pt);
	fscanf(inputFile, type_pt, &type);
	fscanf(inputFile, index_pt, &index);
}*/

Constraint::~Constraint(void)
{
}

void Constraint::readHeader(FILE* inputFile)
{
	fscanf(inputFile, beginConstraint_pt);
	fscanf(inputFile, type_pt, &type);
	fscanf(inputFile, index_pt, &index);
}

