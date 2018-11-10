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
		getline(file, line, ';');
		getline(file, line, '\n');
		gridHight = stoul(line);

		getline(file, line, ';');
		getline(file, line, '\n');
		gridWidth = stoul(line);
		
		getline(file, line, ';');
		getline(file, line, '\n');
		verticalNodeNumber = stoul(line);

		getline(file, line, ';');
		getline(file, line, '\n');
		horizontalNodeNumber = stoul(line);

		getline(file, line, ';');
		getline(file, line, '\n');
		conductionRatio = stold(line);

		getline(file, line, ';');
		getline(file, line, '\n');
		ambientTemperature = stol(line);

		getline(file, line, ';');
		getline(file, line, '\n');
		convectionRatio = stold(line);

		file.close();
	}
}


Input::~Input()
{
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


long double Input::getTemperature()
{
	return ambientTemperature;
}


long double Input::getConductionRatio()
{
	return conductionRatio;
}


long double Input::getConvectionRatio()
{
	return convectionRatio;
}
