#pragma once
#include "GlobalNode.h"
#include "Element.h"
#include "ShapeFunction.h"
#include <vector>

class Grid
{
	std::vector<GlobalNode*> nodes;
	std::vector<Element*> elements;
	ShapeFunction shapeFunction;

public:
	Grid();
	~Grid();

	void displayGrid();
	void displayElement(unsigned long int id);
	void displayShapeFunction();
};

