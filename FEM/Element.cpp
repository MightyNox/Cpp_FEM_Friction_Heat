#include "Element.h"
#include "Input.h"

Element::Element(unsigned long int id, std::array<Node *, 4> &globalNodes, UniversalElement universalElement, Input *input)
{
	//Set element attributes
	{
		this->id = id;
		this->globalNodes = globalNodes;
	}

	//Calculate local matrix H
	{
		localH = new LocalH(globalNodes, universalElement, input);
	}
	
	//Calculate local matrix C
	{
		localC = new LocalC(globalNodes, universalElement, input);
	}

	//Calculate local vector P
	{
		localP = new LocalP(globalNodes, universalElement, input);
	}
}


Element::~Element()
{
	delete localH;
	delete localC;
}


unsigned long int Element::getId()
{
	return id;
}


std::array<Node *, 4> & Element::getGlobalNodes()
{
	return globalNodes;
}


LocalH * Element::getLocalH()
{
	return localH;
}


LocalC * Element::getLocalC()
{
	return localC;
}


LocalP * Element::getLocalP()
{
	return localP;
}