#pragma once
#include <array>
#include "Node.h"
#include "UniversalElement.h"
#include "Input.h"

class LocalH
{
	std::array< std::array <long double, 4>, 4> matrix;
public:
	LocalH(std::array <Node *, 4> globalNodes, UniversalElement &universalElement, Input *input);
	~LocalH();

	std::array< std::array <long double, 4>, 4> getMatrix();
};

