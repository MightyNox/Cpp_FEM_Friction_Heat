#pragma once
#include <fstream>

class Input
{
	std::fstream file;
	static long double gridHight;
	static long double gridWidth;
	static unsigned long int verticalNodeNumber;
	static unsigned long int horizontalNodeNumber;
	static long double conductionRatio;
	static long double ambientTemperature;
public:
	Input();
	~Input();

	static long double getGridHight();
	static long double getGridWidth();
	static unsigned long int getVerticalNodeNumber();
	static unsigned long int getHorizontalNodeNumber();
	static long double getTemperature();
	static long double getConductionRatio();
};