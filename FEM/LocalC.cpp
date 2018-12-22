#include "LocalC.h"



LocalC::LocalC(std::array <Node *, 4> globalNodes, UniversalElement *universalElement, Input *input)
{
	this->globalNodes = globalNodes;
	this->universalElement = universalElement;
	this->input = input;
}


LocalC::~LocalC()
{
}


void LocalC::calculate()
{
	std::array<std::array<long double, 4>, 4> N = universalElement->getN();
	std::array<std::array<long double, 4>, 4> dNdKsi = universalElement->getdNdKsi();
	std::array<std::array<long double, 4>, 4> dNdEta = universalElement->getdNdEta();
	long double density = input->getDensity();
	long double specificHeat = input->getSpecificHeat();

	for (int i = 0; i < 4; i++)
	{
		matrix[i].fill(0);
	}

	//i - the integration point
	for (int i = 0; i < 4; i++)
	{
		//Calculate Transformation Matrix for single node
		std::array<long double, 4> transformationMatrix = { 0, 0, 0, 0 };
		for (int j = 0; j < 4; j++)
		{
			transformationMatrix[0] += globalNodes[j]->getX()*dNdKsi[j][i];
			transformationMatrix[1] += globalNodes[j]->getY()*dNdKsi[j][i];
			transformationMatrix[2] += globalNodes[j]->getX()*dNdEta[j][i];
			transformationMatrix[3] += globalNodes[j]->getY()*dNdEta[j][i];
		}

		//Calculate detJ - transformation jacobian - determinant of Transformation Matrix
		std::array<long double, 4> detJ;
		detJ[i] = transformationMatrix[0] * transformationMatrix[3] - transformationMatrix[1] * transformationMatrix[2];

		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				matrix[j][k] += N[i][j] * N[i][k] * density * specificHeat *detJ[i];
			}
		}
	}
}


std::array< std::array <long double, 4>, 4> LocalC::getMatrix()
{
	return matrix;
}
