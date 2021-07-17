#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <iomanip>
#include <string>
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
	//need to be included in constructors
	std::vector<rational> saved_objective_function;
	int st_saved_objective_function = 0;
	std::vector<rational> saved_constant_terms;
	std::vector<int> basic;
	int st_saved_constant_terms = 0;
	size_t artificial_variables = 0;
	//-----------------------------------
	int max_type = 0; //0 min, 1 max
	int max_obj = 0; //0min ,1 max
	int integer = 0; //0 real, 1 integer
	int st_init = 0;//
	size_t init_col = 0;
	size_t init_row = 0;
	int st_std = 0;
	int st_fin = 0;
	int st_nosol = 0;
	int st_sol = 0;
	int st_noadm = 0;
	int st_w = 0;
	size_t constraints = 0;
	size_t variables = 0;

	void save_objective_function();
	void restore_objective_function();
	void save_constant_terms();
	void restore_constant_terms();

	//bool check_dual_solved();
	//bool check_dual_unbounded();
	//bool check_dual_finished();

	bool check_solved();
	bool check_unbounded();
	bool check_finished();
	int check_w_valid();

	std::vector<size_t> simplex_find_pivot();
	std::vector<int> basic_var();

public:

	lp_problem(matrix m_out, std::vector<int> op_out, int type_out = 0, int obj_out = 0, int integer_out = 0, int init_out = 0, int col_out = 0, int row_out = 0, int std = 0, int fin = 0, int nosol = 0, int sol = 0, int noadm = 0, int w = 0, size_t constraints_out = 0, size_t variables_out = 0);
	lp_problem();

	std::vector<rational>& objective_function();
	std::vector<rational> constant_terms();

	void mult_eq(size_t i, rational q);

	lp_problem init();

	lp_problem to_min_obj();
	lp_problem to_max_obj();

	lp_problem to_min_problem();
	lp_problem to_max_problem();

	lp_problem add_slack_variables();
	lp_problem make_constants_positive();
	lp_problem add_artificial_variables();
	lp_problem solve();
	/*int solve_dual();*/
	lp_problem remove_artificial_variables();
	lp_problem standard();

	lp_problem dual();
	std::vector<rational> solution();

	lp_problem simplex();

	void print_op(size_t i);
	void print();


};

