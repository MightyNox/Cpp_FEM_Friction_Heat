#include "Element.h"
#include "Input.h"

Element::Element(unsigned long int id, std::array<Node *, 4> &globalNodes, UniversalElement *universalElement, Input *input)
{
	//Set element attributes
	{
		this->id = id;
		this->globalNodes = globalNodes;
	}

	//Create local matrix H oject
	{
		localH = new LocalH(globalNodes, universalElement, input);
	}

	//Create local matrix C oject
	{
		localC = new LocalC(globalNodes, universalElement, input);
	}

	//Create local vector P oject
	{
		localP = new LocalP(globalNodes, universalElement, input);
	}
}


Element::~Element()
{
	delete localH;
	delete localC;
	delete localP;
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