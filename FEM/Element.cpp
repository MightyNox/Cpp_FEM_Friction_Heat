#include "Element.h"
#include "Input.h"

Element::Element(unsigned long int id, std::array<Node *, 4> &globalNodes, UniversalElement universalElement, Input *input)
{
	//Set element attributes
	{
		this->id = id;
		this->globalNodes = globalNodes;
	}

	long double specificHeat = input->getSpecificHeat();
	long double density = input->getDensity();
	long double conductionRatio = input->getConductionRatio();
	long double convectionRatio = input->getConvectionRatio();
	long double ambientTemperature = input->getAmbientTemperature();
	long double densityStream = input->getDensityStream();

	std::array<std::array<long double, 4>, 4> N = universalElement.getN();
	std::array<std::array<long double, 4>, 4> NSurface = universalElement.getNSurface();
	std::array<long double, 4> detJ;

	//Calculate matrix H
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				localH[i][j] = 0;
			}
		}

		//Calculate first part of Heat Matrix
		{
			std::array<std::array<long double, 4>, 4> dNdKsi = universalElement.getdNdKsi();
			std::array<std::array<long double, 4>, 4> dNdEta = universalElement.getdNdEta();

			//Calculate first part of Matrix H for every Node
			for (int i = 0; i < 4; i++)
			{
				//Calculate Transformation Matrix for single node
				std::array<long double, 4> transformationJacobian = { 0, 0, 0, 0 };
				for (int j = 0; j < 4; j++)
				{
					transformationJacobian[0] += globalNodes[j]->getX()*dNdKsi[j][i];
					transformationJacobian[1] += globalNodes[j]->getY()*dNdKsi[j][i];
					transformationJacobian[2] += globalNodes[j]->getX()*dNdEta[j][i];
					transformationJacobian[3] += globalNodes[j]->getY()*dNdEta[j][i];
				}

				//Calculate detJ - transformation jacobian - determinant of Transformation Matrix
				detJ[i] = transformationJacobian[0] * transformationJacobian[3] - transformationJacobian[1] * transformationJacobian[2];

				//Reversed Matrix of Jacobian of Transformation - inverted matrix / detJ
				std::array<long double, 4> reversedTransformationJacobian;
				reversedTransformationJacobian[0] = transformationJacobian[3] / detJ[i];
				reversedTransformationJacobian[1] = (-1)*transformationJacobian[1] / detJ[i];
				reversedTransformationJacobian[2] = transformationJacobian[2] / detJ[i];
				reversedTransformationJacobian[3] = transformationJacobian[0] / detJ[i];

				//Calculate dN/dx
				std::array<long double, 4> dNdX;
				for (int j = 0; j < 4; j++)
				{
					dNdX[j] = reversedTransformationJacobian[0] * dNdKsi[j][i] + reversedTransformationJacobian[1] * dNdEta[j][i];
				}

				//Calculate dN/dy
				std::array<long double, 4> dNdY;
				for (int j = 0; j < 4; j++)
				{
					dNdY[j] = reversedTransformationJacobian[2] * dNdKsi[j][i] + reversedTransformationJacobian[3] * dNdEta[j][i];
				}

				//Multiplication and Sum of vectors * determinant of Jacobian of Transformation Matrix * conductivity - dN/dx * dN/dx + dN/dy * dN/dy * detJ * k
				for (int j = 0; j < 4; j++)
				{
					for (int k = 0; k < 4; k++)
					{
						localH[j][k] += (dNdX[k] * dNdX[j] + dNdY[k] * dNdY[j])*detJ[i] * conductionRatio;
					}
				}
			}
		}

		//Calculate second part of Heat Matrix
		{
			std::array < std::array<long double, 4>, 4> matrixN;
			std::array < std::array<long double, 4>, 4>  matrixNT;
			int j;

			//Calculate second part of Matrix H for every surface
			for (int i = 0; i < 4; i++)
			{
				//Current and the next index 4 goes into 0
				j = i + 1;
				if (j == 4)
				{
					j = 0;
				}

				//Check if two points are on the same bound
				bool onBound1 = globalNodes[i]->getOnBoundConvection();
				bool onBound2 = globalNodes[j]->getOnBoundConvection();
				if (onBound1 != true || onBound2 != true)
				{
					continue;
				}

				//Calculate jacobian of transformation - length of the bound - sqrt(P1^2 + P2^2)
				long double localDetJ = 0;
				localDetJ += (globalNodes[i]->getX() - globalNodes[j]->getX()) * (globalNodes[i]->getX() - globalNodes[j]->getX());
				localDetJ += (globalNodes[i]->getY() - globalNodes[j]->getY()) * (globalNodes[i]->getY() - globalNodes[j]->getY());
				localDetJ = sqrt(localDetJ) * 0.5;

				//Fill the arrays
				for (int i = 0; i < 4; i++)
				{
					matrixN[i].fill(0);
					matrixNT[i].fill(0);
				}

				//Calculate matrix N
				matrixN[i][i] = NSurface[i][i] * NSurface[i][i]; //Nx * Nx
				matrixN[i][j] = NSurface[i][i] * NSurface[i][j]; //Nx * Ny
				matrixN[j][i] = matrixN[i][j]; //Ny * Nx - it's the same as Nx * Ny
				matrixN[j][j] = NSurface[i][j] * NSurface[i][j]; //Ny * Ny

				//Transpose matrix N
				matrixNT[i][i] = matrixN[j][j];
				matrixNT[i][j] = matrixN[j][i];
				matrixNT[j][i] = matrixN[i][j];
				matrixNT[j][j] = matrixN[i][i];

				for (int k = 0; k < 4; k++)
				{
					for (int l = 0; l < 4; l++)
					{
						//Calculate {N}{NT}*convection*detJ and Add values to the localHeat matrix
						localH[k][l] += (matrixN[k][l] + matrixNT[k][l]) * convectionRatio * localDetJ;
					}
				}
			}
		}
	}
	
	//Calculate matrix C
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				localC[i][j] = 0;
			}
		}

		//i - the integration point
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					localC[j][k] += N[i][j] * N[i][k] * density * specificHeat *detJ[i];
				}
			}
		}
	}

	//Calculate vector P
	{
		localP.fill(0);

		int j;
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
					localP[i] += NSurface[i][l] * ambientTemperature*convectionRatio*localDetJ;
					localP[j] += NSurface[i][l] * ambientTemperature*convectionRatio*localDetJ;
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
					localP[i] += NSurface[i][l] * densityStream*localDetJ;
					localP[j] += NSurface[i][l] * densityStream*localDetJ;
				}
			}
		}
	}
}


Element::~Element()
{
}


unsigned long int Element::getId()
{
	return id;
}


std::array<Node *, 4> & Element::getGlobalNodes()
{
	return globalNodes;
}


std::array<std::array<long double, 4>, 4> & Element::getLocalH()
{
	return localH;
}


std::array<std::array<long double, 4>, 4> & Element::getLocalC()
{
	return localC;
}


std::array<long double, 4> & Element::getLocalP()
{
	return localP;
}