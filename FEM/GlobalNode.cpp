#include "GlobalNode.h"
#include "Input.h"


GlobalNode::GlobalNode(unsigned long int id, long double x, long double y)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->t = Input::getTemperature();
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