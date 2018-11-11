#include "Grid.h"
#include "Input.h"
#include <iostream>
#include <iomanip>


Grid::Grid()
{
	//Load Input
	unsigned long int verticalNodeNumber = input.getVerticalNodeNumber();
	unsigned long int horizontalNodeNumber = input.getHorizontalNodeNumber();
	long double nodeHight = input.getGridHight()/ (verticalNodeNumber -1);
	long double nodeWidth = input.getGridWidth() / (horizontalNodeNumber -1);

	//Create Global Nodes
	{
		unsigned int id = 0;
		bool onBoundHorizontal;
		bool onBoundVertical;
		for (unsigned long int i = 0; i < horizontalNodeNumber; i++)
		{
			//If the global node is on the left or the right edge
			if (i == 0 || i == horizontalNodeNumber - 1)
			{
				onBoundHorizontal = true;
			}
			else
			{
				onBoundHorizontal = false;
			}

			for (unsigned long int j = 0; j < verticalNodeNumber; j++)
			{
				//If the global node is on the left or the right edge
				if (onBoundHorizontal == true)
				{
					onBoundVertical = true;
				}
				//If the global node is neither on the left nor right edge, check if it is on top or bottom edge
				else if (onBoundHorizontal == false && (j == 0 || j == verticalNodeNumber - 1))
				{
					onBoundVertical = true;
				}
				else
				{
					onBoundVertical = false;
				}

				nodes.push_back(new GlobalNode(++id, i*nodeWidth, j*nodeHight, onBoundVertical, &input));
			}
		}
	}

	//Create Elements and Set Their Global Nodes
	{
		unsigned int id = 0;
		for (unsigned long int i = 0; i < horizontalNodeNumber - 1; i++)
		{
			for (unsigned long int j = 1; j < verticalNodeNumber; j++)
			{
				int nodesId[4];
				nodesId[0] = j + verticalNodeNumber * i;
				nodesId[1] = j + verticalNodeNumber * (i + 1);
				nodesId[2] = j + 1 + verticalNodeNumber * (i + 1);
				nodesId[3] = j + 1 + verticalNodeNumber * i;

				std::vector<GlobalNode *> elementNodes;
				for (int k = 0; k < 4; k++)
				{
					elementNodes.push_back(nodes[nodesId[k] - 1]);
				}

				elements.push_back(new Element(++id, elementNodes, universalElement, &input));
			}
		}
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


long double * gauss(long double **A, long double *B, unsigned long int n)
{
	int iteration = 3;
	long double **M,*N, *x, *result;

	//create arrays
	{
		N = new long double[n];

		M = new long double*[n];
		for (int i = 0; i < n; i++)
			M[i] = new long double[n];

		x = new long double[n];
		for (int i = 0; i < n; i++)
			x[i] = 0;

		result = new long double[n];
	}

	//Calculate matrix N
	for (int i = 0; i < n; i++)
	{
		N[i] = 1 / A[i][i];
	}

	//Calculate matrix M
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i == j)
				M[i][j] = 0;
			else
				M[i][j] = -(A[i][j] * N[i]);
		}
	}

	//Calculate x, y, z
	for (int k = 0; k < iteration; k++)
	{
		for (int i = 0; i < n; i++)
		{
			result[i] = N[i] * B[i];
			for (int j = 0; j < n; j++)
				result[i] += M[i][j] * x[j];
		}
	}

	//Delete arrays
	{
		delete[] N;
		for (int i = 0; i < n; i++)
			delete[] M[i];
		delete[] M;
		delete[] x;
	}

	return result;
}

