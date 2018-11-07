#pragma once
#include <array>

class ShapeFunction
{
	std::array<std::array<long double, 4>, 4> N;
	std::array<std::array<long double, 4>, 4> NdEta;
	std::array<std::array<long double, 4>, 4> NdKsi;

public:
	ShapeFunction();
	~ShapeFunction();

	std::array<std::array<long double, 4>, 4> & getN();
	std::array<std::array<long double, 4>, 4> & getNdEta();
	std::array<std::array<long double, 4>, 4> & getNdKsi();
};

