#include "UniversalElement.h"
#include <cmath>


UniversalElement::UniversalElement()
{
	long double val = 1 / sqrt(3);
	long double ksi[] = { val*(-1), val, val, val*(-1) };
	long double eta[] = { val*(-1), val*(-1), val, val };

	for (int i = 0; i < 4; i++)
	{
		N[i][0] = 0.25*(1 - ksi[i])*(1 - eta[i]);
		N[i][1] = 0.25*(1 + ksi[i])*(1 - eta[i]);
		N[i][2] = 0.25*(1 + ksi[i])*(1 + eta[i]);
		N[i][3] = 0.25*(1 - ksi[i])*(1 + eta[i]);

		NdEta[0][i] = (-0.25)*(1 - eta[i]);
		NdEta[1][i] = (0.25)*(1 - eta[i]);
		NdEta[2][i] = (0.25)*(1 + eta[i]);
		NdEta[3][i] = (-0.25)*(1 + eta[i]);

		NdKsi[0][i] = (-0.25)*(1 - ksi[i]);
		NdKsi[1][i] = (-0.25)*(1 + ksi[i]);
		NdKsi[2][i] = (0.25)*(1 + ksi[i]);
		NdKsi[3][i] = 0.25*(1 - ksi[i]);
	}
}


UniversalElement::~UniversalElement()
{
}


std::array<std::array<long double, 4>, 4> & UniversalElement::getN()
{
	return N;
}


std::array<std::array<long double, 4>, 4> & UniversalElement::getNdEta()
{
	return NdEta;
}


std::array<std::array<long double, 4>, 4> & UniversalElement::getNdKsi()
{
	return NdKsi;
}