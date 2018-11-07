#pragma once
#include "GlobalNode.h"
#include "LocalNode.h"
#include "ShapeFunction.h"
#include <vector>
#include <array>

class Element
{
	unsigned long int id;
	std::vector <GlobalNode *> globalNodes;
	std::vector <LocalNode *> localNodes;

	long double conductionRatio;
public:
	Element(unsigned long int id, std::vector<GlobalNode *> &nodes, ShapeFunction shapeFunction);
	~Element();

	unsigned long int getId();
	std::vector <GlobalNode *> & getNodes();
	std::vector <LocalNode *> & getLocalNodes();
};


