#include "Profile.h"
#include "filePatterns.h"
#include "utils.h"


Profile::Profile(FILE* inputFile, bool isCurrent, int BO)
{
	if (isCurrent){
		index = -1;
		fscanf(inputFile, begin_ccProfile_pt);
	}else{
		fscanf(inputFile, beginProfile_pt);
		fscanf(inputFile, profileCampaignIndex_pt, &index);
	}

	fscanf(inputFile, profilePoints_pt, &NP);

	fscanf(inputFile, decreaseProfile_pt);

	int temp_fPB;
	double temp_cPB;

	fscanf(inputFile, " %d", &temp_fPB);
	fscanf(inputFile, " %lf", &temp_cPB);
	int start = 1;
	if (temp_fPB < BO)
	{
		NP++;
		start++;
		fPB = allocIntVector(NP);
		cPB = allocDoubleVector(NP);
		fPB[0] = BO;
		cPB[0] = 1.0;
		fPB[1] = temp_fPB;
		cPB[1] = temp_cPB;

	}else
	{
		fPB = allocIntVector(NP);
		cPB = allocDoubleVector(NP);
		fPB[0] = temp_fPB;
		cPB[0] = temp_cPB;
	}

	for (int i = start; i < NP; i++){
		fscanf(inputFile, " %d", &fPB[i]);
		fscanf(inputFile, " %lf", &cPB[i]);
	}
	fscanf(inputFile, "\n");

	if (isCurrent){
		fscanf(inputFile, end_ccProfile_pt);
	}else{
		fscanf(inputFile, endProfile_pt);
	}
}

Profile::~Profile(void)
{
	free(cPB);
	free(fPB);
}


int Profile::getNP()
{
	return NP;
}

int Profile::getFPB(int i)
{
	return fPB[i];
}

double Profile::getCPB(int i)
{
	return cPB[i];
}
