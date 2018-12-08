#pragma once
#include <array>

class UniversalElement
{
	std::array<std::array<long double, 4>, 4> N;
	std::array<std::array<long double, 4>, 4> NSurface;
	std::array<std::array<long double, 4>, 4> dNdKsi;
	std::array<std::array<long double, 4>, 4> dNdEta;

public:
	UniversalElement();
	~UniversalElement();

	std::array<std::array<long double, 4>, 4> & getN();
	std::array<std::array<long double, 4>, 4>  & getNSurface();
	std::array<std::array<long double, 4>, 4> & getdNdKsi();
	std::array<std::array<long double, 4>, 4> & getdNdEta();
};

