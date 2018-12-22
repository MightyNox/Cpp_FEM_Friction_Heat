#include "LocalP.h"



LocalP::LocalP(std::array <Node *, 4> globalNodes, UniversalElement *universalElement, Input *input)
{
	this->globalNodes = globalNodes;
	this->universalElement = universalElement;
	this->input = input;
}


void LocalP::calculate()
{
	std::array<std::array<long double, 4>, 4> NSurface = universalElement->getNSurface();
	long double ambientTemperature = input->getAmbientTemperature();
	long double naturalConvectionRatio = input->getNaturalConvectionRatio();
	long double forcedConvectionRatio = input->getForcedConvectionRatio();
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

		//Check if there is natural convection
		if (globalNodes[i]->getOnBoundNaturalConvection() && globalNodes[j]->getOnBoundNaturalConvection() )
		{

			//Calculate jacobian of transformation - length of the bound - sqrt(P1^2 + P2^2)
			long double localDetJ = 0;
			localDetJ += (globalNodes[i]->getX() - globalNodes[j]->getX()) * (globalNodes[i]->getX() - globalNodes[j]->getX());
			localDetJ += (globalNodes[i]->getY() - globalNodes[j]->getY()) * (globalNodes[i]->getY() - globalNodes[j]->getY());
			localDetJ = sqrt(localDetJ) * 0.5;


			for (int l = 0; l < 4; l++)
			{
				vector[i] += NSurface[i][l] * ambientTemperature*naturalConvectionRatio*localDetJ;
				vector[j] += NSurface[i][l] * ambientTemperature*naturalConvectionRatio*localDetJ;
			}
		}

		//Check if there is forced convection
		if (globalNodes[i]->getOnBoundForcedConvection() && globalNodes[j]->getOnBoundForcedConvection())
		{

			//Calculate jacobian of transformation - length of the bound - sqrt(P1^2 + P2^2)
			long double localDetJ = 0;
			localDetJ += (globalNodes[i]->getX() - globalNodes[j]->getX()) * (globalNodes[i]->getX() - globalNodes[j]->getX());
			localDetJ += (globalNodes[i]->getY() - globalNodes[j]->getY()) * (globalNodes[i]->getY() - globalNodes[j]->getY());
			localDetJ = sqrt(localDetJ) * 0.5;


			for (int l = 0; l < 4; l++)
			{
				vector[i] += NSurface[i][l] * ambientTemperature*forcedConvectionRatio*localDetJ;
				vector[j] += NSurface[i][l] * ambientTemperature*forcedConvectionRatio*localDetJ;
			}
		}

		//Check if there is stream
		if (globalNodes[i]->getOnBoundStream()  && globalNodes[j]->getOnBoundStream() )
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