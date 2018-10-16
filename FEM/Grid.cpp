#include "Grid.h"
#include "Input.h"
#include <iostream>


Grid::Grid()
{
	unsigned long int verticalNodeNumber = Input::getVerticalNodeNumber();
	unsigned long int horizontalNodeNumber = Input::getHorizontalNodeNumber();
	long double nodeHight = Input::getGridHight()/ (verticalNodeNumber -1);
	long double nodeWidth = Input::getGridWidth() / (Input::getHorizontalNodeNumber()-1);

	for (unsigned long int i = 0; i < horizontalNodeNumber; i++)
	{
		for (unsigned long int j = 0; j < verticalNodeNumber; j++)
		{
			gridNodes.push_back(new Node(i*nodeWidth, j*nodeHight));
		}
	}

	unsigned int id = 0;
	for (unsigned long int i = 0; i < horizontalNodeNumber - 1; i++)
	{
		for (unsigned long int j = 1; j < verticalNodeNumber; j++)
		{
			int * nodesId = new int[4];
			nodesId[0] = j +  verticalNodeNumber * i;
			nodesId[1] = j +  verticalNodeNumber  * (i+1);
			nodesId[2] = j + 1 + verticalNodeNumber * (i+1);
			nodesId[3] = j + 1 + verticalNodeNumber * i;

			gridElements.push_back(new Element(++id, nodesId));
		}
	}
}


Grid::~Grid()
{
	while (!gridElements.empty())
	{
		delete gridElements.back();
		gridElements.pop_back();
	}

	while (!gridNodes.empty())
	{
		delete gridNodes.back();
		gridNodes.pop_back();
	}
}


void Grid::displayGrid()
{
	for (unsigned long int i = 0; i < sizeof(gridElements)-1; i++)
	{
		std::cout<<gridElements[i]->getId();
		int * nodesId = gridElements[i]->getNodesId();
		std::cout<<" --> ";
		std::cout << nodesId[0] << ", ";
		std::cout << nodesId[1] << ", ";
		std::cout << nodesId[2] << ", ";
		std::cout << nodesId[3] <<std::endl;
	}
}