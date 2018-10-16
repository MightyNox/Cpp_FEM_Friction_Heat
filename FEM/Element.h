#pragma once
#include "Node.h"

class Element
{
	unsigned long int id;
	int * nodesId;
	long double conductionRatio;
public:
	Element(unsigned long int id, int * nodesId);
	~Element();

	unsigned long int getId();
	int * getNodesId();
};

