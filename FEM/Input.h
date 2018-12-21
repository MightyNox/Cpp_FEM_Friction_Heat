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
	long double frictionRatio; //my
	long double earthAcceleration; //g
	long double cubeHeight; //h
	long double cubeArea; //A
	long double velocity; //v
	long double angle; //beta

	long double densityStream; //q

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
	long double getFrictionRatio();
	long double getEarthAcceleration();
	long double getCubeHeight();
	long double getCubeArea();
	long double getVelocity();
	long double getAngle();

	long double getDensityStream();
};