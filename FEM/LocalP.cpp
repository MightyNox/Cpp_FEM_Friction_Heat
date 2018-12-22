#include "LocalP.h"



LocalP::LocalP(std::array <Node *, 4> globalNodes, UniversalElement &universalElement, Input *input)
{
	std::array<std::array<long double, 4>, 4> NSurface = universalElement.getNSurface();
	long double ambientTemperature = input->getAmbientTemperature();
	long double convectionRatio = input->getConvectionRatio();
	long double densityStream = input->getDensityStream();
	int j;

	vector.fill(0);

	//i - surfaceId
	for (int i = 0; i < 4; i++)
	{
		//Current and the next index 4 goes into 0
		j = i + 1;
		if (j == 4)
		{
			j = 0;
		}

		//Check if there is convection or stream
		if (globalNodes[i]->getOnBoundConvection() == true && globalNodes[j]->getOnBoundConvection() == true)
		{

			//Calculate jacobian of transformation - length of the bound - sqrt(P1^2 + P2^2)
			long double localDetJ = 0;
			localDetJ += (globalNodes[i]->getX() - globalNodes[j]->getX()) * (globalNodes[i]->getX() - globalNodes[j]->getX());
			localDetJ += (globalNodes[i]->getY() - globalNodes[j]->getY()) * (globalNodes[i]->getY() - globalNodes[j]->getY());
			localDetJ = sqrt(localDetJ) * 0.5;


			for (int l = 0; l < 4; l++)
			{
				vector[i] += NSurface[i][l] * ambientTemperature*convectionRatio*localDetJ;
				vector[j] += NSurface[i][l] * ambientTemperature*convectionRatio*localDetJ;
			}
		}

		if (globalNodes[i]->getOnBoundStream() == true && globalNodes[j]->getOnBoundStream() == true)
		{
			//Calculate jacobian of transformation - length of the bound - sqrt(P1^2 + P2^2)
			long double localDetJ = 0;
			localDetJ += (globalNodes[i]->getX() - globalNodes[j]->getX()) * (globalNodes[i]->getX() - globalNodes[j]->getX());
			localDetJ += (globalNodes[i]->getY() - globalNodes[j]->getY()) * (globalNodes[i]->getY() - globalNodes[j]->getY());
			localDetJ = sqrt(localDetJ) * 0.5;

			for (int l = 0; l < 4; l++)
			{
				vector[i] += NSurface[i][l] * densityStream*localDetJ;
				vector[j] += NSurface[i][l] * densityStream*localDetJ;
			}
		}
	}
}


LocalP::~LocalP()
{
}


std::array <long double, 4> LocalP::getVector()
{
	return vector;
}