#pragma once
#include <fstream>

class Input
{
	std::fstream file;
	
	long double initialTemperature; //t0
	long double simulationTime; //tau
	long double simulationStepTime; //delta tau
	long double ambientTemperature; //tInf
	long double forcedConvectionRatio; //alphaF
	long double naturalConvectionRatio; //alphaN
	long double gridHight; //H
	long double gridWidth; //L
	unsigned long int verticalNodeNumber; //nH
	unsigned long int horizontalNodeNumber; //nL
	long double specificHeat; //c
	long double conductionRatio; //k
	long double density; //rho
	unsigned long int iterationsNumber;
	long double frictionRatio; //my
	long double earthAcceleration; //g
	long double velocity; //v
	long double velocityStep;
	long double angle; //beta
	long double PI;
	long double densityStream; //q

public:
	Input();
	~Input();

	long double getInitialTemperature();
	long double getSimulationTime();
	long double getSimulationStepTime();
	long double getAmbientTemperature();
	long double getForcedConvectionRatio();
	long double getNaturalConvectionRatio();
	long double getGridHight();
	long double getGridWidth();
	unsigned long int getVerticalNodeNumber();
	unsigned long int getHorizontalNodeNumber();
	long double getSpecificHeat();
	long double getConductionRatio();
	long double getDensity();
	unsigned long int getIterationsNumber();
	long double getFrictionRatio();
	long double getEarthAcceleration();
	long double getVelocity();
	long double getAngle();
	long double getDensityStream();

	void addVelocityStep();
	void caluclateDensityStream();
};