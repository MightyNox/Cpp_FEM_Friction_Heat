#pragma once
#include <array>
#include "Node.h"
#include "UniversalElement.h"
#include "Input.h"

class LocalP
{
	std::array <long double, 4> vector;
	std::array <Node *, 4> globalNodes;
	UniversalElement *universalElement;
	Input *input;
public:
	LocalP(std::array <Node *, 4> globalNodes, UniversalElement *universalElement, Input *input);
	~LocalP();

	void calculate();
	std::array <long double, 4> getVector();
};

