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


void Display::temperature(Input &input, std::vector<GlobalNode*> nodes)
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


void Display::grid(std::vector<Element*> elements, std::vector<GlobalNode*> nodes)
{
	for (unsigned long int i = 0; i < elements.size(); i++)
	{
		std::cout << elements[i]->getId();
		std::array<GlobalNode *, 4> nodes = elements[i]->getGlobalNodes();
		std::cout << " --> ";
		std::cout << nodes[0]->getId() << ", ";
		std::cout << nodes[1]->getId() << ", ";
		std::cout << nodes[2]->getId() << ", ";
		std::cout << nodes[3]->getId() << std::endl;
	}
}


void Display::element(std::vector<Element*> elements, unsigned long int id)
{
	if (id == 0 || id > elements.size())
	{
		std::cout << "WRONG ID!" << std::endl;
		return;
	}

	std::cout << "Element ID: " << id << std::endl << std::endl;

	Element * element = elements[id - 1];

	std::array<GlobalNode *, 4> globalNodes = element->getGlobalNodes();
	std::cout << "Global Node " << globalNodes[0]->getId() << " --> " << "x: " << globalNodes[0]->getX() << ", y: " << globalNodes[0]->getY() << std::endl;
	std::cout << "Global Node " << globalNodes[1]->getId() << " --> " << "x: " << globalNodes[1]->getX() << ", y: " << globalNodes[1]->getY() << std::endl;
	std::cout << "Global Node " << globalNodes[2]->getId() << " --> " << "x: " << globalNodes[2]->getX() << ", y: " << globalNodes[2]->getY() << std::endl;
	std::cout << "Global Node " << globalNodes[3]->getId() << " --> " << "x: " << globalNodes[3]->getX() << ", y: " << globalNodes[3]->getY() << std::endl;

	std::cout << std::endl;

	std::array<LocalNode *, 4> localNodes = element->getLocalNodes();
	std::cout << "Local Node --> " << "eta: " << localNodes[0]->getEta() << ", ksi: " << localNodes[0]->getKsi() << std::endl;
	std::cout << "Local Node --> " << "eta: " << localNodes[1]->getEta() << ", ksi: " << localNodes[1]->getKsi() << std::endl;
	std::cout << "Local Node --> " << "eta: " << localNodes[2]->getEta() << ", ksi: " << localNodes[2]->getKsi() << std::endl;
	std::cout << "Local Node --> " << "eta: " << localNodes[3]->getEta() << ", ksi: " << localNodes[3]->getKsi() << std::endl;
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