#pragma once
#include "GlobalNode.h"
#include "Element.h"
#include "UniversalElement.h"
#include "Display.h"
#include <vector>

class Grid
{
	Input input;
	Display display;
	std::vector<GlobalNode*> nodes;
	std::vector<Element*> elements;
	UniversalElement universalElement;

public:
	Grid();
	~Grid();

	void calculate();
	long double * gaussSeidel(long double **A, long double *B, unsigned long int n, unsigned long int iterations);
};

