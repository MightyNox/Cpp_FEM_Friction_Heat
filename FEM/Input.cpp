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

		//Forced Convection Ratio
		getline(file, line, ';');
		getline(file, line, '\n');
		forcedConvectionRatio = stold(line);

		//Natural Convection Ratio
		getline(file, line, ';');
		getline(file, line, '\n');
		naturalConvectionRatio = stold(line);
		
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

		//Gauss Seidel Iterations Number
		getline(file, line, ';');
		getline(file, line, '\n');
		iterationsNumber = stoul(line);

		//Friction Ratio
		getline(file, line, ';');
		getline(file, line, '\n');
		frictionRatio = stold(line);

		//Earth Acceleration
		getline(file, line, ';');
		getline(file, line, '\n');
		earthAcceleration = stold(line);

		//Velocity
		getline(file, line, ';');
		getline(file, line, '\n');
		velocity = stold(line);

		//Angle
		getline(file, line, ';');
		getline(file, line, '\n');
		angle = stold(line);

		//PI
		getline(file, line, ';');
		getline(file, line, '\n');
		PI = stold(line);

		file.close();
	}

	basicAcceleration = earthAcceleration * (sin(angle*PI / 180.0) - frictionRatio * cos(angle*PI / 180.0));
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


long double Input::getForcedConvectionRatio()
{
	return forcedConvectionRatio;
}


long double Input::getNaturalConvectionRatio()
{
	return naturalConvectionRatio;
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


unsigned long int Input::getIterationsNumber()
{
	return iterationsNumber;
}


long double Input::getFrictionRatio()
{
	return frictionRatio;
}


long double Input::getEarthAcceleration()
{
	return earthAcceleration;
}


long double Input::getVelocity()
{
	return velocity;
}


long double Input::getAngle()
{
	return angle;
}


long double Input::getDensityStream()
{
	return densityStream;
}


void Input::addVelocityStep()
{
	long double velocityStep;
	long double resistAcceleration;
	for (int i = 0; i < 1000; i++)
	{
		resistAcceleration = 1.05 *velocity*velocity*0.5 / gridWidth;
		velocityStep = (basicAcceleration - resistAcceleration)*simulationStepTime / 1000;
		velocity += velocityStep;
	}
}


void Input::caluclateDensityStream()
{
	densityStream = frictionRatio * density  * gridHight * earthAcceleration * velocity * cos(angle*PI / 180.0);
}