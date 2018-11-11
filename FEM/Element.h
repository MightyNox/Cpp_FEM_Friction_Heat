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
	std::array < std::array <long double, 4>, 4> localH;
	std::array < std::array <long double, 4>, 4> localC;
	std::array <long double, 4> localP;
public:
	Element(unsigned long int id, std::vector<GlobalNode *> &globalNodes, UniversalElement universalELement, Input *input);
	~Element();

	unsigned long int getId();
	std::vector <GlobalNode *> & getGlobalNodes();
	std::vector <LocalNode *> & getLocalNodes();
	std::array<std::array<long double, 4>, 4> & getLocalH();
	std::array<std::array<long double, 4>, 4> & getLocalC();
	std::array <long double, 4> & getLocalP();
};


