#include "UniversalElement.h"
#include <cmath>


UniversalElement::UniversalElement()
{
	long double val = 1 / sqrt(3);

	long double ksi[] = { val*(-1), val, val, val*(-1) };
	long double eta[] = { val*(-1), val*(-1), val, val };

	long double ksiSurface[] = { -val, 1, val, -1 };
	long double etaSurface[] = { -1, -val, 1, val };

	for (int i = 0; i < 4; i++)
	{
		//Calculate N1, N2, N3, N4 in Volume
		N[i][0] = 0.25*(1 - ksi[i])*(1 - eta[i]);
		N[i][1] = 0.25*(1 + ksi[i])*(1 - eta[i]);
		N[i][2] = 0.25*(1 + ksi[i])*(1 + eta[i]);
		N[i][3] = 0.25*(1 - ksi[i])*(1 + eta[i]);

		//Calculate N1, N2, N3, N4 in Surface
		NSurface[i][0] = 0.25*(1 - ksiSurface[i])*(1 - etaSurface[i]);
		NSurface[i][1] = 0.25*(1 + ksiSurface[i])*(1 - etaSurface[i]);
		NSurface[i][2] = 0.25*(1 + ksiSurface[i])*(1 + etaSurface[i]);
		NSurface[i][3] = 0.25*(1 - ksiSurface[i])*(1 + etaSurface[i]);

		//Calculade dN1/dEta, dN2/dEta, dN3/dEta, dN4/dEta in Volume
		dNdEta[0][i] = (-0.25)*(1 - eta[i]);
		dNdEta[1][i] = (0.25)*(1 - eta[i]);
		dNdEta[2][i] = (0.25)*(1 + eta[i]);
		dNdEta[3][i] = (-0.25)*(1 + eta[i]);

		//Calculate dN1/dKsi, dN2/dKsi, dN3/dKsi, dN4/dKsi in Volume
		dNdKsi[0][i] = (-0.25)*(1 - ksi[i]);
		dNdKsi[1][i] = (-0.25)*(1 + ksi[i]);
		dNdKsi[2][i] = (0.25)*(1 + ksi[i]);
		dNdKsi[3][i] = 0.25*(1 - ksi[i]);
	}
}


UniversalElement::~UniversalElement()
{
}


std::array<std::array<long double, 4>, 4> & UniversalElement::getN()
{
	return N;
}


std::array<std::array<long double, 4>, 4>  & UniversalElement::getNSurface()
{
	return NSurface;
}


std::array<std::array<long double, 4>, 4> & UniversalElement::getdNdKsi()
{
	return dNdEta;
}


std::array<std::array<long double, 4>, 4> & UniversalElement::getdNdEta()
{
	return dNdKsi;
}