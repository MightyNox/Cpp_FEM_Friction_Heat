#pragma once
#include "GlobalNode.h"
#include "LocalNode.h"
#include "UniversalElement.h"
#include "Input.h"
#include <vector>
#include <array>

class Element
{
	unsigned long int id;
	std::vector <GlobalNode *> globalNodes;
	std::vector <LocalNode *> localNodes;
	std::array <long double, 4> globalHeat;
	long double conductionRatio;
	long double convectionRatio;
public:
	Element(unsigned long int id, std::vector<GlobalNode *> &globalNodes, UniversalElement universalELement, Input *input);
	~Element();

	unsigned long int getId();
	std::vector <GlobalNode *> & getGlobalNodes();
	std::vector <LocalNode *> & getLocalNodes();
};


