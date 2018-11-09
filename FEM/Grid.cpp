#include "Grid.h"
#include "Input.h"
#include <iostream>
#include <iomanip>


Grid::Grid()
{
	unsigned long int verticalNodeNumber = Input::getVerticalNodeNumber();
	unsigned long int horizontalNodeNumber = Input::getHorizontalNodeNumber();
	long double nodeHight = Input::getGridHight()/ (verticalNodeNumber -1);
	long double nodeWidth = Input::getGridWidth() / (Input::getHorizontalNodeNumber()-1);

	//Creating All Nodes
	unsigned int id = 0;
	for (unsigned long int i = 0; i < horizontalNodeNumber; i++)
	{
		for (unsigned long int j = 0; j < verticalNodeNumber; j++)
		{
			nodes.push_back(new GlobalNode(++id, i*nodeWidth, j*nodeHight));
		}
	}


	//Creating All Elements and Set Their Nodes
	id = 0;

	for (unsigned long int i = 0; i < horizontalNodeNumber - 1; i++)
	{
		for (unsigned long int j = 1; j < verticalNodeNumber; j++)
		{
			if (j == 2)
				goto here;

			int nodesId[4];
			nodesId[0] = j +  verticalNodeNumber * i;
			nodesId[1] = j +  verticalNodeNumber  * (i+1);
			nodesId[2] = j + 1 + verticalNodeNumber * (i+1);
			nodesId[3] = j + 1 + verticalNodeNumber * i;

			std::vector<GlobalNode *> elementNodes;
			for (int k = 0; k < 4; k++)
			{
				elementNodes.push_back(nodes[nodesId[k] - 1]);
			}

			elements.push_back(new Element(++id, elementNodes, universalElement));
		}
	}

here:
	{

	}
}


Grid::~Grid()
{
	while (!elements.empty())
	{
		delete elements.back();
		elements.pop_back();
	}

	while (!nodes.empty())
	{
		delete nodes.back();
		nodes.pop_back();
	}
}


void Grid::displayGrid()
{
	for (unsigned long int i = 0; i < elements.size(); i++)
	{
		std::cout<<elements[i]->getId();
		std::vector<GlobalNode *> nodes = elements[i]->getNodes();
		std::cout<<" --> ";
		std::cout << nodes[0]->getId() << ", ";
		std::cout << nodes[1]->getId() << ", ";
		std::cout << nodes[2]->getId() << ", ";
		std::cout << nodes[3]->getId() <<std::endl;
	}
}


void Grid::displayElement(unsigned long int id)
{
	if (id ==0 || id > elements.size())
	{
		std::cout << "WRONG ID!" << std::endl;
		return;
	}

	std::cout <<"Element ID: "<< id <<std::endl << std::endl;

	Element * element = elements[id-1];
	
	std::vector<GlobalNode *> globalNodes = element->getNodes();
	std::cout << "Global Node "<< globalNodes[0]->getId() <<" --> " << "x: " << globalNodes[0]->getX() << ", y: " << globalNodes[0]->getY() << std::endl;
	std::cout << "Global Node " << globalNodes[1]->getId() << " --> " << "x: " << globalNodes[1]->getX() << ", y: " << globalNodes[1]->getY() << std::endl;
	std::cout << "Global Node " << globalNodes[2]->getId() << " --> " << "x: " << globalNodes[2]->getX() << ", y: " << globalNodes[2]->getY() << std::endl;
	std::cout << "Global Node " << globalNodes[3]->getId() << " --> " << "x: " << globalNodes[3]->getX() << ", y: " << globalNodes[3]->getY() << std::endl;

	std::cout << std::endl;

	std::vector<LocalNode *> localNodes = element->getLocalNodes();
	std::cout << "Local Node --> " << "eta: " << localNodes[0]->getEta() << ", ksi: " << localNodes[0]->getKsi() << std::endl;
	std::cout << "Local Node --> " << "eta: " << localNodes[1]->getEta() << ", ksi: " << localNodes[1]->getKsi() << std::endl;
	std::cout << "Local Node --> " << "eta: " << localNodes[2]->getEta() << ", ksi: " << localNodes[2]->getKsi() << std::endl;
	std::cout << "Local Node --> " << "eta: " << localNodes[3]->getEta() << ", ksi: " << localNodes[3]->getKsi() << std::endl;
}


void Grid::displayUniversalElement()
{
	std::array<std::array<long double, 4>, 4> tmpArray = universalElement.getN();

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

	tmpArray = universalElement.getNdEta();

	std::cout << std::setw(12) << " Point "
		<< std::setw(12) << " NdEta1 "
		<< std::setw(12) << " NdEta2 "
		<< std::setw(12) << " NdEta3 "
		<< std::setw(12) << " NdEta4 "
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

	tmpArray = universalElement.getNdKsi();

	std::cout << std::setw(12) << " Point "
		<< std::setw(12) << " NdKsi1 "
		<< std::setw(12) << " NdKsi2 "
		<< std::setw(12) << " NdKsi3 "
		<< std::setw(12) << " NdKsi4 "
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