void Grid::calculate()
{
	unsigned long int nodeCount = input.getHorizontalNodeNumber()*input.getVerticalNodeNumber();
	long double **globalH = new long double *[nodeCount];
	long double **globalC = new long double *[nodeCount];
	long double *globalP = new long double [nodeCount];
	long double stepTime = input.getSimulationStepTime();

	//Create and fill arrays
	for (unsigned long int i = 0; i < nodeCount; i++)
	{
		globalH[i] = new long double[nodeCount];
		globalC[i] = new long double[nodeCount];

		for (unsigned long int j = 0; j < nodeCount; j++)
		{
			globalH[i][j] = 0;
			globalC[i][j] = 0;
		}

		globalP[i] = 0;
	}

	//Convert Local H, Local C, Local P into Global H, Global C, Global P
	{
		for (unsigned long int i = 0; i < elements.size(); i++)
		{
			Element *element = elements[i];
			std::array < std::array <long double, 4>, 4> localH = element->getLocalH();
			std::array < std::array <long double, 4>, 4> localC = element->getLocalC();
			std::array <long double, 4> localP = element->getLocalP();

			for (int j = 0; j < 4; j++)
			{
				unsigned long int id1 = element->getGlobalNodes()[j]->getId();

				for (int k = 0; k < 4; k++)
				{
					unsigned long int id2 = element->getGlobalNodes()[k]->getId();
					globalH[id1 - 1][id2 - 1] += localH[j][k];
					globalC[id1 - 1][id2 - 1] += localC[j][k];
				}

				globalP[id1 - 1] += localP[j];
			}
		}
	}

	//Calculate [H]=[H]+[C]/deltaTau
	{
		for (unsigned long int i = 0; i < nodeCount; i++)
		{
			for (unsigned long int j = 0; j < nodeCount; j++)
			{
				globalH[i][j] = globalC[i][j] / stepTime + globalH[i][j];
			}
		}
	}

	long double *tmpP = new long double[nodeCount];
	for (int z = 0; z < 2; z++)
	{
		std::cout << "ITERATION: " << z+1<<std::endl<<std::endl;
		{
			//Calculate {P}=([C]/deltaTau)*{t0}+{P}
			for (unsigned long int i = 0; i < nodeCount; i++)
			{
				long double value = 0;

				for (unsigned long int j = 0; j < nodeCount; j++)
				{
					value += globalC[i][j] / stepTime * nodes[i]->getT();
				}

				tmpP[i] = value + globalP[i];
			}
		}


		//Calculate {t1} by Gauss Elimination - [H]{t1}={P}
		{
			//Get new temperatures
			long double *t1 = gauss(globalH, tmpP, nodeCount);

			std::cout << "Temperatures" << std::endl;
			for (int i = 0; i < nodeCount; i++)
			{
				std::cout << t1[i]<<"	";
				nodes[i]->setT(t1[i]);
			}

			std::cout << std::endl << std::endl;

			delete[] t1;
		}

		std::cout << "[H]" << std::endl;
		for (int i = 0; i < nodeCount; i++)
		{
			for (int j = 0; j < nodeCount; j++)
			{
				std::cout << globalH[i][j]<<"  ";
			}

			std::cout << std::endl;
		}
		std::cout << std::endl << std::endl;

		std::cout << "{P}" << std::endl;
		for (int j = 0; j < nodeCount; j++)
		{
			std::cout << tmpP[j] << "  ";
		}
		std::cout << std::endl << std::endl;
	}

	for (unsigned long int i = 0; i < nodeCount; i++)
	{
		delete[] globalH[i];
		delete[] globalC[i];
	}

	delete[] globalH;
	delete[] globalC;
	delete[] globalP;
	delete[] tmpP;
}


void Grid::displayGrid()
{
	for (unsigned long int i = 0; i < elements.size(); i++)
	{
		std::cout<<elements[i]->getId();
		std::vector<GlobalNode *> nodes = elements[i]->getGlobalNodes();
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
	
	std::vector<GlobalNode *> globalNodes = element->getGlobalNodes();
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

	tmpArray = universalElement.getdNdEta();

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

	tmpArray = universalElement.getdNdKsi();

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
