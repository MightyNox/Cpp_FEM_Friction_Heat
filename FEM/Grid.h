#pragma once
#include "Node.h"
#include "Element.h"
#include "UniversalElement.h"
#include "Display.h"
#include "EquationSolver.h"
#include <vector>

class Grid
{
	Input input;
	Display display;
	std::vector<Node*> nodes;
	std::vector<Element*> elements;
	UniversalElement universalElement;
	EquationSolver equationSolver;
public:
	Grid();
	~Grid();

	void calculate();
	long double * gaussSeidel(long double **A, long double *B, unsigned long int n, unsigned long int iterations);
};

