#pragma once
#include <fstream>

class Input
{
	std::fstream file;
	
	long double initialTemperature; //t0
	long double simulationTime; //tau
	long double simulationStepTime; //delta tau
	long double ambientTemperature; //tInf
	long double convectionRatio; //alpha
	long double gridHight; //H
	long double gridWidth; //L
	unsigned long int verticalNodeNumber; //nH
	unsigned long int horizontalNodeNumber; //nL
	long double specificHeat; //c
	long double conductionRatio; //k
	long double density; //rho
	unsigned long int iterationsNumber;
public:
	Input();
	~Input();

	long double getInitialTemperature();
	long double getSimulationTime();
	long double getSimulationStepTime();
	long double getAmbientTemperature();
	long double getConvectionRatio();
	long double getGridHight();
	long double getGridWidth();
	unsigned long int getVerticalNodeNumber();
	unsigned long int getHorizontalNodeNumber();
	long double getSpecificHeat();
	long double getConductionRatio();
	long double getDensity();
	unsigned long int getIterationsNumber();
};