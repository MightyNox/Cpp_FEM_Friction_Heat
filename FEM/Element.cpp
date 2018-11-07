#include "Element.h"
#include "Input.h"
#include <iostream>

Element::Element(unsigned long int id, std::vector<GlobalNode *> &nodes, ShapeFunction shapeFunction)
{
	//Set element attributes
	{
		this->id = id;
		this->conductionRatio = Input::getConductionRatio();
		this->globalNodes = nodes;
	}

	//Calculating Global Nodes Into Local Nodes
	{
		std::array<std::array<long double, 4>, 4> N = shapeFunction.getN();
		long double xToEta;
		long double yToKsi;
		for (int i = 0; i < 4; i++)
		{
			xToEta = 0;
			yToKsi = 0;

			for (int j = 0; j < 4; j++)
			{
				xToEta += N[i][j] * nodes[j]->getX();
				yToKsi += N[i][j] * nodes[j]->getY();
			}

			localNodes.push_back(new LocalNode(xToEta, yToKsi));
		}
	}

	//Calculating matrix H
	{
		std::array<std::array<long double, 4>, 4> NdEta = shapeFunction.getNdEta();
		std::array<std::array<long double, 4>, 4> NdKsi = shapeFunction.getNdKsi();

		//Jacobian of Transformation
		long double jakobian[4][4];
		for (int i = 0; i < 4; i++)
		{
			jakobian[0][i] = 0;
			jakobian[1][i] = 0;
			jakobian[2][i] = 0;
			jakobian[3][i] = 0;

			for (int j = 0; j < 4; j++)
			{
				jakobian[0][i] += nodes[j]->getX()*NdEta[j][i];
				jakobian[1][i] += nodes[j]->getY()*NdEta[j][i];
				jakobian[2][i] += nodes[j]->getX()*NdKsi[j][i];
				jakobian[3][i] += nodes[j]->getY()*NdKsi[j][i];
			}

			/*std::cout << "jakobian[0]["<<i<<"] "<<jakobian[0][i]<< std::endl;
			std::cout << "jakobian[1][" << i << "] " << jakobian[1][i] << std::endl;
			std::cout << "jakobian[2][" << i << "] " << jakobian[2][i] << std::endl;
			std::cout << "jakobian[3][" << i << "] " << jakobian[3][i] << std::endl;*/
		}

		//det J
		long double detJ[4];
		for (int i = 0; i < 4; i++)
		{
			detJ[i] = jakobian[0][i] * jakobian[3][i] - jakobian[1][i] * jakobian[2][i];
			//std::cout <<"detJ["<<i<<"] "<<detJ[i] << std::endl;
		}

		// odwrocony jakobian
		long double macierz2[4][4];
		for (int i = 0; i < 4; i++) //idzie w poziomie
		{
			macierz2[0][i] = jakobian[3][i] / detJ[0];
			macierz2[1][i] = (-1)*jakobian[1][i] / detJ[1];
			macierz2[2][i] = jakobian[2][i] / detJ[2];
			macierz2[3][i] = jakobian[0][i] / detJ[3];

			/*std::cout << "macierz2 [0][" << i << "] "<< macierz2[0][i] << std::endl;
			std::cout << "macierz2 [1][" << i << "] " << macierz2[1][i] << std::endl;
			std::cout << "macierz2 [2][" << i << "] " << macierz2[2][i] << std::endl;
			std::cout << "macierz2 [3][" << i << "] " << macierz2[3][i] << std::endl;*/
		}

		// pochodne funkcji kszta³tu po zmienych globalnych
		long double pochodnePoX[4][4];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				pochodnePoX[i][j] = macierz2[0][i] * NdEta[j][i] + macierz2[1][i] * NdKsi[j][i];
				//std::cout << "pochodne po X[" << i << "]["<<j<<"] " << pochodnePoX[i][j] << std::endl;
			}
		}

		long double pochodnePoY[4][4];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				pochodnePoY[i][j] = macierz2[2][i] * NdEta[j][i] + macierz2[3][i] * NdKsi[j][i];
				//std::cout << "pochodne po Y[" << i << "][" << j << "] " << pochodnePoY[i][j] << std::endl;
			}
		}

		//Mno¿enie wektorów {dN/dx} {dn/dY}
		long double mnozenieWektorowPoX[4][4][4];
		long double mnozenieWektorowPoY[4][4][4];
		for (int i = 0; i < 4; i++) //ktory punkt
		{
			for (int j = 0; j < 4; j++) //pion
			{
				for (int k = 0; k < 4; k++) //poziom
				{
					mnozenieWektorowPoX[i][j][k] = pochodnePoX[i][j] * pochodnePoX[i][k];
					mnozenieWektorowPoY[i][j][k] = pochodnePoY[i][j] * pochodnePoY[i][k];
					//std::cout << "Punkt "<<i<<" mnozenie wektorow po X[" << j << "][" << k << "] " << mnozenieWektorowPoX[i][j][k] << std::endl;
					//std::cout << "Punkt "<<i<<" mnozenie wektorow po Y[" << j << "][" << k << "] " << mnozenieWektorowPoY[i][j][k] << std::endl;
				}
			}
		}

		//Pomno¿one wektory mno¿ymy przez wyznacznik
		for (int i = 0; i < 4; i++) //ktory punkt
		{
			for (int j = 0; j < 4; j++) //pion
			{
				for (int k = 0; k < 4; k++) //poziom
				{
					mnozenieWektorowPoX[i][j][k] *= detJ[i];
					mnozenieWektorowPoY[i][j][k] *= detJ[i];

					//std::cout << "Punkt "<<i<<" Wyznacznik[" << j << "][" << k << "] " << mnozenieWektorowPoX[i][j][k] << std::endl;
					//std::cout << "Punkt "<<i<<" Wyznacznik[" << j << "][" << k << "] " << mnozenieWektorowPoY[i][j][k] << std::endl;
				}
			}
		}

		//suma wektórów * conductivity
		long double tmp[4][4][4];
		for (int i = 0; i < 4; i++) //ktory punkt
		{
			for (int j = 0; j < 4; j++) //pion
			{
				for (int k = 0; k < 4; k++) //poziom
				{
					tmp[i][j][k] = (mnozenieWektorowPoX[i][j][k] + mnozenieWektorowPoY[i][j][k])*conductionRatio;
					//std::cout << "punkt " << i << " indeks [" << j << "][" << k << "] " << tmp[i][j][k] << std::endl;
				}
			}
		}

		//H
		long double H[4][4];
		for (int i = 0; i < 4; i++) //ktory punkt
		{
			for (int j = 0; j < 4; j++) //pion
			{
				H[i][j] = tmp[0][i][j] + tmp[1][i][j] + tmp[2][i][j] + tmp[3][i][j];

				//std::cout << "H [" << i << "][" << j << "] " << H[i][j] << std::endl;
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