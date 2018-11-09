#pragma once
#include <array>

class UniversalElement
{
	std::array<std::array<long double, 4>, 4> N;
	std::array<std::array<long double, 4>, 4> NdEta;
	std::array<std::array<long double, 4>, 4> NdKsi;

public:
	UniversalElement();
	~UniversalElement();

	std::array<std::array<long double, 4>, 4> & getN();
	std::array<std::array<long double, 4>, 4> & getNdEta();
	std::array<std::array<long double, 4>, 4> & getNdKsi();
};

