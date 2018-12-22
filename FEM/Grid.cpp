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
		bool onBoundNaturalConvection;
		bool onBoundForcedConvection;
		bool onBoundStream;
		long double initialTemperature = input.getInitialTemperature();
		for (unsigned long int i = 0; i < horizontalNodeNumber; i++)
		{
			for (unsigned long int j = 0; j < verticalNodeNumber; j++)
			{
				onBoundNaturalConvection = false;
				onBoundForcedConvection = false;
				onBoundStream = false;

				//If the node is on the grid's left side
				if (i == 0)
				{
					onBoundNaturalConvection = true;

					if (j == 0)
					{
						onBoundStream = true;
					}
				}
				//If the node is on the grid's right side
				else if (i == horizontalNodeNumber - 1)
				{
					onBoundForcedConvection = true;

					if (j == 0)
					{
						onBoundStream = true;
					}
				}
				//If the global node is neither on the left nor right grid's side, check if it is on top or bottom grid's side
				else 
				{
					if (j == 0)
					{
						onBoundStream = true;
					}
					else if (j == verticalNodeNumber - 1)
					{
						onBoundNaturalConvection = true;
					}
				}

				nodes.push_back(new Node(++id, i*nodeWidth, j*nodeHight, onBoundNaturalConvection, onBoundForcedConvection, onBoundStream, initialTemperature));
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

				elements.push_back(new Element(++id, elementNodes, &universalElement, &input));
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

	std::vector<long double> minTemperature;
	std::vector<long double> maxTemperature;

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

	//Calculate and Convert Local H, Local C into Global H, Global C
	{
		Element *element;
		std::array < std::array <long double, 4>, 4> localHMatrix;
		std::array < std::array <long double, 4>, 4> localCMatrix;
		unsigned long int id1;
		unsigned long int id2;
		for (unsigned long int i = 0; i < elements.size(); i++)
		{
			element = elements[i];
			element->getLocalH()->calculate();
			element->getLocalC()->calculate();

			localHMatrix = element->getLocalH()->getMatrix();
			localCMatrix = element->getLocalC()->getMatrix();

			for (int j = 0; j < 4; j++)
			{
				id1 = element->getGlobalNodes()[j]->getId();

				for (int k = 0; k < 4; k++)
				{
					id2 = element->getGlobalNodes()[k]->getId();
					globalH[id1 - 1][id2 - 1] += localHMatrix[j][k];
					globalC[id1 - 1][id2 - 1] += localCMatrix[j][k];
				}
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
		display.velocity(&input);

		//In this case occurs acceleration so velocity is changing
		input.addVelocityStep();
		input.caluclateDensityStream();

		//Calculate and Convert Local P into Global P
		{
			Element *element;
			std::array <long double, 4> localPVector;
			unsigned long int id1;

			for (unsigned long int i = 0; i < nodeCount; i++)
			{
				globalP[i] = 0;
			}

			for (unsigned long int i = 0; i < elements.size(); i++)
			{
				element = elements[i];
				element->getLocalP()->calculate();
				localPVector = element->getLocalP()->getVector();

				for (int j = 0; j < 4; j++)
				{
					id1 = element->getGlobalNodes()[j]->getId();
					globalP[id1 - 1] += localPVector[j];
				}
			}
		}

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

			minTemperature.push_back(tmp_min);
			maxTemperature.push_back(tmp_max);

			delete[] t1;
		}

		display.temperature(&input, nodes);

		//display.H(nodeCount, globalH);
		//display.P(nodeCount, tmpP);
	}

	display.minmax(minTemperature, maxTemperature);

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