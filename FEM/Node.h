#pragma once
#include "Input.h"

class Node
{
	unsigned long int id;
	long double x;
	long double y;
	long double t;
	bool onBoundConvection;
	bool onBoundStream;
public:
	Node(unsigned long int id, long double x, long double y, bool onBoundConvection, bool onBoundStream, Input *input);
	~Node();

	unsigned long int getId();
	long double getX();
	long double getY();
	long double getT();
	bool getOnBoundConvection();
	bool getOnBoundStream();

	void setT(long double t);
};

