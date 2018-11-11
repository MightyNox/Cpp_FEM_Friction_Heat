#pragma once
#include "GlobalNode.h"
#include "Element.h"
#include "UniversalElement.h"
#include <vector>

class Grid
{
	Input input;
	std::vector<GlobalNode*> nodes;
	std::vector<Element*> elements;
	UniversalElement universalElement;

public:
	Grid();
	~Grid();

	void calculate();
	void displayGrid();
	void displayElement(unsigned long int id);
	void displayUniversalElement();
};

