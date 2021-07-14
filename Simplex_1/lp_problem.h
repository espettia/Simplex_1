#pragma once
#include <iostream>
#include <vector>
#include "rational.h"
#include "matrix.h"

class lp_problem
{
private:

	//This is the original problem obj_funct + contraints
	//Second vector containts the comparisons in the constraints, whether they are >= = <= < >
	//third value especifies whether the function is required to be maxed or minimized
	matrix m;
	std::vector<int> op;
	int max_type = 0; //0 min, 1 max
	int integer = 0; //0 real, 1 integer
	int init = 0;//

public:

	lp_problem(matrix, std::vector<int>, int, int, int);
	lp_problem operator=(lp_problem);

	std::vector<rational> objective_function();
	//std::vector<rational> constraints();
	std::vector<rational> constant_terms();

	std::vector<int> basic_var();

	bool is_canonical(std::vector<rational>);
	//bool check_std();
	bool check_solved();
	bool check_unbounded();
	int check_finished();
	
	lp_problem to_min_problem();
	lp_problem add_slack_variables();
	lp_problem make_coefficients_positive();
	lp_problem add_artificial_variables();
	int solve(int z_w = 0);
	lp_problem remove_artificial_variables();
	int standard();
	
	lp_problem to_max_problem();
	lp_problem dual();
	//
	//std::vector<rational> solution();

	void print_op(size_t i);
	void print();


};

