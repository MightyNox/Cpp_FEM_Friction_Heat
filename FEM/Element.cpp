#include "Element.h"
#include "Input.h"
#include <iostream>

Element::Element(unsigned long int id, std::vector<GlobalNode *> &globalNodes, UniversalElement universalElement)
{
	//Set element attributes
	{
		this->id = id;
		this->conductionRatio = Input::getConductionRatio();
		this->globalNodes = globalNodes;
	}

	//Convert Global Nodes Into Local Nodes
	{
		std::array<std::array<long double, 4>, 4> N = universalElement.getN();
		long double xToEta;
		long double yToKsi;
		for (int i = 0; i < 4; i++)
		{
			xToEta = 0;
			yToKsi = 0;

			for (int j = 0; j < 4; j++)
			{
				xToEta += N[i][j] * globalNodes[j]->getX();
				yToKsi += N[i][j] * globalNodes[j]->getY();
			}

			localNodes.push_back(new LocalNode(xToEta, yToKsi));
		}
	}

	//Calculating Heat Matrix
	{
		std::array<std::array<long double, 4>, 4> localHeat;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				localHeat[i][j] = 0;
			}
		}

		std::array<std::array<long double, 4>, 4> NdEta = universalElement.getNdEta();
		std::array<std::array<long double, 4>, 4> NdKsi = universalElement.getNdKsi();

		for (int localNodeId = 0; localNodeId < 4; localNodeId++)
		{
			//Calculate Jacobian of Transformation Matrix for single node
			std::array<long double, 4> transformationJacobian = { 0, 0, 0, 0 };
			for (int j = 0; j < 4; j++)
			{
				transformationJacobian[0] += globalNodes[j]->getX()*NdEta[j][localNodeId];
				transformationJacobian[1] += globalNodes[j]->getY()*NdEta[j][localNodeId];
				transformationJacobian[2] += globalNodes[j]->getX()*NdKsi[j][localNodeId];
				transformationJacobian[3] += globalNodes[j]->getY()*NdKsi[j][localNodeId];
			}

			//Calculate detJ - determinant of Jacobian of Transformation Matrix
			long double detJ = transformationJacobian[0] * transformationJacobian[3] - transformationJacobian[1] * transformationJacobian[2];

			//Reversed Matrix of Jacobian of Transformation - inverted matrix / detJ
			std::array<long double, 4> reversedTransformationJacobian;
			reversedTransformationJacobian[0] = transformationJacobian[3] / detJ;
			reversedTransformationJacobian[1] = (-1)*transformationJacobian[1] / detJ;
			reversedTransformationJacobian[2] = transformationJacobian[2] / detJ;
			reversedTransformationJacobian[3] = transformationJacobian[0] / detJ;

			//Calculate derivative N / derivative x
			std::array<long double, 4> dNdX;
			for (int j = 0; j < 4; j++)
			{
				dNdX[j] = reversedTransformationJacobian[0] * NdEta[j][localNodeId] + reversedTransformationJacobian[1] * NdKsi[j][localNodeId];
			}

			//Calculate derivative N / derivative y
			std::array<long double, 4> dNdY;
			for (int j = 0; j < 4; j++)
			{
				dNdY[j] = reversedTransformationJacobian[2] * NdEta[j][localNodeId] + reversedTransformationJacobian[3] * NdKsi[j][localNodeId];
			}

			//Multiplication and Sum of vectors * determinant of Jacobian of Transformation Matrix * conductivity - dN/dx * dN/dx + dN/dy * dN/dy * detJ * k
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					localHeat[i][j] += (dNdX[j] * dNdX[i] + dNdY[j] * dNdY[i])*detJ * conductionRatio;
				}
			}
		}
	}
}


Element::~Element()
{
	while (!localNodes.empty())
	{
		delete localNodes.back();
		localNodes.pop_back();
	}
}


unsigned long int Element::getId()
{
	return id;
}


std::vector<GlobalNode *> & Element::getNodes()
{
	return globalNodes;
}


std::vector<LocalNode *> & Element::getLocalNodes()
{
	return localNodes;
}