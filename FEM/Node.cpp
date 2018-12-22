#include "Node.h"
#include "Input.h"


Node::Node(unsigned long int id, long double x, long double y, bool onBoundNaturalConvection, bool onBoundForcedConvection, bool onBoundStream, long double initialTemperature)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->t = initialTemperature;
	this->onBoundNaturalConvection = onBoundNaturalConvection;
	this->onBoundForcedConvection = onBoundForcedConvection;
	this->onBoundStream = onBoundStream;
}


Node::~Node()
{
}


unsigned long int Node::getId()
{
	return id;
}


long double Node::getX()
{
	return x;
}


long double Node::getY()
{
	return y;
}


long double Node::getT()
{
	return t;
}


bool Node::getOnBoundNaturalConvection()
{
	return onBoundNaturalConvection;
}


bool Node::getOnBoundForcedConvection()
{
	return onBoundForcedConvection;
}


bool Node::getOnBoundStream()
{
	return onBoundStream;
}


void Node::setT(long double t)
{
	this->t = t;
}