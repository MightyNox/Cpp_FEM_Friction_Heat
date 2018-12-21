#include "Display.h"
#include <iostream>
#include <iomanip>


Display::Display()
{
}


Display::~Display()
{
}


void Display::iteration(unsigned long int x)
{
	std::cout << "ITERATION: " << x << std::endl << std::endl;
}


void Display::temperature(Input &input, std::vector<Node*> nodes)
{
	std::cout << "Temperature" << std::endl;
	for (unsigned long int i = 0; i < input.getHorizontalNodeNumber(); i++)
	{
		for (unsigned long int j = 0; j < input.getVerticalNodeNumber(); j++)
			std::cout << nodes[j + (i*input.getVerticalNodeNumber())]->getT() << "	";

		std::cout << std::endl << std::endl;
	}
}


void Display::H(unsigned long int nodeCount, long double **globalH)
{
	std::cout << "[H]" << std::endl;
	for (unsigned long int i = 0; i < nodeCount; i++)
	{
		for (unsigned long int j = 0; j < nodeCount; j++)
		{
			std::cout << globalH[i][j]<<"  ";
		}

		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;
}


void Display::P(unsigned long int nodeCount, long double *globalP)
{
	std::cout << "{P}" << std::endl;
	for (unsigned long int j = 0; j < nodeCount; j++)
	{
		std::cout << globalP[j] << "  ";
	}
	std::cout << std::endl << std::endl;
}


void Display::grid(std::vector<Element*> elements, std::vector<Node*> nodes)
{
	for (unsigned long int i = 0; i < elements.size(); i++)
	{
		std::cout << elements[i]->getId();
		std::array<Node *, 4> nodes = elements[i]->getGlobalNodes();
		std::cout << " --> ";
		std::cout << nodes[0]->getId() << ", ";
		std::cout << nodes[1]->getId() << ", ";
		std::cout << nodes[2]->getId() << ", ";
		std::cout << nodes[3]->getId() << std::endl;
	}
}


void Display::universalElement(UniversalElement universalElement)
{
	std::array<std::array<long double, 4>, 4> tmpArray;
	
	//N
	{
		tmpArray = universalElement.getN();

		std::cout << std::setw(12) << " Point "
			<< std::setw(12) << " N1 "
			<< std::setw(12) << " N2 "
			<< std::setw(12) << " N3 "
			<< std::setw(12) << " N4 "
			<< std::endl;

		for (int i = 0; i < 4; i++)
		{
			std::cout << std::setw(12) << i;

			for (int j = 0; j < 4; j++)
			{
				std::cout << std::setw(12) << tmpArray[i][j];
			}

			std::cout << std::endl;
		}

		std::cout << std::endl << std::endl;
	}

	//NdEta
	{
		tmpArray = universalElement.getdNdEta();

		std::cout << std::setw(12) << " Point "
			<< std::setw(12) << " N1dEta "
			<< std::setw(12) << " N2dEta "
			<< std::setw(12) << " N3dEta "
			<< std::setw(12) << " N4dEta "
			<< std::endl;

		for (int i = 0; i < 4; i++)
		{
			std::cout << std::setw(12) << i;

			for (int j = 0; j < 4; j++)
			{
				std::cout << std::setw(12) << tmpArray[i][j];
			}

			std::cout << std::endl;
		}

		std::cout << std::endl << std::endl;
	}

	//NdKsi
	{
		tmpArray = universalElement.getdNdKsi();

		std::cout << std::setw(12) << " Point "
			<< std::setw(12) << " N1dKsi "
			<< std::setw(12) << " N2dKsi "
			<< std::setw(12) << " N3dKsi "
			<< std::setw(12) << " N4dKsi "
			<< std::endl;

		for (int i = 0; i < 4; i++)
		{
			std::cout << std::setw(12) << i;

			for (int j = 0; j < 4; j++)
			{
				std::cout << std::setw(12) << tmpArray[i][j];
			}

			std::cout << std::endl;
		}
	}
}


void Display::minmax(std::vector<long double> min, std::vector<long double> max)
{
	for (unsigned long int i=0; i < min.size(); i++)
	{
		std::cout << "Iteration: " << i + 1;
		std::cout << "	MIN: " << min[i];
		std::cout << "	MAX: " << max[i];
		std::cout << std::endl;
	}
}