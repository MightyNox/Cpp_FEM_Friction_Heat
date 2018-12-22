#include "LocalH.h"



LocalH::LocalH(std::array <Node *, 4> globalNodes, UniversalElement *universalElement, Input *input)
{
	this->globalNodes = globalNodes;
	this->universalElement = universalElement;
	this->input = input;
}


LocalH::~LocalH()
{
}


void LocalH::calculate()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix[i].fill(0);
		}
	}

	//Calculate first part of Heat Matrix
	{
		std::array<std::array<long double, 4>, 4> dNdKsi = universalElement->getdNdKsi();
		std::array<std::array<long double, 4>, 4> dNdEta = universalElement->getdNdEta();
		long double conductionRatio = input->getConductionRatio();

		//Calculate first part of Matrix H for every Node
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

			//Reversed Matrix of Transformation divided by it's determinant (before reversing)- inverted matrix / detJ
			std::array<long double, 4> reversedTransformationMatrix;
			reversedTransformationMatrix[0] = transformationMatrix[3] / detJ[i];
			reversedTransformationMatrix[1] = (-1)*transformationMatrix[1] / detJ[i];
			reversedTransformationMatrix[2] = transformationMatrix[2] / detJ[i];
			reversedTransformationMatrix[3] = transformationMatrix[0] / detJ[i];

			//Calculate dN/dx
			std::array<long double, 4> dNdX;
			for (int j = 0; j < 4; j++)
			{
				dNdX[j] = reversedTransformationMatrix[0] * dNdKsi[j][i] + reversedTransformationMatrix[1] * dNdEta[j][i];
			}

			//Calculate dN/dy
			std::array<long double, 4> dNdY;
			for (int j = 0; j < 4; j++)
			{
				dNdY[j] = reversedTransformationMatrix[2] * dNdKsi[j][i] + reversedTransformationMatrix[3] * dNdEta[j][i];
			}

			//Multiplication and Sum of vectors * determinant of Jacobian of Transformation Matrix * conductivity - dN/dx * dN/dx + dN/dy * dN/dy * detJ * k
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					matrix[j][k] += (dNdX[k] * dNdX[j] + dNdY[k] * dNdY[j])*detJ[i] * conductionRatio;
				}
			}
		}
	}

	//Calculate second part of Heat Matrix
	{
		std::array<std::array<long double, 4>, 4> NSurface = universalElement->getNSurface();
		std::array < std::array<long double, 4>, 4> matrixN;
		std::array < std::array<long double, 4>, 4>  matrixNT;
		long double naturalConvectionRatio = input->getNaturalConvectionRatio();
		long double forcedConvectionRatio = input->getForcedConvectionRatio();
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

			//Check if there is natural convection
			if (globalNodes[i]->getOnBoundNaturalConvection() && globalNodes[j]->getOnBoundNaturalConvection())
			{

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
						matrix[k][l] += (matrixN[k][l] + matrixNT[k][l]) * naturalConvectionRatio * localDetJ;
					}
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
						matrix[k][l] += (matrixN[k][l] + matrixNT[k][l]) * forcedConvectionRatio * localDetJ;
					}
				}
			}
		}
	}
}


std::array< std::array <long double, 4>, 4> LocalH::getMatrix()
{
	return matrix;
}