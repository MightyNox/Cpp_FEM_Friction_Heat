#pragma once
#include "Node.h"
#include "UniversalElement.h"
#include "LocalH.h"
#include "LocalC.h"
#include "LocalP.h"
#include "Input.h"
#include <vector>
#include <array>

class Element
{
	unsigned long int id;
	std::array <Node *, 4> globalNodes;
	LocalH *localH;
	LocalC *localC;
	LocalP *localP;
public:
	Element(unsigned long int id, std::array<Node *, 4> &globalNodes, UniversalElement *universalELement, Input *input);
	~Element();

	unsigned long int getId();
	std::array <Node *, 4> & getGlobalNodes();
	LocalH * getLocalH();
	LocalC * getLocalC();
	LocalP * getLocalP();
};


