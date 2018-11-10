#include "Element.h"
#include "Input.h"
#include <iostream>

Element::Element(unsigned long int id, std::vector<GlobalNode *> &globalNodes, UniversalElement universalElement, Input *input)
{
	//Set element attributes
	{
		this->id = id;
		this->conductionRatio = input->getConductionRatio();
		this->convectionRatio = input->getConvectionRatio();
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

	//Calculate Heat Matrix
	{
		std::array<std::array<long double, 4>, 4> localHeat;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				localHeat[i][j] = 0;
			}
		}

		//Calculate first part of Heat Matrix
		{
			std::array<std::array<long double, 4>, 4> NdEta = universalElement.getdNdEta();
			std::array<std::array<long double, 4>, 4> NdKsi = universalElement.getdNdKsi();

			//Calculate first part of Matrix H for every Node
			//i - localNodeId
			for (int i = 0; i < 4; i++)
			{
				//Calculate Jacobian of Transformation Matrix for single node
				std::array<long double, 4> transformationJacobian = { 0, 0, 0, 0 };
				for (int j = 0; j < 4; j++)
				{
					transformationJacobian[0] += globalNodes[j]->getX()*NdEta[j][i];
					transformationJacobian[1] += globalNodes[j]->getY()*NdEta[j][i];
					transformationJacobian[2] += globalNodes[j]->getX()*NdKsi[j][i];
					transformationJacobian[3] += globalNodes[j]->getY()*NdKsi[j][i];
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
					dNdX[j] = reversedTransformationJacobian[0] * NdEta[j][i] + reversedTransformationJacobian[1] * NdKsi[j][i];
				}

				//Calculate derivative N / derivative y
				std::array<long double, 4> dNdY;
				for (int j = 0; j < 4; j++)
				{
					dNdY[j] = reversedTransformationJacobian[2] * NdEta[j][i] + reversedTransformationJacobian[3] * NdKsi[j][i];
				}

				//Multiplication and Sum of vectors * determinant of Jacobian of Transformation Matrix * conductivity - dN/dx * dN/dx + dN/dy * dN/dy * detJ * k
				for (int j = 0; j < 4; j++)
				{
					for (int k = 0; k < 4; k++)
					{
						localHeat[j][k] += (dNdX[k] * dNdX[j] + dNdY[k] * dNdY[j])*detJ * conductionRatio;
					}
				}
			}
		}

		//Calculate second part of Heat Matrix
		{
			std::array<std::array<std::array<long double, 4>, 2>, 4> NSurface = universalElement.getNSurface();
			std::array < std::array<long double, 4>, 4> matrixN;
			std::array < std::array<long double, 4>, 4>  matrixNT;
			int onBoundCounter = 0;
			int j;
			//Calculate second part of Matrix H for every surface
			//i - surfaceId
			for (int i = 0; i < 1; i++)
			{
				//There can be only 2 Boundary Conditions
				if (onBoundCounter == 2)
					break;

				//Current and the next index 4 goes into 0
				j = i + 1;
				if (j == 4)
				{
					j = 0;
				}

				//Check if two points are on the same bound
				bool onBound1 = globalNodes[i]->getOnBound();
				bool onBound2 = globalNodes[j]->getOnBound();
				if (onBound1 != true || onBound2 != true)
				{
					continue;
				}
				else
				{
					onBoundCounter++;
				}

				//Length of the bound - sqrt(P1^2 + P2^2)
				long double sideLength = 0;
				sideLength += (globalNodes[i]->getX() - globalNodes[j]->getX()) * (globalNodes[i]->getX() - globalNodes[j]->getX());
				sideLength += (globalNodes[i]->getY() - globalNodes[j]->getY()) * (globalNodes[i]->getY() - globalNodes[j]->getY());
				sideLength = sqrt(sideLength);

				//Calculate detJ - determinant of Jacobian
				long double detJ = sideLength / 2.0;

				//Fill the arrays
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						matrixN[i][j] = 0;
						matrixNT[i][j] = 0;
					}
				}

				//Calculate matrix N
				matrixN[i][i] = NSurface[i][0][i] * NSurface[i][0][i] * convectionRatio; //Nx * Nx
				matrixN[i][j] = NSurface[i][0][i] * NSurface[i][0][j] * convectionRatio; //Nx * Ny
				matrixN[j][i] = matrixN[i][j]; //Ny * Nx - it's the same as Nx * Ny
				matrixN[j][j] = NSurface[i][0][j] * NSurface[i][0][j] * convectionRatio; //Ny * Ny

				//Transpose matrix N
				matrixNT[i][i] = matrixN[j][j];
				matrixNT[i][j] = matrixN[j][i];
				matrixNT[j][i] = matrixN[i][j];
				matrixNT[j][j] = matrixN[i][i];

				for (int k = 0; k < 4; k++)
				{
					for (int j = 0; j < 4; j++)
					{
						//Calculate {N}{NT}*convection*detJ
						matrixN[k][j] = (matrixN[k][j] + matrixNT[k][j]) * detJ;

						//Add values to the localHeat matrix
						localHeat[k][j] += matrixN[k][j];
					}
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


std::vector<GlobalNode *> & Element::getGlobalNodes()
{
	return globalNodes;
}


std::vector<LocalNode *> & Element::getLocalNodes()
{
	return localNodes;
}