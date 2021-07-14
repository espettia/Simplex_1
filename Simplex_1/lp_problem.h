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
	int max_obj = 0; //0min ,1 max
	int integer = 0; //0 real, 1 integer
	int st_init = 0;//
	size_t init_col = 0;
	int st_std = 0;
	int st_fin = 0;
	int st_nosol = 0;
	int st_sol = 0;
	int st_noadm = 0;
	int st_w = 0;


public:
	lp_problem(matrix m_out, std::vector<int> op_out, int type_out = 0, int integer_out = 0, int init_out = 0, int std = 0, int fin = 0, int nosol = 0, int sol = 0, int noadm = 0, int w = 0);
	lp_problem();
	lp_problem operator=(lp_problem);

	std::vector<rational> objective_function();
	//std::vector<rational> constraints();
	std::vector<rational> constant_terms();

	std::vector<int> basic_var();

	bool is_canonical(std::vector<std::reference_wrapper<rational>>);
	//bool check_std();
	bool check_dual_simplex();
	bool check_solved();
	bool check_unbounded();
	int check_finished();
	bool init();
	
	lp_problem to_min_problem();
	lp_problem add_slack_variables();
	lp_problem make_coefficients_positive();
	lp_problem add_artificial_variables();
	lp_problem solve();
	/*int solve_dual();*/
	lp_problem remove_artificial_variables();
	lp_problem standard();
	
	lp_problem to_max_problem();
	lp_problem dual();
	std::vector<rational> solution();

	void print_op(size_t i);
	void print();


};

