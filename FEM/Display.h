#pragma once
#include "Input.h"
#include "GlobalNode.h"
#include "Element.h"
#include "UniversalElement.h"
#include <vector>

class Display
{
public:
	Display();
	~Display();

	void iteration(unsigned long int x);
	void temperature(Input &input, std::vector<GlobalNode*> nodes);
	void H(unsigned long int nodeCount, long double **globalH);
	void P(unsigned long int nodeCount, long double *globalP);
	void grid(std::vector<Element*> elements, std::vector<GlobalNode*> nodes);
	void element(std::vector<Element*> elements, unsigned long int id);
	void universalElement(UniversalElement universalElement);
};

