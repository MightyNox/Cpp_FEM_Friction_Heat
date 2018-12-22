#pragma once
class EquationSolver
{
public:
	EquationSolver();
	~EquationSolver();

	long double * gaussSeidel(long double **A, long double *B, unsigned long int n, unsigned long int iterations);
};

