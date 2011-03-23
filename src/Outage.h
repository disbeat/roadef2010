#pragma once

class Outage
{
	int start;
	int duration;
	double refuelAmount;
	int indexPowerPlant;
	int campaign;

public:

	int getStart();
	void setStart(int start);
	int getEnd();
	int getPowerPlantIndex();
	void setPowerPlantIndex(int ppt);
	int getDuration();
	void setDuration(int duration);
	double getRefuelAmount();
	void setRefuelAmount(double refuelAmount);
	int getCampaign();
	void setCampaign(int k);

	Outage(int start, int duration, int refuelAmount, int ppt, int k);
	Outage(void);
	~Outage(void);
};
