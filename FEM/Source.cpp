#include "Grid.h"
#include "FileNotFoundException.h"
#include <iostream>


int main()
{
	try
	{
		Grid grid;

		//Display Elements with Nodes ID
		/*grid.displayGrid();

		system("pause");
		system("cls");*/

		//Display Element with node id and position
		//grid.displayElement(1);


		//Display Shape Function
		//grid.displayUniversalElement();

		grid.calculate();

		system("pause");
		system("cls");
	}
	catch (FileNotFoundException)
	{
		std::cout << "File Not Found!" << std::endl;
		system("pause");

		return 1;
	}

	return 0;
}