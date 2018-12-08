#include "Grid.h"
#include "FileNotFoundException.h"
#include <iostream>


int main()
{
	try
	{
		Grid grid;
		grid.calculate();

		system("pause");
	}
	catch (FileNotFoundException)
	{
		std::cout << "File Not Found!" << std::endl;
		system("pause");

		return 1;
	}

	return 0;
}