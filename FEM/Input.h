#pragma once
#include <fstream>

class Input
{
	std::fstream file;
	long double gridHight; //H
	long double gridWidth; //L
	unsigned long int verticalNodeNumber; //nH
	unsigned long int horizontalNodeNumber; //nL
	long double conductionRatio; //k
	long double ambientTemperature; //t0
	long double convectionRatio; //alpha
public:
	Input();
	~Input();

	long double getGridHight();
	long double getGridWidth();
	unsigned long int getVerticalNodeNumber();
	unsigned long int getHorizontalNodeNumber();
	long double getTemperature();
	long double getConductionRatio();
	long double getConvectionRatio();
};