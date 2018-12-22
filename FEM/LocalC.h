#pragma once
#include <array>
#include "Node.h"
#include "UniversalElement.h"
#include "Input.h"

class LocalC
{
	std::array< std::array <long double, 4>, 4> matrix;
	std::array <Node *, 4> globalNodes;
	UniversalElement *universalElement;
	Input *input;
public:
	LocalC(std::array <Node *, 4> globalNodes, UniversalElement *universalElement, Input *input);
	~LocalC();

	void calculate();
	std::array< std::array <long double, 4>, 4> getMatrix();
};

