#pragma once
class GlobalNode
{
	unsigned long int id;
	long double x;
	long double y;
	long double t;
public:
	GlobalNode(unsigned long int id, long double x, long double y);
	~GlobalNode();

	unsigned long int getId();
	long double getX();
	long double getY();
};

