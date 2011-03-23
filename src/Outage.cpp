#include "Outage.h"

Outage::Outage(void)
{
}

Outage::Outage(int start, int duration, int refuelAmount, int ppt, int k)
{
	this->start = start;
	this->duration = duration;
	this->refuelAmount = refuelAmount;
	this->indexPowerPlant = ppt;
	this->campaign = k;
}


Outage::~Outage(void)
{
}

int Outage::getStart(){
	return start;
}
void Outage::setStart(int start){
	this->start = start;
}


int Outage::getEnd()
{
	return start + duration;
}

int Outage::getPowerPlantIndex()
{
	return this->indexPowerPlant;
}


void Outage::setPowerPlantIndex(int ppt)
{
	this->indexPowerPlant = ppt;
}
int Outage::getDuration(){
	return duration;
}

void Outage::setDuration(int duration){
	this->duration = duration;
}

double Outage::getRefuelAmount(){
	return refuelAmount;
}

void Outage::setRefuelAmount(double refuelAmount){
	this->refuelAmount = refuelAmount;
}

int Outage::getCampaign(){
	return campaign;
}
void Outage::setCampaign(int k){
	this->campaign = k;
}
