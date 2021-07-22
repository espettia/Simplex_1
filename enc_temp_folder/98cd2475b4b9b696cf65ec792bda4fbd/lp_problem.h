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
	friend class method;

protected:

	//This is the original problem obj_funct + contraints
	//Second vector containts the comparisons in the constraints, whether they are >= = <= < >
	//third value especifies whether the function is required to be maxed or minimized
	matrix m;
	std::vector<int> op;
	std::vector<int> bd;
	std::string f;
	//-----------------------------------
	int max_type = 0;
	int max_obj = 0;
	size_t init_col = 0;
	size_t init_row = 0;
	size_t constraints = 0;
	size_t variables = 0;
	std::vector<int> basic_map_in;

public:

	//It is asumed that reader.h makes sure input is valid
	lp_problem(const matrix& m_out, const std::vector<int>& op_out, const std::vector<int>& bd_out, const std::string& f_out = "z")
		: m(m_out), 
		op(op_out), 
		bd(bd_out), 
		f(f_out)
	{
		max_type = op[0];
		max_obj = bd[0];
		init_col = m.columns();
		init_row = m.rows();
		constraints = init_row - 1;
		variables = init_col - 1;
		basic_map_in = std::vector<int>(m.columns() - 1, - 1);
	}

	lp_problem(const lp_problem& p_out) = default;
	lp_problem() = default;

	int type() {
		return max_type;
	}

	int obj() {
		return max_obj;
	}

	std::vector<matrix::matrix_vector> equations_iterate(){
		return std::vector<matrix::matrix_vector>(m.matrix_vector_rows.begin() + 1, m.matrix_vector_rows.end());
	}

	size_t initial_columns() {
		return init_col;
	}

	size_t initial_rows() {
		return init_row;
	}

	size_t number_of_constraints() {
		return constraints;
	}

	size_t number_of_variables() {
		return variables;
	}

	std::vector<int> boundaries() {
		return bd;
	}

	std::vector<int> operations() {
		return op;
	}

	size_t eq_size() {
		return variables + 1;
	}

	size_t var_size() {
		return constraints + 1;
	}

	matrix::matrix_vector& objective_function() {
		return m.first_r();
	}

	const std::vector<rational> objective_function_only_variables() {
		std::vector<rational> temp = objective_function().vector();
		return std::vector<rational> (temp.begin() + 1, temp.end());
	}


	matrix::matrix_vector& constant_terms() {
		return m.first_c();
	}

	const std::vector<rational> constant_terms_only_constraints() {
		std::vector<rational> temp = constant_terms().vector();
		return std::vector<rational>(temp.begin() + 1, temp.end());
	}

	matrix::matrix_vector& equation(int n) {
		return m.row(n);
	}

	const std::vector<rational> equation_only_variables(int n) {
		std::vector<rational> temp = equation(n).vector();
		return std::vector<rational>(temp.begin() + 1, temp.end());
	}

	matrix::matrix_vector& column(int n) {
		return m.col(n);
	}

	const std::vector<rational> column_only_constraints(int n) {
		std::vector<rational> temp = column(n).vector();
		return std::vector<rational>(temp.begin() + 1, temp.end());
	}

	matrix::matrix_vector& last_eq() {
		return m.last_r();
	}

	rational& independent_term() {
		return m.at(0, 0);
	}

	lp_problem& push_eq(const std::vector<rational>& eq_out, int op_out = 0);
	lp_problem& push_var(const std::vector<rational>& var_out, int bd_out = 1);
	lp_problem& push_var(int n, rational val = 1, int bd_out = 1);
	lp_problem& pop_eq();
	lp_problem& pop_var();

	lp_problem& piv(size_t i, size_t j);
	lp_problem& mult_eq(int i, rational q);

	lp_problem& to_min_obj();
	lp_problem& to_max_obj();

	lp_problem& to_min_problem();
	lp_problem& to_max_problem();
	lp_problem& add_slack_variables();
	std::vector<int> basic_map();

	lp_problem dual();
	

	void print_tableau();
	void print_op(size_t i);
	void print_problem();
};