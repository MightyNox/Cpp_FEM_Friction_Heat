#pragma once
#include "Node.h"
#include "Element.h"
#include <vector>

class Grid
{
	std::vector<Node*> gridNodes;
	std::vector<Element*> gridElements;
public:
	Grid();
	~Grid();

	void displayGrid();
};

