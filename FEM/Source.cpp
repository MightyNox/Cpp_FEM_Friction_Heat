#include "Input.h"
#include "Element.h"
#include "Grid.h"
#include "FileNotFoundException.h"


int main()
{
	try
	{
		Input input;

		Grid grid;
		grid.displayGrid();

		system("pause");
	}
	catch (FileNotFoundException)
	{
		return 1;
	}

	return 0;
}