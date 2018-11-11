#include "Input.h"
#include "FileNotFoundException.h"
#include <string>

Input::Input()
{
	file.open("Files/input.csv", std::ios::in);
	if (!file.good())
	{
		throw FileNotFoundException();
	}
	else
	{
		std::string line;
		//Initial Temperature
		getline(file, line, ';');
		getline(file, line, '\n');
		initialTemperature = stold(line);

		//Simulation Time
		getline(file, line, ';');
		getline(file, line, '\n');
		simulationTime = stold(line);

		//Simulation Step Time
		getline(file, line, ';');
		getline(file, line, '\n');
		simulationStepTime = stold(line);

		//Ambient Temperature
		getline(file, line, ';');
		getline(file, line, '\n');
		ambientTemperature = stold(line);

		//Convection Ratio
		getline(file, line, ';');
		getline(file, line, '\n');
		convectionRatio = stold(line);
		
		//Grid Hight
		getline(file, line, ';');
		getline(file, line, '\n');
		gridHight = stold(line);

		//Grid Width
		getline(file, line, ';');
		getline(file, line, '\n');
		gridWidth = stold(line);

		//Vertical Node Number
		getline(file, line, ';');
		getline(file, line, '\n');
		verticalNodeNumber = stoul(line);

		//Horizontal Node Number
		getline(file, line, ';');
		getline(file, line, '\n');
		horizontalNodeNumber = stoul(line);

		//Specific Heat
		getline(file, line, ';');
		getline(file, line, '\n');
		specificHeat = stold(line);

		//Conduction Ratio
		getline(file, line, ';');
		getline(file, line, '\n');
		conductionRatio = stold(line);

		//Density
		getline(file, line, ';');
		getline(file, line, '\n');
		density = stold(line);

		file.close();
	}
}


Input::~Input()
{
}


long double Input::getInitialTemperature()
{
	return initialTemperature;
}


long double Input::getSimulationTime()
{
	return simulationTime;
}


long double Input::getSimulationStepTime()
{
	return simulationStepTime;
}


long double Input::getAmbientTemperature()
{
	return ambientTemperature;
}


long double Input::getConvectionRatio()
{
	return convectionRatio;
}


long double Input::getGridHight()
{
	return gridHight;
}


long double Input::getGridWidth()
{
	return gridWidth;
}


unsigned long int Input::getVerticalNodeNumber()
{
	return verticalNodeNumber;
}


unsigned long int Input::getHorizontalNodeNumber()
{
	return horizontalNodeNumber;
}


long double Input::getSpecificHeat()
{
	return specificHeat;
}


long double Input::getConductionRatio()
{
	return conductionRatio;
}


long double Input::getDensity()
{
	return density;
}

