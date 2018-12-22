#include "Grid.h"
#include "Input.h"

Grid::Grid()
{
	unsigned long int verticalNodeNumber = input.getVerticalNodeNumber();
	unsigned long int horizontalNodeNumber = input.getHorizontalNodeNumber();
	long double nodeHight = input.getGridHight()/ (verticalNodeNumber -1);
	long double nodeWidth = input.getGridWidth() / (horizontalNodeNumber -1);

	//Create Nodes
	{
		unsigned int id = 0;
		bool onBoundConvection;
		bool onBoundStream;
		long double initialTemperature = input.getInitialTemperature();
		for (unsigned long int i = 0; i < horizontalNodeNumber; i++)
		{
			for (unsigned long int j = 0; j < verticalNodeNumber; j++)
			{
				onBoundStream = false;
				onBoundConvection = false;

				//If the global node is on the left or the right edge
				if (i == 0 || i == horizontalNodeNumber - 1)
				{
					onBoundConvection = true;

					if (j == 0)
					{
						onBoundStream = true;
					}
				}
				//If the global node is neither on the left nor right edge, check if it is on top or bottom edge
				else
				{
					if (j == 0)
					{
						onBoundStream = true;
					}
					else if (j == verticalNodeNumber - 1)
					{
						onBoundConvection = true;
					}
				}

				nodes.push_back(new Node(++id, i*nodeWidth, j*nodeHight, onBoundConvection, onBoundStream, initialTemperature));
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
				std::array<int, 4> nodesId;
				nodesId[0] = j + verticalNodeNumber * i;
				nodesId[1] = j + verticalNodeNumber * (i + 1);
				nodesId[2] = j + 1 + verticalNodeNumber * (i + 1);
				nodesId[3] = j + 1 + verticalNodeNumber * i;

				std::array<Node *, 4> elementNodes;
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
	std::vector<long double> min;
	std::vector<long double> max;

	//display.universalElement(universalElement);
	//display.grid(elements, nodes);

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
			std::array < std::array <long double, 4>, 4> localH = element->getLocalH()->getMatrix();
			std::array < std::array <long double, 4>, 4> localC = element->getLocalC()->getMatrix();
			std::array <long double, 4> localP = element->getLocalP()->getVector();

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
			long double *t1 = equationSolver.gaussSeidel(globalH, tmpP, nodeCount, iterations);

			long double tmp_min=t1[0];
			long double tmp_max=t1[0];

			//Set new temperatures
			for (unsigned long int i = 0; i < nodeCount; i++)
			{
				if (tmp_min > t1[i])
				{
					tmp_min = t1[i];
				}

				if (tmp_max < t1[i])
				{
					tmp_max = t1[i];
				}

				nodes[i]->setT(t1[i]);
			}

			min.push_back(tmp_min);
			max.push_back(tmp_max);

			delete[] t1;
		}

		display.temperature(input, nodes);
		//display.H(nodeCount, globalH);
		//display.P(nodeCount, tmpP);
	}

	display.minmax(min, max);

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