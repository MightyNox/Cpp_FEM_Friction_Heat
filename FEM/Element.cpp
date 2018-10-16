#include "Element.h"
#include "Input.h"


Element::Element(unsigned long int id, int * nodesId)
{
	this->id = id;
	this->nodesId = nodesId;
	this->conductionRatio = Input::getConductionRatio();
}


Element::~Element()
{
	delete nodesId;
}


unsigned long int Element::getId()
{
	return id;
}


int * Element::getNodesId()
{
	return nodesId;
}