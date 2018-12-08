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
	std::array <GlobalNode *, 4> globalNodes;
	std::array <LocalNode *, 4> localNodes;
	std::array < std::array <long double, 4>, 4> localH;
	std::array < std::array <long double, 4>, 4> localC;
	std::array <long double, 4> localP;
public:
	Element(unsigned long int id, std::array<GlobalNode *, 4> &globalNodes, UniversalElement universalELement, Input *input);
	~Element();

	unsigned long int getId();
	std::array <GlobalNode *, 4> & getGlobalNodes();
	std::array <LocalNode *, 4> & getLocalNodes();
	std::array<std::array<long double, 4>, 4> & getLocalH();
	std::array<std::array<long double, 4>, 4> & getLocalC();
	std::array <long double, 4> & getLocalP();
};


