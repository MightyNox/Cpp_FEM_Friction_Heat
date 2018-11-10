#include "GlobalNode.h"
#include "Input.h"


GlobalNode::GlobalNode(unsigned long int id, long double x, long double y, bool onBound, Input *input)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->t = input->getTemperature();
	this->onBound = onBound;
}


GlobalNode::~GlobalNode()
{
}


unsigned long int GlobalNode::getId()
{
	return id;
}


long double GlobalNode::getX()
{
	return x;
}


long double GlobalNode::getY()
{
	return y;
}


bool GlobalNode::getOnBound()
{
	return onBound;
}