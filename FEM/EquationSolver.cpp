#include "EquationSolver.h"



EquationSolver::EquationSolver()
{
}


EquationSolver::~EquationSolver()
{
}


long double * EquationSolver::gaussSeidel(long double **A, long double *B, unsigned long int n, unsigned long int iterations)
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