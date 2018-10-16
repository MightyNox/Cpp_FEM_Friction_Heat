#include "Node.h"
#include "Input.h"


Node::Node(long double x, long double y)
{
	this->x = x;
	this->y = y;
	this->t = Input::getTemperature();
}


Node::~Node()
{
}
