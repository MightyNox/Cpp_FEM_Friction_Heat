#pragma once
#include <array>

class UniversalElement
{
	std::array<std::array<long double, 4>, 4> N;
	std::array<std::array<std::array<long double, 4>, 2>, 4> NSurface;
	std::array<std::array<long double, 4>, 4> dNdEta;
	std::array<std::array<long double, 4>, 4> dNdKsi;

public:
	UniversalElement();
	~UniversalElement();

	std::array<std::array<long double, 4>, 4> & getN();
	std::array<std::array<std::array<long double, 4>, 2>, 4>  & getNSurface();
	std::array<std::array<long double, 4>, 4> & getdNdEta();
	std::array<std::array<long double, 4>, 4> & getdNdKsi();
};

