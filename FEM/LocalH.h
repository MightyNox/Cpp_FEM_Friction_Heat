#pragma once
#include <array>
#include "Node.h"
#include "UniversalElement.h"
#include "Input.h"

class LocalH
{
	std::array< std::array <long double, 4>, 4> matrix;
	std::array <Node *, 4> globalNodes;
	UniversalElement *universalElement;
	Input *input;
public:
	LocalH(std::array <Node *, 4> globalNodes, UniversalElement *universalElement, Input *input);
	~LocalH();

	void calculate();
	std::array< std::array <long double, 4>, 4> getMatrix();
};

