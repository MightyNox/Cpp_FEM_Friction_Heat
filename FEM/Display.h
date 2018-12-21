#pragma once
#include "Input.h"
#include "Node.h"
#include "Element.h"
#include "UniversalElement.h"
#include <vector>

class Display
{
public:
	Display();
	~Display();

	void iteration(unsigned long int x);
	void temperature(Input &input, std::vector<Node*> nodes);
	void H(unsigned long int nodeCount, long double **globalH);
	void P(unsigned long int nodeCount, long double *globalP);
	void grid(std::vector<Element*> elements, std::vector<Node*> nodes);
	void universalElement(UniversalElement universalElement);
	void minmax(std::vector<long double> min, std::vector<long double> max);
};

