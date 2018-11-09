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

		//Display Elements with Nodes ID
		/*grid.displayGrid();

		system("pause");
		system("cls");*/

		//Display Element with node id and position
		//grid.displayElement(1);


		//Display Shape Function
		//grid.displayUniversalElement();

		system("pause");
		system("cls");
	}
	catch (FileNotFoundException)
	{
		return 1;
	}

	return 0;
}