#include "Grid.h"
#include "Input.h"


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
		bool onBound;
		for (unsigned long int i = 0; i < horizontalNodeNumber; i++)
		{
			for (unsigned long int j = 0; j < verticalNodeNumber; j++)
			{
				//If the global node is on the left or the right edge
				if (i == 0 || i == horizontalNodeNumber - 1)
				{
					onBound = true;
				}
				//If the global node is neither on the left nor right edge, check if it is on top or bottom edge
				else if (j == 0 || j == verticalNodeNumber - 1)
				{
					onBound = true;
				}
				else
				{
					onBound = false;
				}

				nodes.push_back(new GlobalNode(++id, i*nodeWidth, j*nodeHight, onBound, &input));
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

				std::array<GlobalNode *, 4> elementNodes;
				for (int k = 0; k < 4; k++)
				{
					elementNodes[k] = nodes[nodesId[k] - 1];
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


void Grid::calculate()
{
	unsigned long int nodeCount = input.getHorizontalNodeNumber()*input.getVerticalNodeNumber();
	long double **globalH = new long double *[nodeCount];
	long double **globalC = new long double *[nodeCount];
	long double *globalP = new long double [nodeCount];
	long double stepTime = input.getSimulationStepTime();
	long double simulationTime = input.getSimulationTime();
	unsigned long int iterations = input.getIterationsNumber();
	long double *tmpP = new long double[nodeCount];
	unsigned long int iterationNumber = 0;

	//Display
	//display.universalElement(universalElement);
	//display.grid(elements, nodes);
	//display.element(elements, 1);

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
				globalH[i][j] += globalC[i][j] / stepTime;
			}
		}
	}

	for (long double time = 0; time < simulationTime; time += stepTime)
	{
		//Display current iteration number
		display.iteration(++iterationNumber);

		//Calculate {P}=([C]/deltaTau)*{t0}+{P}
		{
			for (unsigned long int i = 0; i < nodeCount; i++)
			{
				long double value = 0;

				for (unsigned long int j = 0; j < nodeCount; j++)
				{
					value += globalC[i][j] / stepTime * nodes[j]->getT();
				}

				tmpP[i] = value + globalP[i];
			}
		}

		//Calculate {t1} by Gauss Elimination - [H]{t1}={P}
		{
			//Get new temperatures
			long double *t1 = gaussSeidel(globalH, tmpP, nodeCount, iterations);

			//Set new temperatures
			for (unsigned long int i = 0; i < nodeCount; i++)
			{
				nodes[i]->setT(t1[i]);
			}
			delete[] t1;
		}

		display.temperature(input, nodes);
		display.H(nodeCount, globalH);
		display.P(nodeCount, tmpP);
	}


	//Delete
	{
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
}


long double * Grid::gaussSeidel(long double **A, long double *B, unsigned long int n, unsigned long int iterations)
{
	long double **M, *N, *x, *result;

	//Create arrays
	{
		N = new long double[n];

		M = new long double*[n];

		x = new long double[n];

		result = new long double[n];

		for (unsigned long int i = 0; i < n; i++)
		{
			M[i] = new long double[n];
			x[i] = 0;
		}
	}

	//Calculate matrix N
	for (unsigned long int i = 0; i < n; i++)
	{
		N[i] = 1 / A[i][i];
	}

	//Calculate matrix M
	for (unsigned long int i = 0; i < n; i++)
	{
		for (unsigned long int j = 0; j < n; j++)
		{
			if (i == j)
				M[i][j] = 0;
			else
				M[i][j] = -(A[i][j] * N[i]);
		}
	}

	//Calculate results
	for (unsigned long int k = 0; k < iterations; k++)
	{
		for (unsigned long int i = 0; i < n; i++)
		{
			result[i] = N[i] * B[i];
			for (unsigned long int j = 0; j < n; j++)
				result[i] += M[i][j] * x[j];

			x[i] = result[i];
		}
	}

	//Delete arrays
	{
		for (unsigned long int i = 0; i < n; i++)
			delete[] M[i];
		delete[] N;
		delete[] M;
		delete[] x;
	}

	return result;
}