#pragma once
#include "GlobalNode.h"
#include "LocalNode.h"
#include "UniversalElement.h"
#include <vector>
#include <array>

class Element
{
	unsigned long int id;
	std::vector <GlobalNode *> globalNodes;
	std::vector <LocalNode *> localNodes;
	std::array <long double, 4> globalHeat;

	long double conductionRatio;
public:
	Element(unsigned long int id, std::vector<GlobalNode *> &globalNodes, UniversalElement universalELement);
	~Element();

	unsigned long int getId();
	std::vector <GlobalNode *> & getNodes();
	std::vector <LocalNode *> & getLocalNodes();
};


