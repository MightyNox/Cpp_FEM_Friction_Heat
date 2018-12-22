#pragma once
#include "Input.h"

class Node
{
	unsigned long int id;
	long double x;
	long double y;
	long double t;
	bool onBoundNaturalConvection;
	bool onBoundForcedConvection;
	bool onBoundStream;
public:
	Node(unsigned long int id, long double x, long double y, bool onBoundNaturalConvection, bool onBoundForcedConvection, bool onBoundStream, long double initialTemperature);
	~Node();

	unsigned long int getId();
	long double getX();
	long double getY();
	long double getT();
	bool getOnBoundNaturalConvection();
	bool getOnBoundForcedConvection();
	bool getOnBoundStream();

	void setT(long double t);
};

